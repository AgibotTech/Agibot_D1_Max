/**
 * @file async.cpp
 * @brief Robot SDK Asynchronous Mode Example
 *
 * This example demonstrates how to communicate with the robot using
 * asynchronous mode, including:
 * - Asynchronous connection management (non-blocking connect/disconnect)
 * - Asynchronous command sending (light control)
 */

#include <atomic>
#include <chrono>
#include <csignal>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>

#include "robot_sdk/sdk_client.hpp"

using namespace robot_sdk;

// Global flags for graceful shutdown
static std::atomic<bool> g_running{true};
static std::atomic<bool> g_connected{false};

// Signal handler function
void SignalHandler(int signal) {
  std::cout << "\n[INFO] Received signal " << signal << ", shutting down..."
            << std::endl;
  g_running = false;
}

// Connection state string mapping
static const char* GetStateString(ConnectionState state) {
  switch (state) {
    case ConnectionState::DISCONNECTED:
      return "DISCONNECTED";
    case ConnectionState::CONNECTING:
      return "CONNECTING";
    case ConnectionState::HANDSHAKING:
      return "HANDSHAKING";
    case ConnectionState::CONNECTED:
      return "CONNECTED";
    case ConnectionState::DISCONNECTING:
      return "DISCONNECTING";
    case ConnectionState::RECONNECTING:
      return "RECONNECTING";
    default:
      return "UNKNOWN";
  }
}

// Data callback class - asynchronously receive robot sensor data
class AsyncDataCallback : public IDataCallback {
 public:
  void OnRobotStateData(const RobotState& data) override {
    static int count = 0;
    if (++count % 10 == 0) {  // Print every 10 times to avoid screen flooding
      std::cout << "[DATA] Robot State - Front Fill Light: "
                << static_cast<int>(data.front_fill_light) << std::endl;
    }
  }

  void OnFaultData(const FaultDatas& data) override {
    if (data.empty()) return;
    std::cout << "[FAULT] ⚠️  " << data.size()
              << " fault(s) detected:" << std::endl;
    for (const auto& fault : data) {
      std::cout << "  └─ Level: " << static_cast<int>(fault.level)
                << ", Code: " << static_cast<int>(fault.code)
                << ", Message: " << fault.message << std::endl;
    }
  }
};

// Control callback class - asynchronously receive control command confirmations
class AsyncControlCallback : public IControlCallback {
 public:
  void OnFrontLight(bool on) override {
    std::cout << "[CTRL] ✓ Front light: " << (on ? "🔆 ON" : "⬛ OFF")
              << std::endl;
  }

  void OnBackLight(bool on) override {
    std::cout << "[CTRL] ✓ Back light: " << (on ? "🔆 ON" : "⬛ OFF")
              << std::endl;
  }
};

// Asynchronous command queue manager
class AsyncCommandQueue {
 public:
  AsyncCommandQueue(SDKClient& client) : client_(client) {}

  void Start() {
    running_ = true;
    worker_thread_ = std::thread([this]() { this->ProcessCommands(); });
  }

  void Stop() {
    running_ = false;
    if (worker_thread_.joinable()) {
      worker_thread_.join();
    }
  }

 private:
  void ProcessCommands() {
    int step = 0;
    while (running_ && g_running) {
      if (!g_connected.load()) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        continue;
      }

      // Periodically toggle the fill light state
      switch (step) {
        case 0:
          SendLightControl(true);  // Turn on light
          break;
        case 10:
          SendLightControl(false);  // Turn off light
          break;
        case 20:
          step = -1;  // Loop again
          break;
      }

      step++;
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  }

  void SendLightControl(bool on) {
    auto ec =
        client_.FrontLight(on, 0, [](const std::error_code& ec, std::size_t) {
          if (ec) {
            std::cerr << "[ERROR] Front light command sent failed: "
                      << ec.message() << std::endl;
          } else {
            std::cout << "[INFO] Front light command sent successfully."
                      << std::endl;
          }
        });
    if (ec) {
      std::cerr << "[ERROR] Light control failed: " << ec.message()
                << std::endl;
    }
  }

  SDKClient& client_;
  std::thread worker_thread_;
  std::atomic<bool> running_{false};
};

int main(int argc, char* argv[]) {
  // Parameter validation
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " <ip> <port>" << std::endl;
    std::cerr << "  Example: " << argv[0] << " 192.168.234.1 8082" << std::endl;
    return EXIT_FAILURE;
  }

  std::string ip = argv[1];
  std::string port = argv[2];

  // Register signal handlers
  std::signal(SIGINT, SignalHandler);
  std::signal(SIGTERM, SignalHandler);

  std::cout << "========================================" << std::endl;
  std::cout << "   Robot SDK Asynchronous Mode Demo" << std::endl;
  std::cout << "========================================" << std::endl;
  std::cout << "Target: " << ip << ":" << port << std::endl;
  std::cout << "Press Ctrl+C to exit" << std::endl;
  std::cout << "========================================\n" << std::endl;

  try {
    // 1. Create SDK client
    SDKClient client;

    // 2. Set callbacks
    auto data_cb = std::make_shared<AsyncDataCallback>();
    auto ctrl_cb = std::make_shared<AsyncControlCallback>();
    client.SetDataCallback(data_cb);
    client.SetControlCallback(ctrl_cb);

    // 3. Asynchronous connection (non-blocking, returns immediately)
    std::cout << "[INFO] Initiating asynchronous connection..." << std::endl;
    auto ec = client.Connect(
        ip, port, false,  // block = false, asynchronous connection
        [](const std::error_code& ec) {
          if (!ec) {
            std::cout << "[INFO] ✓ Connection established successfully"
                      << std::endl;
            g_connected = true;
          } else {
            std::cerr << "[ERROR] Connection failed: " << ec.message()
                      << std::endl;
          }
        });

    if (ec) {
      std::cerr << "[ERROR] Failed to initiate connection: " << ec.message()
                << std::endl;
      return EXIT_FAILURE;
    }

    // 4. Wait for initial connection establishment
    std::cout << "[INFO] Waiting for connection to establish..." << std::endl;
    int timeout = 15;
    while (timeout-- > 0 && !g_connected.load() && g_running) {
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    if (!g_connected.load()) {
      std::cerr << "[ERROR] Connection timeout" << std::endl;
      g_running = false;
      return EXIT_FAILURE;
    }

    // 5. Start asynchronous command queue (periodically toggle fill light)
    std::cout << "[INFO] Starting light control loop..." << std::endl;
    AsyncCommandQueue cmd_queue(client);
    cmd_queue.Start();

    // 6. Main loop - monitor connection state
    std::cout << "\n[INFO] System running. Light will toggle every 10 seconds."
              << std::endl;

    while (g_running) {
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // 7. Graceful shutdown
    std::cout << "\n[INFO] Shutting down..." << std::endl;

    cmd_queue.Stop();
    std::cout << "[INFO] Disconnecting..." << std::endl;
    ec =
        client.Disconnect(true);  // Synchronous disconnect to ensure clean exit
    if (ec) {
      std::cerr << "[ERROR] Disconnect failed: " << ec.message() << std::endl;
    }

    std::cout << "\n[INFO] Final state: "
              << GetStateString(client.GetConnectionState()) << std::endl;
    std::cout << "[INFO] Program exited successfully" << std::endl;

  } catch (const std::exception& e) {
    std::cerr << "[FATAL] Exception: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

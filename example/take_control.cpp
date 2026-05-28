/**
 * @file take_control.cpp
 * @brief Robot SDK Control Ownership Management
 *
 * This example demonstrates control ownership related operations, including:
 * - Control ownership taken by APP
 * - SDK regains control after APP releases control
 * - Control ownership taken notification
 * - Control ownership released notification
 */

#include <atomic>
#include <chrono>
#include <condition_variable>
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

static const char* GetControlSourceString(CtrlSource source) {
  switch (source) {
    case CtrlSource::CTRL_SOURCE_UNKNOWN:
      return "UNKNOWN";
    case CtrlSource::CTRL_SOURCE_APP:
      return "APP";
    case CtrlSource::CTRL_SOURCE_SDK:
      return "SDK";
    case CtrlSource::CTRL_SOURCE_OTHER:
      return "OTHER";
    default:
      return "INVALID";
  }
}

/// @brief Control ownership management
class TakeControlManager {
 public:
  TakeControlManager(SDKClient& client) : client_(client) {}

  ~TakeControlManager() { Stop(); }
  void Start() {
    running_ = true;
    take_control_thread_ = std::thread([this]() { this->TakeControlLoop(); });
  }

  void Stop() {
    running_ = false;
    take_control_cv_.notify_one();
    if (take_control_thread_.joinable()) {
      take_control_thread_.join();
    }
  }

  void TriggerTakeControl() {
    std::lock_guard<std::mutex> lock(take_control_mtx_);
    take_control_cv_.notify_one();
  }

 private:
  void TakeControlLoop() {
    while (running_) {
      {
        std::unique_lock<std::mutex> lock(take_control_mtx_);
        take_control_cv_.wait(lock);
      }

      if (!running_) break;

      auto ec =
          client_.TakeControl(5000);  // Synchronous mode, 5 second timeout
      if (!ec) {
        std::cout << "[TAKE_CONTROL] ✓ Successfully took control" << std::endl;
        continue;
      }

      std::cerr << "[TAKE_CONTROL] × Failed: " << ec.message() << std::endl;
    }
  }

  SDKClient& client_;
  std::thread take_control_thread_;
  std::mutex take_control_mtx_;
  std::condition_variable take_control_cv_;
  std::atomic<bool> running_{false};
};

// Data Callback Class - asynchronously receive robot sensor data
class AsyncDataCallback : public IDataCallback {
 public:
  AsyncDataCallback() = default;
  void SetTakeControlManager(TakeControlManager* mgr) {
    take_control_mgr_ = mgr;
  }

 public:
  void OnRobotStateData(const RobotState& data) override {
    static CtrlSource control_source = CtrlSource::CTRL_SOURCE_UNKNOWN;
    if (data.control_source != control_source) {
      control_source = data.control_source;
      std::cout << "[DATA] Control Source Changed: "
                << GetControlSourceString(control_source) << std::endl;
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

  void OnControlLost(const ControlLostInfo& info) override {
    std::cout << "[DATA] ⚠️  Control lost notification received." << std::endl;
  }

  void OnControlAvailable(const ControlAvailableInfo& info) override {
    std::cout << "[DATA] ✅ Control available notification received."
              << std::endl;
    take_control_mgr_->TriggerTakeControl();
  }

 private:
  TakeControlManager* take_control_mgr_;
};

// Control Callback Class - asynchronously receive control command confirmations
class AsyncControlCallback : public IControlCallback {
 public:
  void OnTakeControlAck(const TakeControlAck& ack) override {
    std::cout << "[CTRL] ✓ Take Control Ack: "
              << ((ack.error_code == 0) ? "SUCCESS" : "FAILURE")
              << ", Reason: " << ack.reason << std::endl;
  }
};

int main(int argc, char* argv[]) {
  // Parameter validation
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " <ip> <port>" << std::endl;
    std::cerr << "  Example: " << argv[0] << " 192.168.234.1 8081" << std::endl;
    return EXIT_FAILURE;
  }

  std::string ip = argv[1];
  std::string port = argv[2];

  // Register signal handlers
  std::signal(SIGINT, SignalHandler);
  std::signal(SIGTERM, SignalHandler);

  std::cout << "========================================" << std::endl;
  std::cout << "   Robot SDK Take Control Demo" << std::endl;
  std::cout << "========================================" << std::endl;
  std::cout << "Target: " << ip << ":" << port << std::endl;
  std::cout << "Press Ctrl+C to exit" << std::endl;
  std::cout << "========================================\n" << std::endl;

  try {
    // 1. Create SDK client
    SDKClient client;

    // Initialize take control manager
    TakeControlManager take_control_mgr(client);
    take_control_mgr.Start();

    // 2. Set callbacks
    auto data_cb = std::make_shared<AsyncDataCallback>();
    auto ctrl_cb = std::make_shared<AsyncControlCallback>();
    data_cb->SetTakeControlManager(&take_control_mgr);
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

    // 6. Main loop - wait for APP takeover and control release notifications
    std::cout << "\n[INFO] System running." << std::endl;

    while (g_running) {
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // 7. Graceful shutdown
    std::cout << "\n[INFO] Shutting down..." << std::endl;

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

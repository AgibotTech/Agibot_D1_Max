/**
 * @file manual_reconnect.cpp
 * @brief Robot SDK Manual Reconnection Example
 *
 * This example demonstrates how to implement manual reconnection mechanism at
 * application layer:
 * - Asynchronous command sending, errors handled through callbacks
 * - Independent reconnection thread, ensuring main control loop is not blocked
 * - Graceful thread exit and resource cleanup
 */

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <csignal>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <unordered_map>

#include "robot_sdk/sdk_client.hpp"

using namespace robot_sdk;

// Global flags for graceful shutdown
static std::atomic<bool> g_running{true};

// Signal handler function
void SignalHandler(int signal) {
  std::cout << "\n[INFO] Received signal " << signal << ", shutting down..."
            << std::endl;
  g_running = false;
}

// Connection state string mappings
static const std::unordered_map<ConnectionState, const char*> g_state_map = {
    {ConnectionState::DISCONNECTED, "DISCONNECTED"},
    {ConnectionState::CONNECTING, "CONNECTING"},
    {ConnectionState::HANDSHAKING, "HANDSHAKING"},
    {ConnectionState::CONNECTED, "CONNECTED"},
    {ConnectionState::DISCONNECTING, "DISCONNECTING"},
    {ConnectionState::RECONNECTING, "RECONNECTING"}};

static const char* GetStateString(ConnectionState state) {
  auto it = g_state_map.find(state);
  return (it != g_state_map.end()) ? it->second : "UNKNOWN";
}

// Data callback class
class UserDataCallback : public IDataCallback {
 public:
  void OnRobotStateData(const RobotState& data) override {
    // Handle robot state data as needed
  }

  void OnFaultData(const FaultDatas& data) override {
    if (data.empty()) return;
    std::cout << "[FAULT] Detected " << data.size()
              << " fault(s):" << std::endl;
    for (const auto& fault : data) {
      std::cout << "  └─ Level: " << static_cast<int>(fault.level)
                << ", Code: " << static_cast<int>(fault.code)
                << ", Message: " << fault.message << std::endl;
    }
  }
};

// Manual reconnection manager
class ManualReconnectManager {
 public:
  ManualReconnectManager(SDKClient& client, const std::string& ip,
                         const std::string& port)
      : client_(client), ip_(ip), port_(port) {}

  ~ManualReconnectManager() { Stop(); }

  void Start() {
    running_ = true;
    reconnect_thread_ = std::thread([this]() { this->ReconnectLoop(); });
  }

  void Stop() {
    running_ = false;
    reconnect_cv_.notify_one();
    if (reconnect_thread_.joinable()) {
      reconnect_thread_.join();
    }
  }

  void TriggerReconnect() {
    std::lock_guard<std::mutex> lock(reconnect_mtx_);
    reconnect_cv_.notify_one();
  }

 private:
  void ReconnectLoop() {
    while (running_) {
      // Wait for reconnection trigger
      {
        std::unique_lock<std::mutex> lock(reconnect_mtx_);
        reconnect_cv_.wait(lock);
      }

      if (!running_) break;

      std::cout << "\n[RECONNECT] Starting reconnection process..."
                << std::endl;

      // 1. First disconnect the current connection
      client_.Disconnect(true);

      // 2. Loop to retry reconnection
      int retry_count = 0;
      while (running_) {
        retry_count++;
        std::cout << "[RECONNECT] Attempt #" << retry_count << "..."
                  << std::endl;

        auto ec = client_.Connect(ip_, port_, true);
        if (!ec) {
          std::cout << "[RECONNECT] ✓ Reconnected successfully" << std::endl;
          break;
        }

        std::cerr << "[RECONNECT] × Failed: " << ec.message() << std::endl;

        // Reconnection failed, clean up connection state
        client_.Disconnect(true);

        // Wait before retrying
        std::this_thread::sleep_for(std::chrono::seconds(2));
      }
    }
  }

  SDKClient& client_;
  std::string ip_;
  std::string port_;
  std::thread reconnect_thread_;
  std::mutex reconnect_mtx_;
  std::condition_variable reconnect_cv_;
  std::atomic<bool> running_{false};
};

int main(int argc, char* argv[]) {
  // Install signal handlers
  std::signal(SIGINT, SignalHandler);
  std::signal(SIGTERM, SignalHandler);

  // Parse command line arguments
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " <ip> <port>" << std::endl;
    std::cerr << "Example: " << argv[0] << " 192.168.1.100 8001" << std::endl;
    return EXIT_FAILURE;
  }

  const std::string ip = argv[1];
  const std::string port = argv[2];

  std::cout << "========================================" << std::endl;
  std::cout << "Robot SDK - Manual Reconnect Example" << std::endl;
  std::cout << "========================================" << std::endl;
  std::cout << "Target: " << ip << ":" << port << std::endl;
  std::cout << "Press Ctrl+C to exit" << std::endl;
  std::cout << "========================================\n" << std::endl;

  // Initialize SDK client
  SDKClient sdk_client;

  auto data_callback = std::make_shared<UserDataCallback>();
  sdk_client.SetDataCallback(data_callback);

  // Initialize manual reconnection manager
  ManualReconnectManager reconnect_mgr(sdk_client, ip, port);

  // Start reconnection manager
  reconnect_mgr.Start();

  // Execute initial connection
  std::cout << "[INIT] Connecting to robot..." << std::endl;
  auto ec = sdk_client.Connect(ip, port, true);
  if (ec) {
    std::cerr << "[ERROR] Initial connection failed: " << ec.message()
              << std::endl;
    return EXIT_FAILURE;
  } else {
    std::cout << "[INIT] ✓ Connected successfully\n" << std::endl;
  }

  // Monitor connection state changes
  ConnectionState last_state = sdk_client.GetConnectionState();
  std::cout << "[STATE] Initial state: " << GetStateString(last_state)
            << std::endl;

  // Main loop
  while (g_running) {
    if (sdk_client.GetConnectionState() == ConnectionState::CONNECTED) {
      // Synchronous mode: send test commands
      // if (auto ec = sdk_client.Move(0.0, 0.0, 0.0, 1000)) {
      //   std::cerr << "[ERROR] Move command failed, " << ec.message()
      //             << std::endl;
      //   reconnect_mgr.TriggerReconnect();
      // }

      // Asynchronous mode: send test commands
      sdk_client.Move(0.0, 0.0, 0.0, 0,
                      [&reconnect_mgr](const std::error_code& ec, std::size_t) {
                        if (ec) {
                          std::cerr << "[ERROR] Move command failed, "
                                    << ec.message() << std::endl;
                          reconnect_mgr.TriggerReconnect();
                        }
                      });
    }

    // Print when state changes
    ConnectionState current_state = sdk_client.GetConnectionState();
    if (current_state != last_state) {
      std::cout << "[STATE] Connection state changed: "
                << GetStateString(last_state) << " → "
                << GetStateString(current_state) << std::endl;
      last_state = current_state;
    }
    // Check state every 10ms
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  // Clean up and exit
  std::cout << "\n[SHUTDOWN] Cleaning up..." << std::endl;
  reconnect_mgr.Stop();
  sdk_client.Disconnect(true);
  std::cout << "[SHUTDOWN] ✓ Exited cleanly" << std::endl;

  return EXIT_SUCCESS;
}
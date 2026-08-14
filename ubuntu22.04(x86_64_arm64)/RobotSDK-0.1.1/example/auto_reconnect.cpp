/**
 * @file auto_reconnect.cpp
 * @brief Robot SDK Auto-Reconnection Example
 *
 * This example demonstrates SDK's built-in auto-reconnection feature:
 * - Enable auto_reconnect configuration, SDK automatically handles
 * disconnection and reconnection
 * - Application layer requires no additional reconnection logic
 */

#include <atomic>
#include <chrono>
#include <csignal>
#include <iostream>
#include <memory>
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

// Connection state string helper function
static const char* GetStateString(ConnectionState state) {
  static const std::unordered_map<ConnectionState, const char*> state_map = {
      {ConnectionState::DISCONNECTED, "DISCONNECTED"},
      {ConnectionState::CONNECTING, "CONNECTING"},
      {ConnectionState::HANDSHAKING, "HANDSHAKING"},
      {ConnectionState::CONNECTED, "CONNECTED"},
      {ConnectionState::DISCONNECTING, "DISCONNECTING"},
      {ConnectionState::RECONNECTING, "RECONNECTING"}};
  auto it = state_map.find(state);
  return (it != state_map.end()) ? it->second : "UNKNOWN";
}

// Data callback class
class DataCallback : public IDataCallback {
 public:
  void OnRobotStateData(const RobotState& data) override {
    // Periodically receive robot state data (process as needed)
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

// Control callback class (add callback methods as needed)
class ControlCallback : public IControlCallback {};

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
  std::cout << "Robot SDK - Auto Reconnect Example" << std::endl;
  std::cout << "========================================" << std::endl;
  std::cout << "Target: " << ip << ":" << port << std::endl;
  std::cout << "Auto-reconnect: ENABLED" << std::endl;
  std::cout << "Press Ctrl+C to exit" << std::endl;
  std::cout << "========================================\n" << std::endl;

  // Configure auto-reconnection
  ConnectionConfig config;
  config.auto_reconnect = true;  // Enable auto-reconnection

  // Initialize SDK client
  SDKClient sdk_client([](const std::error_code& ec) {}, config);

  // Set callbacks
  auto data_cb = std::make_shared<DataCallback>();
  auto ctrl_cb = std::make_shared<ControlCallback>();
  sdk_client.SetDataCallback(data_cb);
  sdk_client.SetControlCallback(ctrl_cb);

  // Use synchronous mode for connection
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

  // Main loop: simulate 100Hz command sending
  while (g_running) {
    ConnectionState current_state = sdk_client.GetConnectionState();

    // Print when state changes
    if (current_state != last_state) {
      std::cout << "[STATE] Connection state changed: "
                << GetStateString(last_state) << " → "
                << GetStateString(current_state) << std::endl;
      last_state = current_state;
    }
    // Send velocity commands
    sdk_client.Move(0.0, 0.0, 0.0);

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  // Clean up and exit
  std::cout << "\n[SHUTDOWN] Disconnecting..." << std::endl;
  sdk_client.Disconnect(true);
  std::cout << "[SHUTDOWN] ✓ Exited cleanly" << std::endl;

  return EXIT_SUCCESS;
}
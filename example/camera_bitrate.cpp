/**
 * @file camera_bitrate.cpp
 * @brief Robot Camera Bitrate Configuration Example
 *
 * This example demonstrates how to configure the bitrate of front and rear
 * cameras through SDK. Supports command line input format: f,2000 (set front
 * camera to 2000kbps)
 *
 * Applicable scenarios:
 * - Adjust video stream quality
 * - Optimize network bandwidth usage
 * - SDK API feature demonstration
 */

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <csignal>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>

#include "robot_sdk/sdk_client.hpp"

using namespace robot_sdk;

// Global control flags
static std::atomic<bool> g_running{true};

// Synchronization primitives
static std::mutex g_connect_mtx;
static std::condition_variable g_connect_cv;

// Camera operation callback class
class CameraCallback : public IControlCallback {
 public:
  void OnUpdateCameraBitrateAck(const CameraBitrateAck& ack) override {
    std::cout << "[✓] " << ack.camera_name
              << " Camera Bitrate: " << ack.camera_bps << " bps" << std::endl;
  }
};

// Signal handler function
void SignalHandler(int signal) {
  std::cout << "\n[INFO] Shutdown signal received" << std::endl;
  g_running = false;
}

/**
 * @brief Print help information
 */
void PrintHelp() {
  std::cout << "\n====== Camera Bitrate Commands ======" << std::endl;
  std::cout << "  f,<bitrate>  - Set front camera (e.g., f,200000)"
            << std::endl;
  std::cout << "  b,<bitrate>  - Set rear camera (e.g., b,150000)" << std::endl;
  std::cout << "  h            - Show help" << std::endl;
  std::cout << "  q            - Quit" << std::endl;
  std::cout << "====================================\n" << std::endl;
  std::cout << "Bitrate range: 50000-100000000 bps\n" << std::endl;
}

/**
 * @brief Parse and process commands
 * @param input Input string
 * @param sdk_client SDK client reference
 */
void ProcessCommand(const std::string& input, SDKClient& sdk_client) {
  if (input.empty()) return;

  char cmd = input[0];

  // Handle commands with parameters (format: f,2000 or b,1500)
  if ((cmd == 'f' || cmd == 'F' || cmd == 'b' || cmd == 'B') &&
      input.length() > 1 && input[1] == ',') {
    try {
      uint32_t bitrate = std::stoul(input.substr(2));

      if (bitrate < 50000 || bitrate > 100000000) {
        std::cout << "[ERROR] Bitrate must be 50000-100000000 bps" << std::endl;
        return;
      }

      CameraBitrateCmd cmd_data;
      cmd_data.camera_bps = bitrate;

      if (cmd == 'f' || cmd == 'F') {
        cmd_data.camera_name = "camera_front";
        std::cout << "[INFO] Setting front camera to " << bitrate << " bps..."
                  << std::endl;
      } else {
        cmd_data.camera_name = "camera_back";
        std::cout << "[INFO] Setting back camera to " << bitrate << " bps..."
                  << std::endl;
      }

      auto ec = sdk_client.UpdateCameraBitrate(cmd_data, 3000);
      if (ec) {
        std::cerr << "[ERROR] " << ec.message() << std::endl;
      }
    } catch (const std::exception& e) {
      std::cout << "[ERROR] Invalid format or bitrate: " << e.what()
                << std::endl;
      std::cout << "[INFO] Use: f,2000 or b,1500" << std::endl;
    }
    return;
  }

  // Handle single character commands
  switch (cmd) {
    case 'h':
    case 'H':
      PrintHelp();
      break;
    case 'q':
    case 'Q':
      std::cout << "[INFO] Quit command received" << std::endl;
      g_running = false;
      break;
    default:
      std::cout << "[WARN] Unknown command: '" << cmd << "' (Press H for help)"
                << std::endl;
      break;
  }
}

int main(int argc, char* argv[]) {
  // Install signal handlers
  std::signal(SIGINT, SignalHandler);
  std::signal(SIGTERM, SignalHandler);

  // Parse command line arguments
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " <ip> <port>" << std::endl;
    std::cerr << "Example: " << argv[0] << " 192.168.234.1 8081" << std::endl;
    return EXIT_FAILURE;
  }

  const std::string ip = argv[1];
  const std::string port = argv[2];

  std::cout << "========================================" << std::endl;
  std::cout << "  Robot SDK Camera Bitrate Demo" << std::endl;
  std::cout << "========================================" << std::endl;
  std::cout << "Target: " << ip << ":" << port << std::endl;
  std::cout << "========================================\n" << std::endl;

  // Initialize SDK client
  SDKClient sdk_client;

  std::cout << "[INFO] SDK Version: " << sdk_client.Version() << std::endl;
  std::cout << "[INFO] Protocol Version: " << sdk_client.ProtocolVersion()
            << "\n"
            << std::endl;

  // Set callbacks
  auto camera_cb = std::make_shared<CameraCallback>();
  sdk_client.SetControlCallback(camera_cb);

  // Connect to robot
  std::cout << "[INIT] Connecting to robot..." << std::endl;
  {
    std::unique_lock<std::mutex> lock(g_connect_mtx);
    sdk_client.Connect(ip, port, false, [](const std::error_code& ec) {
      std::unique_lock<std::mutex> lock(g_connect_mtx);
      if (ec) {
        std::cerr << "[ERROR] Connect failed: " << ec.message() << std::endl;
      } else {
        std::cout << "[INIT] ✓ Connected successfully" << std::endl;
      }
      g_connect_cv.notify_one();
    });

    // Wait for connection to complete
    auto status = g_connect_cv.wait_for(lock, std::chrono::seconds(10));
    if (status == std::cv_status::timeout) {
      std::cerr << "[ERROR] Connection timeout" << std::endl;
      return EXIT_FAILURE;
    }
  }

  // Check connection status
  if (sdk_client.GetConnectionState() != ConnectionState::CONNECTED) {
    std::cerr << "[ERROR] Not connected" << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << "[INFO] System Version: " << sdk_client.SystemVersion() << "\n"
            << std::endl;

  // Print help
  PrintHelp();

  // Main loop
  std::cout << "[READY] Enter command (h for help):\n" << std::endl;
  std::string line;
  while (g_running && std::getline(std::cin, line)) {
    ProcessCommand(line, sdk_client);
  }

  // Disconnect
  std::cout << "\n[SHUTDOWN] Disconnecting..." << std::endl;
  {
    std::unique_lock<std::mutex> lock(g_connect_mtx);
    sdk_client.Disconnect(false, [](const std::error_code& ec) {
      std::unique_lock<std::mutex> lock(g_connect_mtx);
      if (!ec) {
        std::cout << "[SHUTDOWN] ✓ Disconnected" << std::endl;
      }
      g_connect_cv.notify_one();
    });

    g_connect_cv.wait_for(lock, std::chrono::seconds(3));
  }

  std::cout << "[SHUTDOWN] ✓ Exited" << std::endl;
  return EXIT_SUCCESS;
}

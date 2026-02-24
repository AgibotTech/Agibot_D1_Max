/**
 * @file sync.cpp
 * @brief Robot SDK 同步模式示例
 *
 * 本示例展示如何使用同步模式与机器人进行通信，包括：
 * - 连接管理（同步连接/断开）
 * - 姿态控制（站立、趴下、爬行等）
 * - 灯光控制
 */

#include <chrono>
#include <csignal>
#include <iostream>
#include <memory>
#include <thread>

#include "robot_sdk/sdk_client.hpp"

using namespace robot_sdk;

// 全局标志，用于优雅退出
static volatile bool g_running = true;

// 信号处理函数
void SignalHandler(int signal) {
  std::cout << "\n[INFO] Received signal " << signal << ", shutting down..."
            << std::endl;
  g_running = false;
}

// 状态字符串映射
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

// 数据回调类 - 接收机器人传感器数据
class DataCallback : public IDataCallback {
 public:
  void OnRobotStateData(const RobotState& data) override {}

  void OnFaultData(const FaultDatas& data) override {
    if (data.empty()) return;
    std::cout << "[WARN] Fault detected: " << data.size() << " fault(s)"
              << std::endl;
    for (const auto& fault : data) {
      std::cout << "  - Level: " << static_cast<int>(fault.level)
                << ", Code: " << static_cast<int>(fault.code)
                << ", Message: " << fault.message << std::endl;
    }
  }
};

// 控制回调类 - 接收控制命令的确认
class ControlCallback : public IControlCallback {
 public:
  void OnStandUp() override {
    std::cout << "[CTRL] Robot confirmed: Stand Up" << std::endl;
  }

  void OnLieDown() override {
    std::cout << "[CTRL] Robot confirmed: Lie Down" << std::endl;
  }

  void OnCrawl() override {
    std::cout << "[CTRL] Robot confirmed: Crawl" << std::endl;
  }

  void OnFrontLight(bool on) override {
    std::cout << "[CTRL] Front light: " << (on ? "ON" : "OFF") << std::endl;
  }

  void OnBackLight(bool on) override {
    std::cout << "[CTRL] Back light: " << (on ? "ON" : "OFF") << std::endl;
  }
};

// 错误处理函数
bool HandleError(const std::error_code& ec, const char* operation) {
  if (ec) {
    std::cerr << "[ERROR] " << operation << " failed: " << ec.message()
              << " (code: " << ec.value() << ")" << std::endl;
    return false;
  }
  return true;
}

// 等待连接就绪
bool WaitForConnection(SDKClient& client, int timeout_sec = 10) {
  std::cout << "[INFO] Waiting for connection..." << std::endl;
  auto start = std::chrono::steady_clock::now();

  while (std::chrono::steady_clock::now() - start <
         std::chrono::seconds(timeout_sec)) {
    if (client.IsConnected()) {
      std::cout << "[INFO] Connection established successfully" << std::endl;
      return true;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  std::cerr << "[ERROR] Connection timeout after " << timeout_sec << " seconds"
            << std::endl;
  return false;
}

// 演示姿态控制
void DemoPoseControl(SDKClient& client) {
  std::cout << "\n=== Pose Control Demo ===" << std::endl;

  std::cout << "[CMD] Standing up..." << std::endl;
  if (!HandleError(client.StandUp(3000), "StandUp")) return;
  std::this_thread::sleep_for(std::chrono::seconds(2));

  std::cout << "[CMD] Lying down..." << std::endl;
  if (!HandleError(client.LieDown(3000), "LieDown")) return;
  std::this_thread::sleep_for(std::chrono::seconds(2));

  std::cout << "[CMD] Standing up again..." << std::endl;
  if (!HandleError(client.StandUp(3000), "StandUp")) return;
  std::this_thread::sleep_for(std::chrono::seconds(1));
}

// 演示灯光控制
void DemoLightControl(SDKClient& client) {
  std::cout << "\n=== Light Control Demo ===" << std::endl;

  std::cout << "[CMD] Turning on front light..." << std::endl;
  if (!HandleError(client.FrontLight(true, 3000), "FrontLight ON")) return;
  std::this_thread::sleep_for(std::chrono::seconds(2));

  std::cout << "[CMD] Turning off front light..." << std::endl;
  if (!HandleError(client.FrontLight(false, 3000), "FrontLight OFF")) return;
  std::this_thread::sleep_for(std::chrono::seconds(1));

  std::cout << "[CMD] Turning on back light..." << std::endl;
  if (!HandleError(client.BackLight(true, 3000), "BackLight ON")) return;
  std::this_thread::sleep_for(std::chrono::seconds(2));

  std::cout << "[CMD] Turning off back light..." << std::endl;
  if (!HandleError(client.BackLight(false, 3000), "BackLight OFF")) return;
}

int main(int argc, char* argv[]) {
  // 参数检查
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " <ip> <port>" << std::endl;
    std::cerr << "  Example: " << argv[0] << " 192.168.234.1 8081" << std::endl;
    return EXIT_FAILURE;
  }

  std::string ip = argv[1];
  std::string port = argv[2];

  // 注册信号处理
  std::signal(SIGINT, SignalHandler);
  std::signal(SIGTERM, SignalHandler);

  std::cout << "========================================" << std::endl;
  std::cout << "   Robot SDK Synchronous Mode Demo" << std::endl;
  std::cout << "========================================" << std::endl;
  std::cout << "Target: " << ip << ":" << port << std::endl;
  std::cout << "Press Ctrl+C to exit" << std::endl;
  std::cout << "========================================\n" << std::endl;

  try {
    // 1. 创建 SDK 客户端
    SDKClient client;

    // 2. 设置回调
    auto data_cb = std::make_shared<DataCallback>();
    auto ctrl_cb = std::make_shared<ControlCallback>();
    client.SetDataCallback(data_cb);
    client.SetControlCallback(ctrl_cb);

    // 3. 同步连接
    std::cout << "[INFO] Connecting to robot..." << std::endl;
    auto ec = client.Connect(ip, port, true);  // block = true，同步等待
    if (!HandleError(ec, "Connect")) {
      return EXIT_FAILURE;
    }

    std::cout << "[INFO] Connection state: "
              << GetStateString(client.GetConnectionState()) << std::endl;

    // 4. 等待连接完全建立（握手完成）
    if (!WaitForConnection(client, 10)) {
      client.Disconnect(true);
      return EXIT_FAILURE;
    }

    // 5. 执行控制演示
    // if (g_running) DemoPoseControl(client);
    if (g_running) DemoLightControl(client);

    std::cout << "[INFO] Press Ctrl+C to exit\n" << std::endl;

    while (g_running && client.IsConnected()) {
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // 7. 优雅退出
    std::cout << "\n[INFO] Disconnecting..." << std::endl;
    ec = client.Disconnect(true);
    HandleError(ec, "Disconnect");

    std::cout << "[INFO] Final state: "
              << GetStateString(client.GetConnectionState()) << std::endl;
    std::cout << "[INFO] Program exited successfully" << std::endl;

  } catch (const std::exception& e) {
    std::cerr << "[FATAL] Exception: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

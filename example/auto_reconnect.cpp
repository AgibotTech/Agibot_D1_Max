/**
 * @file auto_reconnect.cpp
 * @brief Robot SDK 自动重连示例
 *
 * 本示例展示 SDK 内置的自动重连功能：
 * - 启用 auto_reconnect 配置，SDK 自动处理断线重连
 * - 应用层无需额外的重连逻辑
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

// 全局标志，用于优雅退出
static std::atomic<bool> g_running{true};

// 信号处理函数
void SignalHandler(int signal) {
  std::cout << "\n[INFO] Received signal " << signal << ", shutting down..."
            << std::endl;
  g_running = false;
}

// 状态字符串辅助函数
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

// 数据回调类
class DataCallback : public IDataCallback {
 public:
  void OnRobotStateData(const RobotState& data) override {
    // 定期接收机器人状态数据（根据需要处理）
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

// 控制回调类（可根据需要添加回调方法）
class ControlCallback : public IControlCallback {};

int main(int argc, char* argv[]) {
  // 安装信号处理器
  std::signal(SIGINT, SignalHandler);
  std::signal(SIGTERM, SignalHandler);

  // 解析命令行参数
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

  // 配置自动重连
  ConnectionConfig config;
  config.auto_reconnect = true;  // 启用自动重连

  // 初始化 SDK 客户端
  SDKClient sdk_client([](const std::error_code& ec) {}, config);

  // 设置回调
  auto data_cb = std::make_shared<DataCallback>();
  auto ctrl_cb = std::make_shared<ControlCallback>();
  sdk_client.SetDataCallback(data_cb);
  sdk_client.SetControlCallback(ctrl_cb);

  // 使用同步模式进行连接
  std::cout << "[INIT] Connecting to robot..." << std::endl;
  auto ec = sdk_client.Connect(ip, port, true);
  if (ec) {
    std::cerr << "[ERROR] Initial connection failed: " << ec.message()
              << std::endl;
    return EXIT_FAILURE;
  } else {
    std::cout << "[INIT] ✓ Connected successfully\n" << std::endl;
  }

  // 监控连接状态变化
  ConnectionState last_state = sdk_client.GetConnectionState();
  std::cout << "[STATE] Initial state: " << GetStateString(last_state)
            << std::endl;

  // 主循环：模拟100Hz发送命令
  while (g_running) {
    ConnectionState current_state = sdk_client.GetConnectionState();

    // 状态变化时打印
    if (current_state != last_state) {
      std::cout << "[STATE] Connection state changed: "
                << GetStateString(last_state) << " → "
                << GetStateString(current_state) << std::endl;
      last_state = current_state;
    }
    // 发送速度命令
    sdk_client.Move(0.0, 0.0, 0.0);

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  // 清理退出
  std::cout << "\n[SHUTDOWN] Disconnecting..." << std::endl;
  sdk_client.Disconnect(true);
  std::cout << "[SHUTDOWN] ✓ Exited cleanly" << std::endl;

  return EXIT_SUCCESS;
}
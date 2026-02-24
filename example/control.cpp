/**
 * @file control.cpp
 * @brief Robot SDK 交互式控制示例
 *
 * 本示例展示完整的机器人交互式控制功能：
 * - 键盘输入实时控制机器人运动
 * - 模式切换、速度调节、姿态控制、灯光控制等命令
 * - 异步连接管理
 *
 * 适用场景：
 * - 机器人远程遥控
 * - 功能测试与调试
 * - SDK API 功能演示
 */

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <csignal>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <thread>

#include "robot_sdk/sdk_client.hpp"

using namespace robot_sdk;

// 全局控制标志
static std::atomic<bool> g_running{true};
static std::atomic<bool> g_front_light_on{false};
static std::atomic<bool> g_back_light_on{false};
static std::atomic<bool> g_auto_mode_light_on{false};
static std::atomic<bool> g_estop_on{false};

// 同步原语
static std::mutex g_connect_mtx;
static std::condition_variable g_connect_cv;

// 机器人状态缓存
static std::mutex g_state_mtx;
static RobotState g_robot_state;

// 状态字符串映射
const std::unordered_map<SportMode, const char*> g_sport_mode_map = {
    {SportMode::SPORT_MODE_GENERAL, "General"},
    {SportMode::SPORT_MODE_IN_PLACE, "InPlace"},
    {SportMode::SPORT_MODE_STAIR, "Stair"},
    {SportMode::SPORT_MODE_UNKNOWN, "Unknown"}};

const std::unordered_map<SpeedLevel, const char*> g_speed_level_map = {
    {SpeedLevel::SPEED_LEVEL_SLOW, "Slow"},
    {SpeedLevel::SPEED_LEVEL_MEDIUM, "Medium"},
    {SpeedLevel::SPEED_LEVEL_HIGH, "High"},
    {SpeedLevel::SPEED_LEVEL_UNKNOWN, "Unknown"}};

const std::unordered_map<MotionStatus, const char*> g_motion_status_map = {
    {MotionStatus::MOTION_STATUS_STAND_UP, "StandUp"},
    {MotionStatus::MOTION_STATUS_LIE_DOWN, "LieDown"},
    {MotionStatus::MOTION_STATUS_CRAWL, "Crawl"},
    {MotionStatus::MOTION_STATUS_LOCKED, "Locked"},
    {MotionStatus::MOTION_STATUS_GENERAL, "General"},
    {MotionStatus::MOTION_STATUS_IN_PLACE, "InPlace"},
    {MotionStatus::MOTION_STATUS_STAIR, "Stair"},
    {MotionStatus::MOTION_STATUS_GAIT, "Gait"},
    {MotionStatus::MOTION_STATUS_SLIM, "Slim"},
    {MotionStatus::MOTION_STATUS_CLIMB, "Climb"},
    {MotionStatus::MOTION_STATUS_UNKNOWN, "Unknown"}};

// 信号处理函数
void SignalHandler(int signal) {
  std::cout << "\n[INFO] Received signal " << signal << ", shutting down..."
            << std::endl;
  g_running = false;
}

/**
 * @brief 数据回调类 - 接收机器人传感器数据
 *
 * @note 回调函数必须轻量级，不能执行耗时操作
 * - 仅执行数据复制、数据校验等快速操作
 * - 耗时操作（数据库写入、文件I/O、网络发送等）需在独立线程处理
 */
class DataCallback : public IDataCallback {
 public:
  void OnRobotStateData(const RobotState& data) override {
    std::lock_guard<std::mutex> lock(g_state_mtx);
    g_robot_state = data;
  }

  void OnFaultData(const FaultDatas& data) override {
    if (data.empty()) return;
    std::cout << "\n[FAULT] Detected " << data.size()
              << " fault(s):" << std::endl;
    for (const auto& fault : data) {
      std::cout << "  └─ Level: " << static_cast<int>(fault.level)
                << ", Code: " << static_cast<int>(fault.code)
                << ", Message: " << fault.message << std::endl;
    }
  }

  void OnControlLost([[maybe_unused]] const ControlLostInfo& info) override {
    std::cout << "\n[WARN] Control Lost! " << std::endl;
  }

  void OnControlAvailable(
      [[maybe_unused]] const ControlAvailableInfo& info) override {
    std::cout << "\n[INFO] Control Available! " << std::endl;
  }
};

/**
 * @brief 控制回调类 - 接收机器人对控制命令的确认
 *
 * SDK 发送控制命令后，机器人会通过此回调确认命令已接收
 */
class ControlCallback : public IControlCallback {
 public:
  void OnSoftEmergencyStop(bool on) override {
    std::cout << "[CTRL] ✓ Emergency Stop: " << (on ? "ON" : "OFF")
              << std::endl;
  }

  void OnStandUp() override { std::cout << "[CTRL] ✓ Stand Up" << std::endl; }

  void OnLieDown() override { std::cout << "[CTRL] ✓ Lie Down" << std::endl; }

  void OnCrawl() override { std::cout << "[CTRL] ✓ Crawl" << std::endl; }

  void OnClimb() override { std::cout << "[CTRL] ✓ Climb" << std::endl; }

  void OnSlim() override { std::cout << "[CTRL] ✓ Slim" << std::endl; }

  void OnReverseHeadTail() override {
    std::cout << "[CTRL] ✓ Reverse Head/Tail" << std::endl;
  }

  void OnMode(int mode) override {
    const char* mode_name[] = {"Unknown", "General", "InPlace", "Stair"};
    std::cout << "[CTRL] ✓ Mode: " << (mode < 4 ? mode_name[mode] : "Invalid")
              << " (" << mode << ")" << std::endl;
  }

  void OnSpeed(int speed_level) override {
    const char* speed_name[] = {"Stop", "Low", "Medium", "High"};
    std::cout << "[CTRL] ✓ Speed: "
              << (speed_level < 4 ? speed_name[speed_level] : "Invalid") << " ("
              << speed_level << ")" << std::endl;
  }

  void OnLocked() override { std::cout << "[CTRL] ✓ Locked" << std::endl; }

  void OnFrontLight(bool on) override {
    std::cout << "[CTRL] ✓ Front Light: " << (on ? "ON" : "OFF") << std::endl;
  }

  void OnBackLight(bool on) override {
    std::cout << "[CTRL] ✓ Back Light: " << (on ? "ON" : "OFF") << std::endl;
  }

  void OnAutoModeLight(bool on) override {
    std::cout << "[CTRL] ✓ Auto Mode Light: " << (on ? "ON" : "OFF")
              << std::endl;
  }
  void OnTakeControlAck(const TakeControlAck& ack) override {
    if (ack.error_code == 0) {
      std::cout << "[CTRL] ✓ Take Control Success" << std::endl;
    } else {
      std::cout << "[CTRL] ✗ Take Control Failed, Reason: " << ack.reason
                << std::endl;
    }
  }
  void OnReleaseControlAck(const ReleaseControlAck& ack) override {
    if (ack.error_code == 0) {
      std::cout << "[CTRL] ✓ Release Control Success" << std::endl;
    } else {
      std::cout << "[CTRL] ✗ Release Control Failed, Reason: " << ack.reason
                << std::endl;
    }
  }
};

/**
 * @brief 输出机器人状态信息
 */
void PrintRobotState(const RobotState& data) {
  std::cout << "\n========== Robot State ==========" << std::endl;

  // 姿态信息
  std::cout << "[Pose]" << std::endl;
  std::cout << "  Head Angle: " << std::fixed << std::setprecision(2)
            << data.head_angle << "°" << std::endl;
  std::cout << "  Head Direction: " << static_cast<int>(data.head_direction)
            << std::endl;
  std::cout << "  Sport Mode: " << g_sport_mode_map.at(data.sport_mode)
            << std::endl;
  std::cout << "  Motion Status: " << g_motion_status_map.at(data.motion_status)
            << std::endl;

  // 速度信息
  std::cout << "[Speed]" << std::endl;
  std::cout << "  Speed Level: " << g_speed_level_map.at(data.speed_level)
            << std::endl;
  std::cout << "  Linear: " << data.speed.line << " m/s" << std::endl;
  std::cout << "  Translation: " << data.speed.translation << " m/s"
            << std::endl;
  std::cout << "  Angular: " << data.speed.angle << " rad/s" << std::endl;

  // 灯光状态
  std::cout << "[Lights]" << std::endl;
  std::cout << "  Front: "
            << (data.front_fill_light == FillLightStatus::FILL_LIGHT_STATUS_ON
                    ? "ON"
                    : "OFF")
            << ", Back: "
            << (data.back_fill_light == FillLightStatus::FILL_LIGHT_STATUS_ON
                    ? "ON"
                    : "OFF")
            << ", Auto: " << (data.auto_mode_light ? "ON" : "OFF") << std::endl;

  // 急停状态
  std::cout << "[Emergency]" << std::endl;
  std::cout << "  Software: "
            << (data.software_emergency_status ==
                        EmergencyStatus::EMERGENCY_STATUS_STOP
                    ? "ACTIVE"
                    : "INACTIVE")
            << ", Hardware: "
            << (data.hardware_emergency_status ==
                        EmergencyStatus::EMERGENCY_STATUS_STOP
                    ? "ACTIVE"
                    : "INACTIVE")
            << std::endl;

  // 电池信息
  std::cout << "[Battery]" << std::endl;
  std::cout << "  Battery 1: " << static_cast<int>(data.battery.power1) << "%"
            << ", " << data.battery.voltage1 << "V"
            << ", " << data.battery.current1 << "A"
            << ", " << data.battery.temperature1 << "°C"
            << ", Status: "
            << static_cast<int>(data.battery.power_supply_status1) << std::endl;
  std::cout << "  Battery 2: " << static_cast<int>(data.battery.power2) << "%"
            << ", " << data.battery.voltage2 << "V"
            << ", " << data.battery.current2 << "A"
            << ", " << data.battery.temperature2 << "°C"
            << ", Status: "
            << static_cast<int>(data.battery.power_supply_status2) << std::endl;

  // 里程信息
  std::cout << "[Odometry]" << std::endl;
  std::cout << "  Total Distance: " << data.mile_data << " m" << std::endl;

  std::cout << "[ControlSource:]" << std::endl;
  std::cout << "  Control Source: "
            << (data.control_source == CtrlSource::CTRL_SOURCE_APP   ? "APP"
                : data.control_source == CtrlSource::CTRL_SOURCE_SDK ? "SDK"
                : data.control_source == CtrlSource::CTRL_SOURCE_OTHER
                    ? "OTHER"
                    : "UNKNOWN")
            << std::endl;

  std::cout << "================================\n" << std::endl;
}

// 命令处理函数类型
using CommandHandler = std::function<void(SDKClient&)>;

/**
 * @brief 打印控制帮助信息
 */
void PrintHelp() {
  std::cout << "\n========== Control Commands ==========" << std::endl;
  std::cout << "[Mode]     1:General  2:InPlace  3:Stair" << std::endl;
  std::cout << "[Speed]    4:Low  5:Medium  6:High" << std::endl;
  std::cout << "[Move]     W:Forward  S:Backward  A:Left  D:Right" << std::endl;
  std::cout << "[Turn]     L:Left  R:Right" << std::endl;
  std::cout << "[Roll]     7:Left  8:Right" << std::endl;
  std::cout << "[Head]     9:Look Left  0:Look Up" << std::endl;
  std::cout << "[Pose]     Z:Stand  X:Crawl  C:Lie  G:Gait  J:Climb  K:Slim "
            << std::endl;
  std::cout << "[Light]    F:Front  B:Back  N:Auto" << std::endl;
  std::cout << "[System]   E:E-Stop  M:Lock  V:Reverse Head/Tail" << std::endl;
  std::cout << "[Control]  T:TakeControl Y:ReleaseControl Space:Stop  O:Status "
               " H:Help  Q:Quit"
            << std::endl;
  std::cout << "======================================\n" << std::endl;
}

/**
 * @brief 创建键盘命令处理映射表
 * @param sdk_client SDK客户端引用
 * @return 命令映射表
 */
std::map<char, CommandHandler> CreateCommandTable(SDKClient& sdk_client) {
  return {
      // ============ 模式选择 ============
      {'1', [](SDKClient& client) { client.SetMode(1); }},  // 通用模式
      {'2', [](SDKClient& client) { client.SetMode(2); }},  // 原地模式
      {'3', [](SDKClient& client) { client.SetMode(3); }},  // 登阶模式

      // ============ 速度控制 ============
      {'4', [](SDKClient& client) { client.SetSpeed(1); }},  // 低速
      {'5', [](SDKClient& client) { client.SetSpeed(2); }},  // 中速
      {'6', [](SDKClient& client) { client.SetSpeed(3); }},  // 高速

      // ============ 原地操作 ============
      {'7', [](SDKClient& client) { client.Turn(1); }},                // 左翻滚
      {'8', [](SDKClient& client) { client.Turn(2); }},                // 右翻滚
      {'9', [](SDKClient& client) { client.ControlHead(0.5, 0.0); }},  // 左探头
      {'0', [](SDKClient& client) { client.ControlHead(0.0, 0.5); }},  // 抬头

      // ============ 方向移动 ============
      {'w', [](SDKClient& client) { client.Move(0.0, 0.11, 0.0); }},   // 向前
      {'a', [](SDKClient& client) { client.Move(0.1, 0.0, 0.0); }},    // 向左
      {'s', [](SDKClient& client) { client.Move(0.0, -0.11, 0.0); }},  // 向后
      {'d', [](SDKClient& client) { client.Move(-0.1, 0.0, 0.0); }},   // 向右

      // ============ 转向控制 ============
      {'l', [](SDKClient& client) { client.Move(0.0, 0.0, 0.1); }},   // 左转
      {'r', [](SDKClient& client) { client.Move(0.0, 0.0, -0.1); }},  // 右转

      // ============ 姿态控制 ============
      {'z', [](SDKClient& client) { client.StandUp(); }},          // 站立
      {'x', [](SDKClient& client) { client.Crawl(); }},            // 匍匐
      {'c', [](SDKClient& client) { client.LieDown(); }},          // 卧倒
      {'v', [](SDKClient& client) { client.ReverseHeadTail(); }},  // 头部切换

      // ============ 运动模式下动作 ============
      {'g', [](SDKClient& client) { client.Gait(); }},   // 步态
      {'j', [](SDKClient& client) { client.Climb(); }},  // 爬高台
      {'k', [](SDKClient& client) { client.Slim(); }},   // 瘦身

      // ============ 控制权管理 ============
      {'t', [](SDKClient& client) { client.TakeControl(); }},     // 获取控制权
      {'y', [](SDKClient& client) { client.ReleaseControl(); }},  // 释放控制权

      // ============ 灯光控制 ============
      {'f',
       [](SDKClient& client) {  // 前补光灯
         bool new_state = !g_front_light_on.load();
         g_front_light_on = new_state;
         client.FrontLight(new_state);
       }},
      {'b',
       [](SDKClient& client) {  // 后补光灯
         bool new_state = !g_back_light_on.load();
         g_back_light_on = new_state;
         client.BackLight(new_state);
       }},
      {'n',
       [](SDKClient& client) {  // 自动模式灯光
         bool new_state = !g_auto_mode_light_on.load();
         g_auto_mode_light_on = new_state;
         client.AutoModeLight(new_state);
       }},

      // ============ 系统控制 ============
      {'e',
       [](SDKClient& client) {  // 急停开关
         bool new_state = !g_estop_on.load();
         g_estop_on = new_state;
         client.SoftEmergencyStop(new_state);
       }},
      {'m', [](SDKClient& client) { client.Locked(); }},  // 锁定

      // ============ 停止与状态 ============
      {' ',
       [](SDKClient& client) {  // 停止所有运动
         client.Move(0.0, 0.0, 0.0);
         client.Turn(0);
         client.ControlHead(0.0, 0.0);
         std::cout << "[CMD] Stop all motion" << std::endl;
       }},
      {'p',
       [](SDKClient& client) {  // 停止所有运动
         client.Move(0.0, 0.0, 0.0);
         client.Turn(0);
         client.ControlHead(0.0, 0.0);
         std::cout << "[CMD] Stop all motion" << std::endl;
       }},
      {'o',
       [](SDKClient& client) {  // 输出状态
         std::lock_guard<std::mutex> lock(g_state_mtx);
         PrintRobotState(g_robot_state);
       }},
      {'h', [](SDKClient& client) { PrintHelp(); }},  // 帮助信息

      // ============ 其他 ============
      {'\n', [](SDKClient& client) {}},  // 回车空操作
  };
}

int main(int argc, char* argv[]) {
  // 安装信号处理器
  std::signal(SIGINT, SignalHandler);
  std::signal(SIGTERM, SignalHandler);

  // 解析命令行参数
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " <ip> <port>" << std::endl;
    std::cerr << "Example: " << argv[0] << " 192.168.234.1 8081" << std::endl;
    return EXIT_FAILURE;
  }

  const std::string ip = argv[1];
  const std::string port = argv[2];

  std::cout << "========================================" << std::endl;
  std::cout << "  Robot SDK Interactive Control Demo" << std::endl;
  std::cout << "========================================" << std::endl;
  std::cout << "Target: " << ip << ":" << port << std::endl;
  std::cout << "========================================\n" << std::endl;

  // 初始化 SDK 客户端
  SDKClient sdk_client;

  std::cout << "[INFO] SDK Version: " << sdk_client.Version() << std::endl;
  std::cout << "[INFO] Protocol Version: " << sdk_client.ProtocolVersion()
            << "\n"
            << std::endl;

  // 设置回调
  auto data_cb = std::make_shared<DataCallback>();
  auto ctrl_cb = std::make_shared<ControlCallback>();
  sdk_client.SetDataCallback(data_cb);
  sdk_client.SetControlCallback(ctrl_cb);

  // 异步连接机器人
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

    // 等待连接完成
    auto status = g_connect_cv.wait_for(lock, std::chrono::seconds(10));
    if (status == std::cv_status::timeout) {
      std::cerr << "[ERROR] Connection timeout" << std::endl;
      return EXIT_FAILURE;
    }
  }

  // 检查连接状态
  if (sdk_client.GetConnectionState() != ConnectionState::CONNECTED) {
    std::cerr << "[ERROR] Not connected. State: "
              << static_cast<int>(sdk_client.GetConnectionState()) << std::endl;
    return EXIT_FAILURE;
  }

  // 获取系统版本
  std::cout << "[INFO] System Version: " << sdk_client.SystemVersion() << "\n"
            << std::endl;

  // 打印控制帮助
  PrintHelp();

  // 创建命令映射表
  auto command_table = CreateCommandTable(sdk_client);

  // 主控制循环
  std::cout << "[READY] Waiting for commands...\n" << std::endl;
  while (g_running) {
    char c = std::getchar();

    // 退出命令
    if (c == 'q' || c == 'Q') {
      std::cout << "[INFO] Quit command received" << std::endl;
      break;
    }

    // 查找并执行命令
    auto it = command_table.find(c);
    if (it != command_table.end()) {
      try {
        it->second(sdk_client);
      } catch (const std::exception& e) {
        std::cerr << "[ERROR] Command execution failed: " << e.what()
                  << std::endl;
      }
    } else if (c != '\n') {
      // 未识别的命令（忽略换行符）
      std::cout << "[WARN] Unknown command: '" << c << "' (Press H for help)"
                << std::endl;
    }
  }

  // 清理退出
  std::cout << "\n[SHUTDOWN] Disconnecting..." << std::endl;
  {
    std::unique_lock<std::mutex> lock(g_connect_mtx);
    sdk_client.Disconnect(false, [](const std::error_code& ec) {
      std::unique_lock<std::mutex> lock(g_connect_mtx);
      if (ec) {
        std::cerr << "[ERROR] Disconnect failed: " << ec.message() << std::endl;
      } else {
        std::cout << "[SHUTDOWN] ✓ Disconnected" << std::endl;
      }
      g_connect_cv.notify_one();
    });

    auto status = g_connect_cv.wait_for(lock, std::chrono::seconds(3));
    if (status == std::cv_status::timeout) {
      std::cerr << "[WARN] Disconnect timeout" << std::endl;
    }
  }

  std::cout << "[SHUTDOWN] ✓ Exited cleanly" << std::endl;
  return EXIT_SUCCESS;
}
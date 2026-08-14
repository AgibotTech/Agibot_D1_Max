/**
 * @file control.cpp
 * @brief Robot SDK Interactive Control Example
 *
 * This example demonstrates complete interactive robot control functionality:
 * - Real-time robot motion control via keyboard input
 * - Mode switching, speed adjustment, posture control, light control commands,
 * etc.
 * - Asynchronous connection management
 *
 * Applicable scenarios:
 * - Remote robot teleoperation
 * - Feature testing and debugging
 * - SDK API feature demonstration
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

// Global control flags
static std::atomic<bool> g_running{true};
static std::atomic<bool> g_front_light_on{false};
static std::atomic<bool> g_back_light_on{false};
static std::atomic<bool> g_auto_mode_light_on{false};
static std::atomic<bool> g_estop_on{false};

// Synchronization primitives
static std::mutex g_connect_mtx;
static std::condition_variable g_connect_cv;

// Robot state cache
static std::mutex g_state_mtx;
static RobotState g_robot_state;

// State string mappings
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

// Signal handler function
void SignalHandler(int signal) {
  std::cout << "\n[INFO] Received signal " << signal << ", shutting down..."
            << std::endl;
  g_running = false;
}

/**
 * @brief Data callback class - receive robot sensor data
 *
 * @note Callback functions must be lightweight and cannot perform
 * time-consuming operations
 * - Only perform fast operations such as data copying and validation
 * - Time-consuming operations (database writes, file I/O, network transmission,
 * etc.) must be handled in separate threads
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
 * @brief Control Callback Class - Receives robot confirmation for control
 * commands
 *
 * After the SDK sends control commands, the robot confirms receipt through this
 * callback
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

  void OnDSB() override { std::cout << "[CTRL] ✓ DSB" << std::endl; }

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
 * @brief Print robot state information
 */
void PrintRobotState(const RobotState& data) {
  std::cout << "\n========== Robot State ==========" << std::endl;

  // Posture information
  std::cout << "[Pose]" << std::endl;
  std::cout << "  Head Angle: " << std::fixed << std::setprecision(2)
            << data.head_angle << "°" << std::endl;
  std::cout << "  Head Direction: " << static_cast<int>(data.head_direction)
            << std::endl;
  std::cout << "  Sport Mode: " << g_sport_mode_map.at(data.sport_mode)
            << std::endl;
  std::cout << "  Motion Status: " << g_motion_status_map.at(data.motion_status)
            << std::endl;

  // Speed information
  std::cout << "[Speed]" << std::endl;
  std::cout << "  Speed Level: " << g_speed_level_map.at(data.speed_level)
            << std::endl;
  std::cout << "  Linear: " << data.speed.line << " m/s" << std::endl;
  std::cout << "  Translation: " << data.speed.translation << " m/s"
            << std::endl;
  std::cout << "  Angular: " << data.speed.angle << " rad/s" << std::endl;

  // Light status
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

  // Emergency stop status
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

  // Battery information
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

  // Odometry information
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

  std::cout << "[Joint Temperature]" << std::endl;
  std::cout << "  Joint Temperatures: " << std::endl;
  for (const auto& [joint, temp] : data.joint_temps) {
    std::cout << "    " << joint << ": " << temp << "°C" << std::endl;
  }

  std::cout << "================================\n" << std::endl;
}

// Command handler function type
using CommandHandler = std::function<void(SDKClient&)>;

/**
 * @brief Print control help information
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
 * @brief Create keyboard command handler mapping table
 * @param sdk_client SDK client reference
 * @return Command mapping table
 */
std::map<char, CommandHandler> CreateCommandTable(SDKClient& sdk_client) {
  return {
      // ============ Mode Selection ============
      {'1', [](SDKClient& client) { client.SetMode(1); }},  // General mode
      {'2', [](SDKClient& client) { client.SetMode(2); }},  // In-place mode
      {'3', [](SDKClient& client) { client.SetMode(3); }},  // Stair mode

      // ============ Speed Control ============
      {'4', [](SDKClient& client) { client.SetSpeed(1); }},  // Low
      {'5', [](SDKClient& client) { client.SetSpeed(2); }},  // Medium
      {'6', [](SDKClient& client) { client.SetSpeed(3); }},  // High

      // ============ In-place Operations ============
      {'7', [](SDKClient& client) { client.Turn(1); }},  // Left roll
      {'8', [](SDKClient& client) { client.Turn(2); }},  // Right roll
      {'9',
       [](SDKClient& client) { client.ControlHead(0.5, 0.0); }},  // Left peek
      {'0',
       [](SDKClient& client) { client.ControlHead(0.0, 0.5); }},  // Look up

      // ============ Directional Movement ============
      {'w', [](SDKClient& client) { client.Move(0.0, 0.11, 0.0); }},  // Forward
      {'a', [](SDKClient& client) { client.Move(0.1, 0.0, 0.0); }},   // Left
      {'s',
       [](SDKClient& client) { client.Move(0.0, -0.11, 0.0); }},  // Backward
      {'d', [](SDKClient& client) { client.Move(-0.1, 0.0, 0.0); }},  // Right

      // ============ Turn Control ============
      {'l',
       [](SDKClient& client) { client.Move(0.0, 0.0, 0.1); }},  // Left turn
      {'r',
       [](SDKClient& client) { client.Move(0.0, 0.0, -0.1); }},  // Right turn

      // ============ Posture Control ============
      {'z', [](SDKClient& client) { client.StandUp(); }},  // Stand
      {'x', [](SDKClient& client) { client.Crawl(); }},    // Crawl
      {'c', [](SDKClient& client) { client.LieDown(); }},  // Lie down
      {'v',
       [](SDKClient& client) { client.ReverseHeadTail(); }},  // Toggle head

      // ============ Movement Mode Actions ============
      {'g', [](SDKClient& client) { client.Gait(); }},   // Gait
      {'j', [](SDKClient& client) { client.Climb(); }},  // Climb
      {'k', [](SDKClient& client) { client.Slim(); }},   // Slim

      // ============ Control Authority Management ============
      {'t', [](SDKClient& client) { client.TakeControl(); }},  // Take control
      {'y',
       [](SDKClient& client) { client.ReleaseControl(); }},  // Release control

      // ============ Light Control ============
      {'f',
       [](SDKClient& client) {  // Front fill light
         bool new_state = !g_front_light_on.load();
         g_front_light_on = new_state;
         auto err = client.FrontLight(
             new_state, 0, [](const std::error_code& ec, std::size_t) {
               if (ec) {
                 std::cerr << "[ERROR] FrontLight command failed: "
                           << ec.message() << std::endl;
               }
             });

       }},
      {'b',
       [](SDKClient& client) {  // Back fill light
         bool new_state = !g_back_light_on.load();
         g_back_light_on = new_state;
         auto err = client.BackLight(
             new_state, 0, [](const std::error_code& ec, std::size_t) {
               if (ec) {
                 std::cerr << "[ERROR] BackLight command failed: "
                           << ec.message() << std::endl;
               }
             });
       }},
      {'n',
       [](SDKClient& client) {  // Auto mode light
         bool new_state = !g_auto_mode_light_on.load();
         g_auto_mode_light_on = new_state;
         client.AutoModeLight(new_state);
       }},

      // ============ System Control ============
      {'e',
       [](SDKClient& client) {  // Emergency stop
         bool new_state = !g_estop_on.load();
         g_estop_on = new_state;
         client.SoftEmergencyStop(new_state);
       }},
      {'m', [](SDKClient& client) { client.Locked(); }},  // Lock

      // ============ Stop and Status ============
      {' ',
       [](SDKClient& client) {  // Stop all motion
         client.Move(0.0, 0.0, 0.0);
         client.Turn(0);
         client.ControlHead(0.0, 0.0);
         std::cout << "[CMD] Stop all motion" << std::endl;
       }},
      {'p',
       [](SDKClient& client) {  // Stop all motion
         client.Move(0.0, 0.0, 0.0);
         client.Turn(0);
         client.ControlHead(0.0, 0.0);
         std::cout << "[CMD] Stop all motion" << std::endl;
       }},
      {'o',
       [](SDKClient& client) {  // Print status
         std::lock_guard<std::mutex> lock(g_state_mtx);
         PrintRobotState(g_robot_state);
       }},
      {'h', [](SDKClient& client) { PrintHelp(); }},  // Help

      // ============ Others ============
      {'\n', [](SDKClient& client) {}},  // Enter (no-op
  };
}

int main(int argc, char* argv[]) {
  // Install signal handlers
  std::signal(SIGINT, SignalHandler);
  std::signal(SIGTERM, SignalHandler);

  // Parse command line arguments
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " <ip> <port>" << std::endl;
    std::cerr << "Example: " << argv[0] << " 192.168.234.1 8082" << std::endl;
    return EXIT_FAILURE;
  }

  const std::string ip = argv[1];
  const std::string port = argv[2];

  std::cout << "========================================" << std::endl;
  std::cout << "  Robot SDK Interactive Control Demo" << std::endl;
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
  auto data_cb = std::make_shared<DataCallback>();
  auto ctrl_cb = std::make_shared<ControlCallback>();
  sdk_client.SetDataCallback(data_cb);
  sdk_client.SetControlCallback(ctrl_cb);

  // Asynchronous connection to robot
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
    std::cerr << "[ERROR] Not connected. State: "
              << static_cast<int>(sdk_client.GetConnectionState()) << std::endl;
    return EXIT_FAILURE;
  }

  // Get system version
  std::cout << "[INFO] System Version: " << sdk_client.SystemVersion() << "\n"
            << std::endl;

  // Print control help
  PrintHelp();

  // Create command mapping table
  auto command_table = CreateCommandTable(sdk_client);

  // Main control loop
  std::cout << "[READY] Waiting for commands...\n" << std::endl;
  while (g_running) {
    char c = std::getchar();

    // Quit command
    if (c == 'q' || c == 'Q') {
      std::cout << "[INFO] Quit command received" << std::endl;
      break;
    }

    // Find and execute command
    auto it = command_table.find(c);
    if (it != command_table.end()) {
      try {
        it->second(sdk_client);
      } catch (const std::exception& e) {
        std::cerr << "[ERROR] Command execution failed: " << e.what()
                  << std::endl;
      }
    } else if (c != '\n') {
      // Unrecognized command (ignore newline)
      std::cout << "[WARN] Unknown command: '" << c << "' (Press H for help)"
                << std::endl;
    }
  }

  // Clean up and exit
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
/**
 * @file example_data.cpp
 * @brief Robot SDK Sensor Data Subscription Example
 *
 * This example demonstrates how to subscribe to and receive robot sensor data:
 * - IMU data (acceleration, gyroscope, quaternion)
 * - Light intensity sensor data
 * - Motion control data (velocity, position, posture)
 * - Robot state data (battery, lights, mode, etc.)
 * - Fault information
 *
 * Applicable scenarios:
 * - Sensor data collection and monitoring
 * - Robot state monitoring
 * - Data recording and analysis
 */

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <csignal>
#include <iomanip>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>

#include "robot_sdk/sdk_client.hpp"

using namespace robot_sdk;

// Global control flags
static std::atomic<bool> g_running{true};
static std::mutex g_connect_mtx;
static std::condition_variable g_connect_cv;

// Signal handler function
void SignalHandler(int signal) {
  std::cout << "\n[INFO] Received signal " << signal << ", shutting down..."
            << std::endl;
  g_running = false;
}
static int count = 0;
/**
 * @brief Data callback class - receive and display sensor data
 *
 * @note Callback functions must be lightweight and cannot perform
 * time-consuming operations
 * - Only perform fast operations such as data copying and validation
 * - Time-consuming operations (database writes, file I/O, network transmission,
 * etc.) must be handled in separate threads
 */
class DataCallback : public IDataCallback {
 public:
  void OnImuData(const ImuData& data) override {
    std::cout << "\n[IMU] Inertial Measurement Unit Data:" << std::endl;
    std::cout << "  Acceleration: X=" << std::fixed << std::setprecision(3)
              << data.acc_x << " Y=" << data.acc_y << " Z=" << data.acc_z
              << " m/s²" << std::endl;
    std::cout << "  Gyroscope:    X=" << data.gyro_x << " Y=" << data.gyro_y
              << " Z=" << data.gyro_z << " rad/s" << std::endl;
    std::cout << "  Quaternion:   X=" << data.quat_x << " Y=" << data.quat_y
              << " Z=" << data.quat_z << " W=" << data.quat_w << std::endl;
  }

  void OnLuxData(const LuxData& data) override {
    std::cout << "\n[LUX] Light Intensity: " << data.lux << " lux" << std::endl;
  }

  void OnMcData(const MotionData& data) override {
    std::cout << "\n[MOTION] Motion Control Data:" << std::endl;
    std::cout << "  World Velocity: X=" << std::fixed << std::setprecision(3)
              << data.v_world[0] << " Y=" << data.v_world[1]
              << " Z=" << data.v_world[2] << " m/s" << std::endl;
    std::cout << "  Position:       X=" << data.position[0]
              << " Y=" << data.position[1] << " Z=" << data.position[2] << " m"
              << std::endl;
    std::cout << "  World Omega:    X=" << data.omega_world[0]
              << " Y=" << data.omega_world[1] << " Z=" << data.omega_world[2]
              << " rad/s" << std::endl;
    std::cout << "  Body Velocity:  X=" << data.v_body[0]
              << " Y=" << data.v_body[1] << " Z=" << data.v_body[2] << " m/s"
              << std::endl;
    std::cout << "  Body Omega:     X=" << data.omega_body[0]
              << " Y=" << data.omega_body[1] << " Z=" << data.omega_body[2]
              << " rad/s" << std::endl;
    std::cout << "  Timestamp:      " << data.time_stamp << " ns" << std::endl;
  }

  void OnRobotStateData(const RobotState& data) override {
    std::cout << "\n[STATE] Robot State Data:" << std::endl;
    std::cout << "  Head Angle: " << data.head_angle
              << "°, Direction: " << static_cast<int>(data.head_direction)
              << std::endl;
    std::cout << "  Lights: Front=" << static_cast<int>(data.front_fill_light)
              << " Back=" << static_cast<int>(data.back_fill_light)
              << std::endl;
    std::cout << "  Speed Level: " << static_cast<int>(data.speed_level)
              << ", E-Stop: SW="
              << static_cast<int>(data.software_emergency_status)
              << " HW=" << static_cast<int>(data.hardware_emergency_status)
              << std::endl;
    std::cout << "  Motion: Status=" << static_cast<int>(data.motion_status)
              << " Mode=" << static_cast<int>(data.sport_mode) << std::endl;
    std::cout << "  Battery: 1=" << static_cast<int>(data.battery.power1) << "%"
              << " 2=" << static_cast<int>(data.battery.power2) << "%"
              << std::endl;
    std::cout << "  Speed: Linear=" << data.speed.line
              << " Trans=" << data.speed.translation
              << " Angular=" << data.speed.angle << std::endl;
    std::cout << "  Odometry: " << data.mile_data << " m" << std::endl;
    std::cout << "  Joint Temperatures: " << std::endl;
    for (const auto& [joint, temp] : data.joint_temps) {
      std::cout << "    " << joint << ": " << temp << "°C" << std::endl;
    }
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

  void OnSpeedData(const SpeedData& data) override {
    std::cout << "\n[SPEED] Speed Data:" << std::endl;
    std::cout << "  X: " << data.x << " m/s, Y: " << data.y
              << " m/s, Yaw: " << data.yaw << " rad/s" << " count:" << ++count
              << std::endl;
  }

  void OnJointStateData(const JointStateData& data) override {
    std::cout << "\n[JOINT STATE] Joint State Data:" << std::endl;
    for (size_t i = 0; i < data.names.size(); ++i) {
      std::cout << "  " << data.names[i] << ": Position=" << data.positions[i]
                << " rad, Velocity=" << data.velocities[i]
                << " rad/s, Effort=" << data.efforts[i] << " Nm" << std::endl;
    }
  }
};

/**
 * @brief Control Callback Class - Receives confirmation for sensor
 * configuration commands
 */
class ControlCallback : public IControlCallback {
 public:
  void OnLuxConfig(bool on) override {
    std::cout << "[CTRL] ✓ Lux sensor config: " << (on ? "ON" : "OFF")
              << std::endl;
  }

  void OnImuConfig(int freq) override {
    std::cout << "[CTRL] ✓ IMU sensor config: "
              << (freq > 0 ? std::to_string(freq) + " Hz" : "OFF") << std::endl;
  }

  void OnMcConfig(bool on) override {
    std::cout << "[CTRL] ✓ Motion control data: " << (on ? "ON" : "OFF")
              << std::endl;
  }

  void OnSpeedReportConfig(bool on, uint32_t frequency) override {
    std::cout << "[CTRL] ✓ Speed report config: " << (on ? "ON" : "OFF")
              << ", Frequency: " << frequency << " Hz" << std::endl;
  }

  void OnJointStateConfig(bool on) override {
    std::cout << "[CTRL] ✓ Joint state data config: " << (on ? "ON" : "OFF")
              << std::endl;
  }
};

/**
 * @brief Demonstrate sensor data subscription process
 */
void DemoSensorData(SDKClient& client) {
  std::cout << "\n========== IMU Sensor Demo ==========" << std::endl;
  std::cout << "[CMD] Enabling IMU @ 100Hz..." << std::endl;
  client.SetImuConfig(100);
  std::this_thread::sleep_for(std::chrono::seconds(3));

  std::cout << "[CMD] Disabling IMU..." << std::endl;
  client.SetImuConfig(0);
  std::this_thread::sleep_for(std::chrono::seconds(1));

  std::cout << "\n========== Lux Sensor Demo ==========" << std::endl;
  std::cout << "[CMD] Enabling light sensor..." << std::endl;
  client.SetLuxConfig(true);
  std::this_thread::sleep_for(std::chrono::seconds(5));

  std::cout << "[CMD] Disabling light sensor..." << std::endl;
  client.SetLuxConfig(false);
  std::this_thread::sleep_for(std::chrono::seconds(1));

  std::cout << "\n========== Motion Data Demo ==========" << std::endl;
  std::cout << "[CMD] Enabling motion control data..." << std::endl;
  client.SetMcConfig(true);
  std::this_thread::sleep_for(std::chrono::seconds(2));

  std::cout << "[CMD] Disabling motion control data..." << std::endl;
  client.SetMcConfig(false);
  std::this_thread::sleep_for(std::chrono::seconds(1));

  std::cout << "\n========== Speed Data Demo ==========" << std::endl;
  std::cout << "[CMD] Enabling speed data @ 20Hz..." << std::endl;
  client.SetSpeedReportConfig(true, 20);
  std::this_thread::sleep_for(std::chrono::seconds(2));

  std::cout << "[CMD] Disabling speed data..." << std::endl;
  client.SetSpeedReportConfig(false, 0);
  std::this_thread::sleep_for(std::chrono::seconds(2));

  std::cout << "\n========== Joint State Data Demo ==========" << std::endl;
  std::cout << "[CMD] Enabling joint state data..." << std::endl;
  client.SetJointStateConfig(true);
  std::this_thread::sleep_for(std::chrono::seconds(2));

  std::cout << "[CMD] Disabling joint state data..." << std::endl;
  client.SetJointStateConfig(false);
  std::this_thread::sleep_for(std::chrono::seconds(3));

  std::cout << "\n[INFO] All sensor demos completed" << std::endl;
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
  std::cout << "  Robot SDK Sensor Data Demo" << std::endl;
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

  // Execute sensor data subscription demonstration
  DemoSensorData(sdk_client);

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
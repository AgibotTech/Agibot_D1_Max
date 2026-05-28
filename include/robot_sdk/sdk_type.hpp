#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
namespace robot_sdk {

enum class TransportProtocol { WebSocket, Udp };

enum class FaultCode {
  Unknown,                      // Unknown fault
  ActuatorDisabled = 10,        // Actuator disabled
  ActuatorEncoderError,         // Actuator encoder error
  ActuatorOffline,              // Actuator offline
  ActuatorOverVoltage,          // Actuator overvoltage
  ActuatorOverheat,             // Actuator overheating
  ActuatorTempWarn,             // Actuator temperature warning
  ActuatorTimeout,              // Actuator control timeout
  ActuatorUndervolt,            // Actuator undervoltage
  PowerControlOverTemp,         // Battery overheat warning
  PowerControlPowerEmpty,       // Battery below 10% capacity
  PowerControlPowerLow,         // Battery below 20%, above 10%
  PowerControlOffline,          // Power control board MCU connection failed
  CANBroken,                    // CAN communication error
  RobotRemoteKeepAliveFailure,  // Remote control disconnected
  SystemClockSanityError,       // System time jump detected
  SystemRobotStatusError,       // Robot status abnormal
  IMUConnectError,              // IMU connection error
  IMUDataNotUpdated,            // IMU data not updating
};

enum class FaultLevel {
  Unknown = 0,
  FatalError,
  Error,
  Warn,
};

/// @brief Fault data structure.
struct FaultData {
  /// @brief Fault code.
  FaultCode code;
  /// @brief Fault level.
  FaultLevel level;
  /// @brief Fault message.
  std::string message;
};
using FaultDatas = std::vector<FaultData>;

/// @brief IMU data structure.
struct ImuData {
  /// @brief Accelerometer: x, y, z axes.
  float acc_x, acc_y, acc_z;
  /// @brief Gyroscope: x, y, z axes.
  float gyro_x, gyro_y, gyro_z;
  /// @brief Quaternion: x, y, z, w components.
  float quat_x, quat_y, quat_z, quat_w;
};

enum class PowerSupplyStatus {
  /// @brief Power supply status unknown.
  UNKNOWN = 0,
  /// @brief Charging.
  CHARGING = 1,
  /// @brief Discharging.
  DISCHARGING = 2,
  /// @brief Fully charged.
  FULL = 4,
};

struct BatteryData {
  /// @brief Battery 1 charge capacity.
  float power1;
  /// @brief Battery 2 charge capacity.
  float power2;
  /// @brief Battery 1 present.
  bool present1;
  /// @brief Battery 2 present.
  bool present2;
  /// @brief Battery 1 voltage (V).
  float voltage1;
  /// @brief Battery 2 voltage (V).
  float voltage2;
  /// @brief Battery 1 temperature.
  float temperature1;
  /// @brief Battery 2 temperature.
  float temperature2;
  /// @brief Battery 1 current (A).
  float current1;
  /// @brief Battery 2 current (A).
  float current2;
  /// @brief Battery 1 power supply status.
  PowerSupplyStatus power_supply_status1;
  /// @brief Battery 2 power supply status.
  PowerSupplyStatus power_supply_status2;
};

/// @brief Light intensity (illuminance) data.
struct LuxData {
  /// @brief Light intensity value (lux).
  float lux;
};

/// @brief Fill light status.
enum class FillLightStatus {
  /// @brief Unknown status.
  FILL_LIGHT_STATUS_UNKNOWN = 0,
  /// @brief Fill light on.
  FILL_LIGHT_STATUS_ON,
  /// @brief Fill light off.
  FILL_LIGHT_STATUS_OFF,
};

/// @brief Speed level.
enum class SpeedLevel {
  /// @brief Unknown speed level.
  SPEED_LEVEL_UNKNOWN = 0,
  /// @brief Low speed.
  SPEED_LEVEL_SLOW,
  /// @brief Medium speed.
  SPEED_LEVEL_MEDIUM,
  /// @brief High speed.
  SPEED_LEVEL_HIGH,
};

/// @brief Emergency stop status.
enum class EmergencyStatus {
  /// @brief Unknown status.
  EMERGENCY_STATUS_UNKNOWN = 0,
  /// @brief Emergency stop released.
  EMERGENCY_STATUS_RECOVER,
  /// @brief Emergency stop activated.
  EMERGENCY_STATUS_STOP,
};

/// @brief Robot head direction.
enum class HeadDirection {
  /// @brief Unknown direction.
  HEAD_DIRECTION_UNKNOWN = 0,
  /// @brief Head is at the head position.
  HEAD_DIRECTION_HEAD,
  /// @brief Tail is at the head position.
  HEAD_DIRECTION_TAIL,
};

/// @brief Motion mode.
enum class SportMode {
  /// @brief Unknown mode.
  SPORT_MODE_UNKNOWN = 0,
  /// @brief General mode.
  SPORT_MODE_GENERAL,
  /// @brief In-place mode.
  SPORT_MODE_IN_PLACE,
  /// @brief Stair-climbing mode.
  SPORT_MODE_STAIR,
};

/// @brief Motion state.
enum class MotionStatus {
  /// @brief Unknown state.
  MOTION_STATUS_UNKNOWN = 0,
  /// @brief Standing.
  MOTION_STATUS_STAND_UP,
  /// @brief Lying down.
  MOTION_STATUS_LIE_DOWN,
  /// @brief Crawling.
  MOTION_STATUS_CRAWL,
  /// @brief Locked.
  MOTION_STATUS_LOCKED,
  /// @brief General mode state.
  MOTION_STATUS_GENERAL,
  /// @brief In-place mode state.
  MOTION_STATUS_IN_PLACE,
  /// @brief Stair-climbing mode state.
  MOTION_STATUS_STAIR,
  /// @brief Climbing high platform state.
  MOTION_STATUS_CLIMB,
  /// @brief Slim (body compress) state.
  MOTION_STATUS_SLIM,
  /// @brief Gait state.
  MOTION_STATUS_GAIT,
};

enum class CtrlSource {
  CTRL_SOURCE_UNKNOWN = 0,
  CTRL_SOURCE_APP = 1,
  CTRL_SOURCE_SDK = 2,
  CTRL_SOURCE_OTHER = 3,
};

/// @brief Velocity information.
struct Speed {
  /// @brief Forward/backward velocity.
  double line;
  /// @brief Left/right lateral velocity.
  double translation;
  /// @brief Rotation velocity (angular velocity).
  double angle;
};

/// @brief Robot state information.
struct RobotState {
  /// @brief Head angle.
  double head_angle;

  /// @brief Front fill light status.
  FillLightStatus front_fill_light;

  /// @brief Back fill light status.
  FillLightStatus back_fill_light;

  /// @brief Auto mode light enabled.
  bool auto_mode_light;

  /// @brief Current speed level.
  SpeedLevel speed_level;

  /// @brief Software emergency stop status.
  EmergencyStatus software_emergency_status;

  /// @brief Hardware emergency stop status.
  EmergencyStatus hardware_emergency_status;

  /// @brief Current head direction.
  HeadDirection head_direction;

  /// @brief Motion state.
  MotionStatus motion_status;

  /// @brief Battery data.
  BatteryData battery;

  /// @brief Current velocity.
  Speed speed;

  /// @brief Cumulative mileage data.
  float mile_data;

  /// @brief Joint temperature map (joint name to temperature).
  std::unordered_map<std::string, double> joint_temps;

  /// @brief Motion mode.
  SportMode sport_mode;

  /// @brief Control source.
  CtrlSource control_source;
};

/// @brief Motion control data information.
struct MotionData {
  /// ---Motion state information---

  /// @brief Quaternion [w, x, y, z].
  float quat[4];

  /// @brief Velocity in world coordinate system [x, y, z] (m/s).
  float v_world[3];

  /// @brief Position in world coordinate system [x, y, z] (m).
  float position[3];

  /// @brief Angular velocity in world coordinate system [x, y, z] (rad/s).
  float omega_world[3];

  /// @brief Velocity in body coordinate system [x, y, z] (m/s).
  float v_body[3];

  /// @brief Angular velocity in body coordinate system [x, y, z] (rad/s).
  float omega_body[3];

  /// @brief Timestamp of the motion data. Unit: nanoseconds (ns).
  uint64_t time_stamp;
};

/// @brief Speed report data information.
struct SpeedData {
  /// @brief Forward/backward velocity (m/s)
  float x;

  /// @brief Left/right lateral velocity (m/s)
  float y;

  /// @brief Rotation velocity (angular velocity) (rad/s)
  float yaw;
};

/// @brief Joint state data information.
struct JointStateData {
  /// @brief Joint names.
  std::vector<std::string> names;
  /// @brief Joint positions.
  std::vector<double> positions;
  /// @brief Joint velocities.
  std::vector<double> velocities;
  /// @brief Joint efforts.
  std::vector<double> efforts;
};

struct ControlLostInfo {};

struct ControlAvailableInfo {};

/// @brief Take control command acknowledgment information.
struct TakeControlAck {
  /// @brief Success status: 0 = success, non-zero = failure.
  uint32_t error_code;
  /// @brief Failure reason description.
  std::string reason;
};

/// @brief Release control command acknowledgment information.
struct ReleaseControlAck {
  /// @brief Success status: 0 = success, non-zero = failure.
  uint32_t error_code;
  /// @brief Failure reason description.
  std::string reason;
};

/// @brief Camera bitrate configuration command.
struct CameraBitrateCmd {
  /// @brief Camera name: "camera_front" for front camera, "camera_back" for
  /// back camera.
  std::string camera_name;
  /// @brief Camera bitrate in bps (bit/s). Range: 50000-100000000.
  uint32_t camera_bps;
};

/// @brief Camera bitrate configuration acknowledgment information.
struct CameraBitrateAck {
  /// @brief Camera name: "camera_front" for front camera, "camera_back" for
  /// back camera.
  std::string camera_name;
  /// @brief Camera bitrate in bps (bit/s). Range: 50000-100000000.
  uint32_t camera_bps;
};

}  // namespace robot_sdk
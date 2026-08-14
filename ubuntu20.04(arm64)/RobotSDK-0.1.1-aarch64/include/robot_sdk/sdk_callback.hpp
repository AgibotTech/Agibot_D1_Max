#pragma once

#include "robot_sdk/sdk_export.hpp"
#include "robot_sdk/sdk_type.hpp"
namespace robot_sdk {

/// @brief User implementation is required as needed.
/// Callback functions must be lightweight and cannot perform time-consuming
/// operations.
/// --Within the callback function, only quick operations such as data copying
/// and data validation should be performed.
/// --For time-consuming operations such as database writes, file I/O, complex
/// calculations, and network transmission, users need to copy the data to
/// another thread (e.g., user's own thread pool) within the callback function
/// for processing.
class ROBOT_EXPORT_API IDataCallback {
 public:
  /// @brief IMU data callback (reported at fixed frequency after
  /// configuration).
  /// @param data IMU data.
  virtual void OnImuData(const ImuData& data) {}

  /// @brief Lux (illuminance) data callback (reported at fixed 1Hz frequency
  /// after configuration).
  /// @param data Lux data.
  virtual void OnLuxData(const LuxData& data) {}

  /// @brief Motion control data callback (reported at fixed 50Hz frequency
  /// after configuration).
  /// @param data Motion data.
  virtual void OnMcData(const MotionData& data) {}

  /// @brief Speed data callback (reported at fixed frequency after
  /// configuration).
  /// @param data
  virtual void OnSpeedData(const SpeedData& data) {}

  /// @brief  Joint state data callback (reported at fixed frequency after
  /// configuration).
  /// @param data
  virtual void OnJointStateData(const JointStateData& data) {}

  /// @brief Robot state data callback (actively reported at 1Hz frequency).
  /// @param data Robot state data.
  virtual void OnRobotStateData(const RobotState& data) {}

  /// @brief Fault information data callback (actively reported when a fault
  /// occurs).
  /// @param data Fault data.
  virtual void OnFaultData(const FaultDatas& data) {}

  /// @brief Control ownership lost callback.
  virtual void OnControlLost(const ControlLostInfo& info) {}

  /// @brief Control ownership available callback.
  virtual void OnControlAvailable(const ControlAvailableInfo& info) {}

  virtual ~IDataCallback() = default;
};

/// @brief User implementation is required as needed.
///        Used in non-blocking mode: indicates that the Robot has received the
///        control command.
class ROBOT_EXPORT_API IControlCallback {
 public:
  /// @brief Acknowledgment for soft emergency stop command received.
  /// @param on true: emergency stop was activated; false: emergency stop was
  /// deactivated.
  virtual void OnSoftEmergencyStop(bool on) {}

  /// @brief Acknowledgment for stand up command received.
  virtual void OnStandUp() {}

  /// @brief Acknowledgment for lie down command received.
  virtual void OnLieDown() {}

  /// @brief Acknowledgment for crawl command received.
  virtual void OnCrawl() {}

  /// @brief Acknowledgment for climb command received.
  virtual void OnClimb() {}

  /// @brief Acknowledgment for slim (body compress) command received.
  virtual void OnSlim() {}

  /// @brief Acknowledgment for gait command received.
  virtual void OnGait() {}

  /// @brief Acknowledgment for DSB command received.
  virtual void OnDSB() {}

  /// @brief Acknowledgment for reverse head-tail command received.
  virtual void OnReverseHeadTail() {}

  /// @brief Acknowledgment for mode switch command received.
  /// @param mode The mode set by the user.
  virtual void OnMode(int mode) {}

  /// @brief Acknowledgment for speed level switch command received.
  /// @param speed_level The speed level set by the user.
  virtual void OnSpeed(int speed_level) {}

  /// @brief Acknowledgment for lock command received.
  virtual void OnLocked() {}

  /// @brief Acknowledgment for front fill light command received.
  /// @param on true: enabled; false: disabled.
  virtual void OnFrontLight(bool on) {}

  /// @brief Acknowledgment for back fill light command received.
  /// @param on true: enabled; false: disabled.
  virtual void OnBackLight(bool on) {}

  /// @brief Acknowledgment for auto mode light command received.
  /// @param on true: enabled; false: disabled.
  virtual void OnAutoModeLight(bool on) {}

  /// @brief Acknowledgment for illuminance configuration command received.
  /// @param on true: enabled; false: disabled.
  virtual void OnLuxConfig(bool on) {}

  /// @brief Acknowledgment for IMU configuration command received.
  /// @param freq The frequency set by the user.
  virtual void OnImuConfig(int freq) {}

  /// @brief Acknowledgment for motion control configuration command received.
  /// @param on true: enabled; false: disabled.
  virtual void OnMcConfig(bool on) {}

  /// @brief Acknowledgment for speed report configuration command received.
  /// @param on true: enabled; false: disabled.
  /// @param frequency The frequency set by the user.
  virtual void OnSpeedReportConfig(bool on, uint32_t frequency) {}

  /// @brief Acknowledgment for joint state configuration command received.
  /// @param on true: enabled; false: disabled.
  virtual void OnJointStateConfig(bool on) {}

  /// @brief Acknowledgment for take control command received.
  /// @param ack Control acknowledgment information.
  virtual void OnTakeControlAck(const TakeControlAck& ack) {}

  /// @brief Acknowledgment for release control command received.
  /// @param ack Control acknowledgment information.
  virtual void OnReleaseControlAck(const ReleaseControlAck& ack) {}

  /// @brief Acknowledgment for camera bitrate update command received.
  /// @param ack Camera bitrate acknowledgment information.
  virtual void OnUpdateCameraBitrateAck(const CameraBitrateAck& ack) {}

  virtual ~IControlCallback() = default;
};
}  // namespace robot_sdk
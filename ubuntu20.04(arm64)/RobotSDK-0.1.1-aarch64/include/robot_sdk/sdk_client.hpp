#pragma once

#include <functional>
#include <memory>
#include <string>
#include <system_error>

#include "robot_sdk/sdk_callback.hpp"
#include "robot_sdk/sdk_connection.hpp"
#include "robot_sdk/sdk_export.hpp"
#include "robot_sdk/sdk_type.hpp"

namespace robot_sdk {

/**
 * @brief SDKClient provides connection, disconnection, and command transmission
 * interfaces.
 */
class ROBOT_EXPORT_API SDKClient {
 public:
  using ConnectHandler = std::function<void(const std::error_code&)>;
  using DisConnectHandler = std::function<void(const std::error_code&)>;
  using ErrorHandler = std::function<void(const std::error_code&)>;
  using WriteHandler = std::function<void(const std::error_code&, std::size_t)>;

  SDKClient(
      ErrorHandler error_callback = [](const std::error_code&) {},
      ConnectionConfig connection_config = ConnectionConfig(),
      TransportProtocol type = TransportProtocol::Udp);
  ~SDKClient();

  std::error_code Connect(
      std::string ip, std::string port, bool block = false,
      ConnectHandler handler = [](const std::error_code&) {});

  std::error_code Disconnect(
      bool block = false,
      DisConnectHandler handler = [](const std::error_code&) {});

  bool IsConnected() const;

  ConnectionState GetConnectionState() const;

  void SetControlCallback(std::shared_ptr<IControlCallback> control_callback);

  void SetDataCallback(std::shared_ptr<IDataCallback> data_callback);

  std::error_code SoftEmergencyStop(
      bool on, int timeout_ms = 0,
      WriteHandler handler = [](const std::error_code&, std::size_t) {});

  std::error_code StandUp(
      int timeout_ms = 0,
      WriteHandler handler = [](const std::error_code&, std::size_t) {});

  std::error_code LieDown(
      int timeout_ms = 0,
      WriteHandler handler = [](const std::error_code&, std::size_t) {});

  std::error_code Crawl(
      int timeout_ms = 0,
      WriteHandler handler = [](const std::error_code&, std::size_t) {});

  std::error_code Climb(
      int timeout_ms = 0,
      WriteHandler handler = [](const std::error_code&, std::size_t) {});

  std::error_code Gait(
      int timeout_ms = 0,
      WriteHandler handler = [](const std::error_code&, std::size_t) {});

  std::error_code Slim(
      int timeout_ms = 0,
      WriteHandler handler = [](const std::error_code&, std::size_t) {});

  std::error_code DSB(
      int timeout_ms = 0,
      WriteHandler handler = [](const std::error_code&, std::size_t) {});

  std::error_code ReverseHeadTail(
      int timeout_ms = 0,
      WriteHandler handler = [](const std::error_code&, std::size_t) {});

  std::error_code SetMode(
      int mode, int timeout_ms = 0,
      WriteHandler handler = [](const std::error_code&, std::size_t) {});

  std::error_code SetSpeed(
      int speed_level, int timeout_ms = 0,
      WriteHandler handler = [](const std::error_code&, std::size_t) {});

  std::error_code Locked(
      int timeout_ms = 0,
      WriteHandler handler = [](const std::error_code&, std::size_t) {});

  std::error_code FrontLight(
      bool on, int timeout_ms = 0,
      WriteHandler handler = [](const std::error_code&, std::size_t) {});

  std::error_code BackLight(
      bool on, int timeout_ms = 0,
      WriteHandler handler = [](const std::error_code&, std::size_t) {});

  std::error_code AutoModeLight(
      bool on, int timeout_ms = 0,
      WriteHandler handler = [](const std::error_code&, std::size_t) {});

  std::error_code Move(
      float left_right, float forward_back, float yaw, int timeout_ms = 0,
      WriteHandler handler = [](const std::error_code&, std::size_t) {});

  std::error_code Turn(
      int direction, int timeout_ms = 0,
      WriteHandler handler = [](const std::error_code&, std::size_t) {});

  std::error_code ControlHead(
      float left_right, float up_down, int timeout_ms = 0,
      WriteHandler handler = [](const std::error_code&, std::size_t) {});

  std::error_code HighLowStance(
      int stance, int timeout_ms = 0,
      WriteHandler handler = [](const std::error_code&, std::size_t) {});

  std::error_code SetImuConfig(
      int freq, int timeout_ms = 0,
      WriteHandler handler = [](const std::error_code&, std::size_t) {});

  std::error_code SetLuxConfig(
      bool on, int timeout_ms = 0,
      WriteHandler handler = [](const std::error_code&, std::size_t) {});

  std::error_code SetMcConfig(
      bool on, int timeout_ms = 0,
      WriteHandler handler = [](const std::error_code&, std::size_t) {});

  std::error_code SetSpeedReportConfig(
      bool on, uint32_t frequency, int timeout_ms = 0,
      WriteHandler handler = [](const std::error_code&, std::size_t) {});

  std::error_code SetJointStateConfig(
      bool on, int timeout_ms = 0,
      WriteHandler handler = [](const std::error_code&, std::size_t) {});

  std::error_code TakeControl(
      int timeout_ms = 0,
      WriteHandler handler = [](const std::error_code&, std::size_t) {});

  std::error_code ReleaseControl(
      int timeout_ms = 0,
      WriteHandler handler = [](const std::error_code&, std::size_t) {});

  std::error_code UpdateCameraBitrate(
      CameraBitrateCmd cmd, int timeout_ms = 0,
      WriteHandler handler = [](const std::error_code&, std::size_t) {});

  const std::string& Version() const;

  const std::string& ProtocolVersion() const;

  const std::string& SystemVersion() const;

 private:
  SDKClient(const SDKClient&) = delete;
  SDKClient& operator=(const SDKClient&) = delete;

 private:
  class Impl;
  std::shared_ptr<Impl> pImpl_;
};

}  // namespace robot_sdk
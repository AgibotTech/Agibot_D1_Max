#pragma once

#include "robot_sdk/sdk_export.hpp"
#include "robot_sdk/sdk_type.hpp"
namespace robot_sdk {

/// @brief 用户层根据需要来进行实现
/// 回调函数必须轻量级，不能执行耗时操作。
/// --在回调函数中，只应该做数据复制、数据校验等快速操作。
/// --如果需要进行数据库写入、文件I/O、复杂计算、网络发送等耗时操作，用户需要在回调函数内部将数据复制到另一个线程（例如，用户自己的工作线程池）中去处理。
class ROBOT_EXPORT_API IDataCallback {
 public:
  /// @brief IMU数据回调（配置后定频上报）
  /// @param data
  virtual void OnImuData(const ImuData& data) {}

  /// @brief Lux数据回调（配置后定频1Hz上报）
  /// @param data
  virtual void OnLuxData(const LuxData& data) {}

  /// @brief 运动数据回调（配置后定频50Hz上报）
  /// @param data
  virtual void OnMcData(const MotionData& data) {}

  /// @brief 机器状态数据回调（1Hz主动上报）
  /// @param data
  virtual void OnRobotStateData(const RobotState& data) {}

  /// @brief 故障信息数据回调（发生故障时主动上报）
  /// @param data
  virtual void OnFaultData(const FaultDatas& data) {}

  /// @brief 控制权丢失回调
  virtual void OnControlLost(const ControlLostInfo& info) {}

  /// @brief 控制权可用回调
  virtual void OnControlAvailable(const ControlAvailableInfo& info) {}

  virtual ~IDataCallback() = default;
};

/// @brief 用户层根据需要来进行实现
///        非阻塞模式下使用：表示Robot已收到控制命令
class ROBOT_EXPORT_API IControlCallback {
 public:
  /// @brief 收到急停命令Ack
  /// @param on true:下发的是开启急停; false:下发的是关闭急停
  virtual void OnSoftEmergencyStop(bool on) {}

  /// @brief 收到站立命令Ack
  virtual void OnStandUp() {}

  /// @brief 收到卧倒命令Ack
  virtual void OnLieDown() {}

  /// @brief 收到匍匐命令Ack
  virtual void OnCrawl() {}

  /// @brief 收到爬高台命令Ack
  virtual void OnClimb() {}

  /// @brief 收到瘦身命令Ack
  virtual void OnSlim() {}

  /// @brief 收到步态命令Ack
  virtual void OnGait() {}

  /// @brief 收到调转头尾命令Ack
  virtual void OnReverseHeadTail() {}

  /// @brief 收到切换Mode命令Ack
  /// @param mode 用户下发的Mode
  virtual void OnMode(int mode) {}

  /// @brief 收到切换速度命令Ack
  /// @param speed_level 用户下发的速度
  virtual void OnSpeed(int speed_level) {}

  /// @brief 收到锁定命令Ack
  virtual void OnLocked() {}

  /// @brief 收到前补光灯命令Ack
  /// @param on true:开启; false:关闭
  virtual void OnFrontLight(bool on) {}

  /// @brief 收到后补光灯命令Ack
  /// @param on true:开启; false:关闭
  virtual void OnBackLight(bool on) {}

  /// @brief 收到自动模式灯光命令Ack
  /// @param on true:开启; false:关闭
  virtual void OnAutoModeLight(bool on) {}

  /// @brief 收到光强值配置命令Ack
  /// @param on true:开启; false:关闭
  virtual void OnLuxConfig(bool on) {}

  /// @brief 收到IMU配置命令Ack
  /// @param freq 下发的频率
  virtual void OnImuConfig(int freq) {}

  /// @brief 收到运动控制配置命令Ack
  /// @param on true:开启; false:关闭
  virtual void OnMcConfig(bool on) {}

  /// @brief 收到获取控制权命令Ack
  /// @param ack
  virtual void OnTakeControlAck(const TakeControlAck& ack) {}

  /// @brief 收到释放控制权命令Ack
  /// @param ack
  virtual void OnReleaseControlAck(const ReleaseControlAck& ack) {}

  virtual ~IControlCallback() = default;
};
}  // namespace robot_sdk
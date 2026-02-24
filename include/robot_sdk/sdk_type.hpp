#pragma once

#include <cstdint>
#include <string>
#include <vector>
namespace robot_sdk {

enum class TransportProtocol { WebSocket, Udp };

enum class FaultCode {
  Unknown,                      // 异常未知
  ActuatorDisabled = 10,        // 执行器失能
  ActuatorEncoderError,         // 执行器编码器错误
  ActuatorOffline,              // 执行器掉线
  ActuatorOverVoltage,          // 执行器过压
  ActuatorOverheat,             // 执行器过温
  ActuatorTempWarn,             // 执行器过温预警
  ActuatorTimeout,              // 执行器控制超时
  ActuatorUndervolt,            // 执行器欠压
  PowerControlOverTemp,         // 单电池过温预警
  PowerControlPowerEmpty,       // 单电池电量低于10%
  PowerControlPowerLow,         // 单电池电量低于20%，高于10%
  PowerControlOffline,          // 电源控制板MCU连接失败
  CANBroken,                    // CAN通信异常
  RobotRemoteKeepAliveFailure,  // 遥控器断链
  SystemClockSanityError,       // 系统时间跳变
  SystemRobotStatusError,       // 机器人状态异常
  IMUConnectError,              // IMU连接异常
  IMUDataNotUpdated,            // IMU数据不更新
};

enum class FaultLevel {
  Unknown = 0,
  FatalError,
  Error,
  Warn,
};

/// @brief 故障数据
struct FaultData {
  /// @brief 故障码
  FaultCode code;
  /// @brief 等级
  FaultLevel level;
  /// @brief 故障信息
  std::string message;
};
using FaultDatas = std::vector<FaultData>;

/// @brief IMU数据
struct ImuData {
  /// @brief 加速度计
  float acc_x, acc_y, acc_z;
  /// @brief 陀螺仪
  float gyro_x, gyro_y, gyro_z;
  /// @brief 四元数
  float quat_x, quat_y, quat_z, quat_w;
};

enum class PowerSupplyStatus {
  /// @brief 电源状态未知
  UNKNOWN = 0,
  /// @brief 正在充电
  CHARGING = 1,
  /// @brief 放电中
  DISCHARGING = 2,
  /// @brief 已满电
  FULL = 4,
};

struct BatteryData {
  /// @brief 电池1 电量
  float power1;
  /// @brief 电池2 电量
  float power2;
  /// @brief 电池1 是否存在
  bool present1;
  /// @brief 电池2 是否存在
  bool present2;
  /// @brief 电池1 电压(V)
  float voltage1;
  /// @brief 电池2 电压(V)
  float voltage2;
  /// @brief 电池1 温度
  float temperature1;
  /// @brief 电池2 温度
  float temperature2;
  /// @brief 电池1 电流(A)
  float current1;
  /// @brief 电池2 电流(A)
  float current2;
  /// @brief 电池1 电源状态
  PowerSupplyStatus power_supply_status1;
  /// @brief 电池2 电源状态
  PowerSupplyStatus power_supply_status2;
};

/// @brief 光强数据
struct LuxData {
  /// @brief 光强值
  float lux;
};

/// @brief 补光灯状态
enum class FillLightStatus {
  /// @brief 异常未知
  FILL_LIGHT_STATUS_UNKNOWN = 0,
  /// @brief 补光灯打开
  FILL_LIGHT_STATUS_ON,
  /// @brief 补光灯关闭
  FILL_LIGHT_STATUS_OFF,
};

/// @brief 速度等级
enum class SpeedLevel {
  /// @brief 异常未知
  SPEED_LEVEL_UNKNOWN = 0,
  /// @brief 慢速
  SPEED_LEVEL_SLOW,
  /// @brief 中速
  SPEED_LEVEL_MEDIUM,
  /// @brief 高速
  SPEED_LEVEL_HIGH,
};

/// @brief 急停状态
enum class EmergencyStatus {
  /// @brief 异常未知
  EMERGENCY_STATUS_UNKNOWN = 0,
  /// @brief 急停解除
  EMERGENCY_STATUS_RECOVER,
  /// @brief 急停触发
  EMERGENCY_STATUS_STOP,
};

/// @brief 狗头方向
enum class HeadDirection {
  /// @brief 异常未知
  HEAD_DIRECTION_UNKNOWN = 0,
  /// @brief 狗头方向为头
  HEAD_DIRECTION_HEAD,
  /// @brief 狗尾方向为头
  HEAD_DIRECTION_TAIL,
};

/// @brief 运动模式
enum class SportMode {
  /// @brief 异常未知
  SPORT_MODE_UNKNOWN = 0,
  /// @brief 通用模式
  SPORT_MODE_GENERAL,
  /// @brief 原地模式
  SPORT_MODE_IN_PLACE,
  /// @brief 登阶模式
  SPORT_MODE_STAIR,
};

/// @brief 运动状态
enum class MotionStatus {
  /// @brief 异常未知
  MOTION_STATUS_UNKNOWN = 0,
  /// @brief 站立
  MOTION_STATUS_STAND_UP,
  /// @brief 卧倒
  MOTION_STATUS_LIE_DOWN,
  /// @brief 匍匐
  MOTION_STATUS_CRAWL,
  /// @brief 锁定
  MOTION_STATUS_LOCKED,
  /// @brief 通用状态
  MOTION_STATUS_GENERAL,
  /// @brief 原地状态
  MOTION_STATUS_IN_PLACE,
  /// @brief 登阶状态
  MOTION_STATUS_STAIR,
  /// @brief 爬高台状态
  MOTION_STATUS_CLIMB,
  /// @brief 瘦身状态
  MOTION_STATUS_SLIM,
  /// @brief 步态状态
  MOTION_STATUS_GAIT,
};

enum class CtrlSource {
  CTRL_SOURCE_UNKNOWN = 0,
  CTRL_SOURCE_APP = 1,
  CTRL_SOURCE_SDK = 2,
  CTRL_SOURCE_OTHER = 3,
};

/// @brief 速度信息
struct Speed {
  /// @brief 前进/后退速度
  double line;
  /// @brief 左右平移速度
  double translation;
  /// @brief 转向速度
  double angle;
};

/// @brief 电机温度数据
struct MotorTemp {
  /// @brief 左前腿1号关节电机温度
  float fl1;

  /// @brief 左前腿2号关节电机温度
  float fl2;

  /// @brief 左前腿3号关节电机温度
  float fl3;

  /// @brief 左前腿4号关节电机温度
  float fl4;

  /// @brief 右前腿1号关节电机温度
  float fr1;

  /// @brief 右前腿2号关节电机温度
  float fr2;

  /// @brief 右前腿3号关节电机温度
  float fr3;

  /// @brief 右前腿4号关节电机温度
  float fr4;

  /// @brief 左后腿1号关节电机温度
  float bl1;

  /// @brief 左后腿2号关节电机温度
  float bl2;

  /// @brief 左后腿3号关节电机温度
  float bl3;

  /// @brief 左后腿4号关节电机温度
  float bl4;

  /// @brief 右后腿1号关节电机温度
  float br1;

  /// @brief 右后腿2号关节电机温度
  float br2;

  /// @brief 右后腿3号关节电机温度
  float br3;

  /// @brief 右后腿4号关节电机温度
  float br4;
};

/// @brief 机器人状态信息
struct RobotState {
  /// @brief 角度
  double head_angle;

  /// @brief 前补光灯
  FillLightStatus front_fill_light;

  /// @brief 后补光灯
  FillLightStatus back_fill_light;

  /// @brief 是否自动模式灯光
  bool auto_mode_light;

  /// @brief 当前速度等级
  SpeedLevel speed_level;

  /// @brief 是否发生软急停
  EmergencyStatus software_emergency_status;

  /// @brief 是否发生硬急停
  EmergencyStatus hardware_emergency_status;

  /// @brief 当前头方向
  HeadDirection head_direction;

  /// @brief 运动状态
  MotionStatus motion_status;

  /// @brief 电池电量
  BatteryData battery;

  /// @brief 当前速度
  Speed speed;

  /// @brief 里程累积数据
  float mile_data;

  /// @brief 电机温度
  MotorTemp motor_temp;

  /// @brief 运动模式
  SportMode sport_mode;

  /// @brief 控制来源
  CtrlSource control_source;
};

/// @brief 运动控制数据信息
struct MotionData {
  /// ---运动状态信息---

  /// @brief 四元数 [w, x, y, z]
  float quat[4];

  /// @brief 世界坐标系下速度 [x, y, z] (m/s)
  float v_world[3];

  /// @brief 世界坐标系下位置 [x, y, z] (m)
  float position[3];

  /// @brief 世界坐标系角速度 [x, y, z] (rad/s)
  float omega_world[3];

  /// @brief 自身坐标系下速度 [x, y, z] (m/s)
  float v_body[3];

  /// @brief 自身坐标系下角速度速度 [x, y, z] (rad/s)
  float omega_body[3];
};

struct ControlLostInfo {};

struct ControlAvailableInfo {};

/// @brief 控制命令确认信息
struct TakeControlAck {
  /// @brief 是否成功 0:成功，非0：失败
  uint32_t error_code;
  /// @brief 失败原因描述
  std::string reason;
};

/// @brief 释放控制命令确认信息
struct ReleaseControlAck {
  /// @brief 是否成功 0:成功，非0：失败
  uint32_t error_code;
  /// @brief 失败原因描述
  std::string reason;
};

}  // namespace robot_sdk
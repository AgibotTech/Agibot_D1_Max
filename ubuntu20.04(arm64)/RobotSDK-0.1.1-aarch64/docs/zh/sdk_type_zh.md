# Robot SDK — 数据类型文档

## 概述

本文档描述 Robot SDK 中使用的所有数据类型、枚举和结构体定义。

---

## 命名空间

```cpp
namespace robot_sdk
```

---

## 传输协议

### TransportProtocol

```cpp
enum class TransportProtocol
```

**说明：**  
SDK 支持的传输协议类型。

| 枚举值 | 说明 |
|:--|:--|
| `WebSocket` | WebSocket 协议（默认推荐） |
| `Udp` | UDP 协议 |

---

## 故障相关

### FaultCode

```cpp
enum class FaultCode
```

**说明：**  
机器人系统中可能出现的故障码。
机器人系统中可能出现的故障码。

| 枚举值 | 说明 |
|:--|:--|
| `Unknown` | 异常未知 |
| `ActuatorDisabled` | 执行器失能 |
| `ActuatorEncoderError` | 执行器编码器错误 |
| `ActuatorOffline` | 执行器掉线 |
| `ActuatorOverVoltage` | 执行器过压 |
| `ActuatorOverheat` | 执行器过温 |
| `ActuatorTempWarn` | 执行器过温预警 |
| `ActuatorTimeout` | 执行器控制超时 |
| `ActuatorUndervolt` | 执行器欠压 |
| `PowerControlOverTemp` | 单电池过温预警 |
| `PowerControlPowerEmpty` | 单电池电量低于10% |
| `PowerControlPowerLow` | 单电池电量低于20%，高于10% |
| `PowerControlOffline` | 电源控制板MCU连接失败 |
| `CANBroken` | CAN通信异常 |
| `RobotRemoteKeepAliveFailure` | 遥控器断链 |
| `SystemClockSanityError` | 系统时间跳变 |
| `SystemRobotStatusError` | 机器人状态异常 |
| `IMUConnectError` | IMU连接异常 |
| `IMUDataNotUpdated` | IMU数据不更新 |

---

### FaultLevel

```cpp
enum class FaultLevel
```

**说明：**  
故障等级定义。

| 枚举值 | 整数值 | 说明 |
|:--|:--|:--|
| `Unknown` | 0 | 未知 |
| `FatalError` | 1 | 致命错误 |
| `Error` | 2 | 一般错误 |
| `Warn` | 3 | 警告 |

---

## IMU 数据

### ImuData

```cpp
struct ImuData
```

**说明：**  
IMU（惯性测量单元）数据结构体。

| 成员 | 类型 | 说明 |
|:--|:--|:--|
| `acc_x`, `acc_y`, `acc_z` | `float` | 加速度计三轴数据 (m/s²) |
| `gyro_x`, `gyro_y`, `gyro_z` | `float` | 陀螺仪三轴数据 (rad/s) |
| `quat_x`, `quat_y`, `quat_z`, `quat_w` | `float` | 四元数 |

---

## 故障数据

### FaultData

```cpp
struct FaultData
```

**说明：**  
故障信息结构体。

| 成员 | 类型 | 说明 |
|:--|:--|:--|
| `code` | `FaultCode` | 故障码 |
| `level` | `FaultLevel` | 故障等级 |
| `message` | `std::string` | 故障信息文本 |

---

## 电源相关

### PowerSupplyStatus

```cpp
enum class PowerSupplyStatus
```

**说明：**  
电池电源状态枚举。

| 枚举值 | 整数值 | 说明 |
|:--|:--|:--|
| `UNKNOWN` | 0 | 电源状态未知 |
| `CHARGING` | 1 | 正在充电 |
| `DISCHARGING` | 2 | 放电中 |
| `FULL` | 4 | 已满电 |
---

### BatteryData

```cpp
struct BatteryData
```

**说明：**  
双电池数据结构体。

| 成员 | 类型 | 说明 |
|:--|:--|:--|
| `power1` | `float` | 电池1 电量（百分比 0-100） |
| `power2` | `float` | 电池2 电量（百分比 0-100） |
| `present1` | `bool` | 电池1 是否在位 |
| `present2` | `bool` | 电池2 是否在位 |
| `voltage1` | `float` | 电池1 电压 (V) |
| `voltage2` | `float` | 电池2 电压 (V) |
| `temperature1` | `float` | 电池1 温度 (°C) |
| `temperature2` | `float` | 电池2 温度 (°C) |
| `current1` | `float` | 电池1 电流 (A) |
| `current2` | `float` | 电池2 电流 (A) |
| `power_supply_status1` | `PowerSupplyStatus` | 电池1 电源状态 |
| `power_supply_status2` | `PowerSupplyStatus` | 电池2 电源状态 |

---

## 传感器数据

### LuxData

```cpp
struct LuxData
```

**说明：**  
光强传感器数据。

| 成员 | 类型 | 说明 |
|:--|:--|:--|
| `lux` | `float` | 光强值 |

---

## 机器人状态枚举

### FillLightStatus

```cpp
enum class FillLightStatus
```

**说明：**  
补光灯状态枚举。

| 枚举值 | 整数值 | 说明 |
|:--|:--|:--|
| `FILL_LIGHT_STATUS_UNKNOWN` | 0 | 未知状态 |
| `FILL_LIGHT_STATUS_ON` | 1 | 补光灯打开 |
| `FILL_LIGHT_STATUS_OFF` | 2 | 补光灯关闭 |

---

### SpeedLevel

```cpp
enum class SpeedLevel
```

**说明：**  
速度等级枚举，影响 `Move` 命令的速度限制。

| 枚举值 | 整数值 | 说明 |
|:--|:--|:--|
| `SPEED_LEVEL_UNKNOWN` | 0 | 未知 |
| `SPEED_LEVEL_SLOW` | 1 | 低速 |
| `SPEED_LEVEL_MEDIUM` | 2 | 中速 |
| `SPEED_LEVEL_HIGH` | 3 | 高速 |

**速度限制说明：**

| 等级 | vx (前后) | vy (左右) | vyaw (旋转) |
|:--|:--|:--|:--|
| 低速 | -0.5 ~ +0.5 m/s | -1.0 ~ +1.0 m/s | -2.0 ~ +2.0 rad/s |
| 中速 | 0 m/s | -2.0 ~ +2.0 m/s | -1.5 ~ +1.5 rad/s |
| 高速 | 0 m/s | -3.0 ~ +3.0 m/s | -1.0 ~ +1.0 rad/s |

---

### EmergencyStatus

```cpp
enum class EmergencyStatus
```

**说明：**  
急停状态枚举。

| 枚举值 | 整数值 | 说明 |
|:--|:--|:--|
| `EMERGENCY_STATUS_UNKNOWN` | 0 | 未知 |
| `EMERGENCY_STATUS_RECOVER` | 1 | 急停解除 |
| `EMERGENCY_STATUS_STOP` | 2 | 急停触发 |

---

### HeadDirection

```cpp
enum class HeadDirection
```

**说明：**  
机器人头尾方向枚举。

| 枚举值 | 整数值 | 说明 |
|:--|:--|:--|
| `HEAD_DIRECTION_UNKNOWN` | 0 | 未知 |
| `HEAD_DIRECTION_HEAD` | 1 | 狗头方向为前 |
| `HEAD_DIRECTION_TAIL` | 2 | 狗尾方向为前 |

---

### SportMode

```cpp
enum class SportMode
```

**说明：**  
机器人运动模式枚举。

| 枚举值 | 整数值 | 说明 |
|:--|:--|:--|
| `SPORT_MODE_UNKNOWN` | 0 | 未知 |
| `SPORT_MODE_GENERAL` | 1 | 通用模式（可移动与一些姿态变换） |
| `SPORT_MODE_IN_PLACE` | 2 | 原地模式（原地动作） |
| `SPORT_MODE_STAIR` | 3 | 登阶模式（仅支持移动） |

---

### MotionStatus

```cpp
enum class MotionStatus
```

**说明：**  
机器人运动状态枚举。

| 枚举值 | 整数值 | 说明 |
|:--|:--|:--|
| `MOTION_STATUS_UNKNOWN` | 0 | 未知 |
| `MOTION_STATUS_STAND_UP` | 1 | 站立中 |
| `MOTION_STATUS_LIE_DOWN` | 2 | 卧倒状态 |
| `MOTION_STATUS_CRAWL` | 3 | 匍匐状态 |
| `MOTION_STATUS_LOCKED` | 4 | 锁定状态 |
| `MOTION_STATUS_GENERAL` | 5 | 通用模式运动状态 |
| `MOTION_STATUS_IN_PLACE` | 6 | 原地模式运动状态 |
| `MOTION_STATUS_STAIR` | 7 | 登阶模式运动状态 |
| `MOTION_STATUS_CLIMB` | 8 | 爬高台状态 |
| `MOTION_STATUS_SLIM` | 9 | 瘦身状态 |
| `MOTION_STATUS_GAIT` | 10 | 步态状态 |

---

## 控制相关

### CtrlSource

```cpp
enum class CtrlSource
```

**说明：**  
机器人控制来源枚举。

| 枚举值 | 整数值 | 说明 |
|:--|:--|:--|
| `CTRL_SOURCE_UNKNOWN` | 0 | 未知 |
| `CTRL_SOURCE_APP` | 1 | APP 控制 |
| `CTRL_SOURCE_SDK` | 2 | SDK 控制 |
| `CTRL_SOURCE_OTHER` | 3 | 其他控制 |

---

### ControlLostInfo

```cpp
struct ControlLostInfo
```

**说明：**  
控制权丢失信息结构体。

---

### ControlAvailableInfo

```cpp
struct ControlAvailableInfo
```

**说明：**  
控制权可用信息结构体。

---

### TakeControlAck

```cpp
struct TakeControlAck
```

**说明：**  
获取控制权命令应答信息。

| 成员 | 类型 | 说明 |
|:--|:--|:--|
| `error_code` | `uint32_t` | 错误码（0 表示成功，非 0 表示失败） |
| `reason` | `std::string` | 失败原因描述 |

---

### ReleaseControlAck

```cpp
struct ReleaseControlAck
```

**说明：**  
释放控制权命令应答信息。

| 成员 | 类型 | 说明 |
|:--|:--|:--|
| `error_code` | `uint32_t` | 错误码（0 表示成功，非 0 表示失败） |
| `reason` | `std::string` | 失败原因描述 |

---

### CameraBitrateCmd

```cpp
struct CameraBitrateCmd
```

**说明：**  
摄像头码率配置参数。

| 成员 | 类型 | 说明 |
|:--|:--|:--|
| `camera_name` | `std::string` | 摄像头名称:前摄 "camera_front"，后摄 "camera_back" |
| `camera_bps` | `uint32_t` | 摄像头码率，单位bps(bit/s)，范围50000-100000000 |

---

### CameraBitrateAck

```cpp
struct CameraBitrateAck
```

**说明：**  
摄像头码率配置参数。

| 成员 | 类型 | 说明 |
|:--|:--|:--|
| `camera_name` | `std::string` | 摄像头名称:前摄 "camera_front"，后摄 "camera_back" |
| `camera_bps` | `uint32_t` | 摄像头码率，单位bps(bit/s)，范围50000-100000000 |

---

### Speed

```cpp
struct Speed
```

**说明：**  
机器人当前速度信息。

| 成员 | 类型 | 说明 |
|:--|:--|:--|
| `line` | `double` | 前进/后退速度 (m/s) |
| `translation` | `double` | 左右平移速度 (m/s) |
| `angle` | `double` | 转向角速度 (rad/s) |

---

### 关节温度说明

```cpp
std::unordered_map<std::string, double>
```

**说明：**  
机器人关节名字与关节温度信息映射表。

轮足关节名字如下表

| 名字 | 说明 |
|:--|:--|
| `fl1`, `fl2`, `fl3`, `fl4` |  左前腿 1-4 号关节 |
| `fr1`, `fr2`, `fr3`, `fr4` |  右前腿 1-4 号关节 |
| `bl1`, `bl2`, `bl3`, `bl4` |  左后腿 1-4 号关节 |
| `br1`, `br2`, `br3`, `br4` |  右后腿 1-4 号关节 |

---

### RobotState

```cpp
struct RobotState
```

**说明：**  
机器人综合状态信息。

| 成员 | 类型 | 说明 |
|:--|:--|:--|
| `head_angle` | `double` | 头部角度 (rad) |
| `front_fill_light` | `FillLightStatus` | 前补光灯状态 |
| `back_fill_light` | `FillLightStatus` | 后补光灯状态 |
| `auto_mode_light` | `bool` | 是否为自动灯光模式 |
| `speed_level` | `SpeedLevel` | 当前速度等级 |
| `software_emergency_status` | `EmergencyStatus` | 软件急停状态 |
| `hardware_emergency_status` | `EmergencyStatus` | 硬件急停状态 |
| `head_direction` | `HeadDirection` | 当前头尾方向 |
| `motion_status` | `MotionStatus` | 当前运动状态 |
| `battery` | `BatteryData` | 电池信息 |
| `speed` | `Speed` | 当前速度 |
| `mile_data` | `float` | 里程累积数据 (m) |
| `joint_temps` | `std::unordered_map<std::string, double>` | 关节温度<关节名称,关节温度> 单位：°C|
| `sport_mode` | `SportMode` | 运动模式 |
| `control_source` | `CtrlSource` | 控制来源 |

---

### MotionData

```cpp
struct MotionData
```

**说明：**  
运动控制详细数据信息。

| 成员 | 类型 | 说明 |
|:--|:--|:--|
| `quat[4]` | `float` | 四元数 [w, x, y, z] |
| `v_world[3]` | `float` | 世界坐标系下速度 [x, y, z] (m/s) |
| `position[3]` | `float` | 世界坐标系下位置 [x, y, z] (m) |
| `omega_world[3]` | `float` | 世界坐标系角速度 [x, y, z] (rad/s) |
| `v_body[3]` | `float` | 自身坐标系下速度 [x, y, z] (m/s) |
| `omega_body[3]` | `float` | 自身坐标系下角速度 [x, y, z] (rad/s) |
| `time_stamp` | `uint64_t` | 时间戳 (ns) |

### SpeedData

```cpp
struct SpeedData
```

**说明：**  
速度信息说明。

| 成员 | 类型 | 说明 |
|:--|:--|:--|
| `x` | `float` | 前后速度 (m/s) |
| `y` | `float` | 左右平移速度 (m/s) |
| `yaw` | `float` | 旋转角速度 (rad/s)|

---

## 相关文档

- [SDKClient API 文档](sdk_client_api_zh.md) - 客户端接口详细说明
- [连接配置文档](sdk_connection_zh.md) - 连接参数和状态说明
- [Callback 回调接口](sdk_callback_zh.md) - 回调接口定义
- [状态定义文档](sdk_state_zh.md) - 连接状态和运动状态详解
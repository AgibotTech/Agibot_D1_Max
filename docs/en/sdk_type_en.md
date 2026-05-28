# Robot SDK — Data Type Documentation

## Overview

This document describes all data types, enumerations, and structure definitions used in Robot SDK.

---

## Namespace

```cpp
namespace robot_sdk
```

---

## Transport Protocol

### TransportProtocol

```cpp
enum class TransportProtocol
```

**Description:**  
Transport protocol types supported by the SDK.

| Enum Value | Description |
|:--|:--|
| `WebSocket` | WebSocket protocol (default recommended) |
| `Udp` | UDP protocol |

---

## Fault-Related

### FaultCode

```cpp
enum class FaultCode
```

**Description:**  
Possible fault codes that may occur in the robot system.

| Enum Value | Description |
|:--|:--|
| `Unknown` | Unknown exception |
| `ActuatorDisabled` | Actuator disabled |
| `ActuatorEncoderError` | Actuator encoder error |
| `ActuatorOffline` | Actuator offline |
| `ActuatorOverVoltage` | Actuator overvoltage |
| `ActuatorOverheat` | Actuator overheating |
| `ActuatorTempWarn` | Actuator overheating warning |
| `ActuatorTimeout` | Actuator control timeout |
| `ActuatorUndervolt` | Actuator undervoltage |
| `PowerControlOverTemp` | Single battery overheating warning |
| `PowerControlPowerEmpty` | Single battery power below 10% |
| `PowerControlPowerLow` | Single battery power below 20%, above 10% |
| `PowerControlOffline` | Power control board MCU connection failed |
| `CANBroken` | CAN communication error |
| `RobotRemoteKeepAliveFailure` | Remote control disconnected |
| `SystemClockSanityError` | System time jump detected |
| `SystemRobotStatusError` | Robot status abnormal |
| `IMUConnectError` | IMU connection error |
| `IMUDataNotUpdated` | IMU data not updating |

---

### FaultLevel

```cpp
enum class FaultLevel
```

**Description:**  
Fault level definition.

| Enum Value | Integer Value | Description |
|:--|:--|:--|
| `Unknown` | 0 | Unknown |
| `FatalError` | 1 | Fatal error |
| `Error` | 2 | General error |
| `Warn` | 3 | Warning |

---

## IMU Data

### ImuData

```cpp
struct ImuData
```

**Description:**  
IMU (Inertial Measurement Unit) data structure.

| Member | Type | Description |
|:--|:--|:--|
| `acc_x`, `acc_y`, `acc_z` | `float` | Accelerometer three-axis data (m/s²) |
| `gyro_x`, `gyro_y`, `gyro_z` | `float` | Gyroscope three-axis data (rad/s) |
| `quat_x`, `quat_y`, `quat_z`, `quat_w` | `float` | Quaternion |

---

## Fault Data

### FaultData

```cpp
struct FaultData
```

**Description:**  
Fault information structure.

| Member | Type | Description |
|:--|:--|:--|
| `code` | `FaultCode` | Fault code |
| `level` | `FaultLevel` | Fault level |
| `message` | `std::string` | Fault information text |

---

## Power-Related

### PowerSupplyStatus

```cpp
enum class PowerSupplyStatus
```

**Description:**  
Battery power supply status enumeration.

| Enum Value | Integer Value | Description |
|:--|:--|:--|
| `UNKNOWN` | 0 | Power supply status unknown |
| `CHARGING` | 1 | Charging |
| `DISCHARGING` | 2 | Discharging |
| `FULL` | 4 | Fully charged |

---

### BatteryData

```cpp
struct BatteryData
```

**Description:**  
Dual battery data structure.

| Member | Type | Description |
|:--|:--|:--|
| `power1` | `float` | Battery 1 charge capacity (percentage 0-100) |
| `power2` | `float` | Battery 2 charge capacity (percentage 0-100) |
| `present1` | `bool` | Battery 1 present |
| `present2` | `bool` | Battery 2 present |
| `voltage1` | `float` | Battery 1 voltage (V) |
| `voltage2` | `float` | Battery 2 voltage (V) |
| `temperature1` | `float` | Battery 1 temperature (°C) |
| `temperature2` | `float` | Battery 2 temperature (°C) |
| `current1` | `float` | Battery 1 current (A) |
| `current2` | `float` | Battery 2 current (A) |
| `power_supply_status1` | `PowerSupplyStatus` | Battery 1 power supply status |
| `power_supply_status2` | `PowerSupplyStatus` | Battery 2 power supply status |

---

## Sensor Data

### LuxData

```cpp
struct LuxData
```

**Description:**  
Light intensity sensor data.

| Member | Type | Description |
|:--|:--|:--|
| `lux` | `float` | Light intensity value |

---

## Robot State Enumerations

### FillLightStatus

```cpp
enum class FillLightStatus
```

**Description:**  
Fill light status enumeration.

| Enum Value | Integer Value | Description |
|:--|:--|:--|
| `FILL_LIGHT_STATUS_UNKNOWN` | 0 | Unknown status |
| `FILL_LIGHT_STATUS_ON` | 1 | Fill light on |
| `FILL_LIGHT_STATUS_OFF` | 2 | Fill light off |

---

### SpeedLevel

```cpp
enum class SpeedLevel
```

**Description:**  
Speed level enumeration, affecting speed limits for the `Move` command.

| Enum Value | Integer Value | Description |
|:--|:--|:--|
| `SPEED_LEVEL_UNKNOWN` | 0 | Unknown |
| `SPEED_LEVEL_SLOW` | 1 | Low speed |
| `SPEED_LEVEL_MEDIUM` | 2 | Medium speed |
| `SPEED_LEVEL_HIGH` | 3 | High speed |

**Speed Limit Specifications:**

| Level | vx (forward/back) | vy (left/right) | vyaw (rotation) |
|:--|:--|:--|:--|
| Low speed | -0.5 ~ +0.5 m/s | -1.0 ~ +1.0 m/s | -2.0 ~ +2.0 rad/s |
| Medium speed | 0 m/s | -2.0 ~ +2.0 m/s | -1.5 ~ +1.5 rad/s |
| High speed | 0 m/s | -3.0 ~ +3.0 m/s | -1.0 ~ +1.0 rad/s |

---

### EmergencyStatus

```cpp
enum class EmergencyStatus
```

**Description:**  
Emergency stop status enumeration.

| Enum Value | Integer Value | Description |
|:--|:--|:--|
| `EMERGENCY_STATUS_UNKNOWN` | 0 | Unknown |
| `EMERGENCY_STATUS_RECOVER` | 1 | Emergency stop released |
| `EMERGENCY_STATUS_STOP` | 2 | Emergency stop activated |

---

### HeadDirection

```cpp
enum class HeadDirection
```

**Description:**  
Robot head/tail direction enumeration.

| Enum Value | Integer Value | Description |
|:--|:--|:--|
| `HEAD_DIRECTION_UNKNOWN` | 0 | Unknown |
| `HEAD_DIRECTION_HEAD` | 1 | Robot head facing forward |
| `HEAD_DIRECTION_TAIL` | 2 | Robot tail facing forward |

---

### SportMode

```cpp
enum class SportMode
```

**Description:**  
Robot motion mode enumeration.

| Enum Value | Integer Value | Description |
|:--|:--|:--|
| `SPORT_MODE_UNKNOWN` | 0 | Unknown |
| `SPORT_MODE_GENERAL` | 1 | General mode (mobile and some posture transformations) |
| `SPORT_MODE_IN_PLACE` | 2 | In-place mode (in-place actions) |
| `SPORT_MODE_STAIR` | 3 | Stair-climbing mode (mobile only) |

---

### MotionStatus

```cpp
enum class MotionStatus
```

**Description:**  
Robot motion state enumeration.

| Enum Value | Integer Value | Description |
|:--|:--|:--|
| `MOTION_STATUS_UNKNOWN` | 0 | Unknown |
| `MOTION_STATUS_STAND_UP` | 1 | Standing |
| `MOTION_STATUS_LIE_DOWN` | 2 | Lying down |
| `MOTION_STATUS_CRAWL` | 3 | Crawling |
| `MOTION_STATUS_LOCKED` | 4 | Locked |
| `MOTION_STATUS_GENERAL` | 5 | General mode motion state |
| `MOTION_STATUS_IN_PLACE` | 6 | In-place mode motion state |
| `MOTION_STATUS_STAIR` | 7 | Stair-climbing mode motion state |
| `MOTION_STATUS_CLIMB` | 8 | Climbing high platform state |
| `MOTION_STATUS_SLIM` | 9 | Slim (body compress) state |
| `MOTION_STATUS_GAIT` | 10 | Gait state |

---

## Control-Related

### CtrlSource

```cpp
enum class CtrlSource
```

**Description:**  
Robot control source enumeration.

| Enum Value | Integer Value | Description |
|:--|:--|:--|
| `CTRL_SOURCE_UNKNOWN` | 0 | Unknown |
| `CTRL_SOURCE_APP` | 1 | APP control |
| `CTRL_SOURCE_SDK` | 2 | SDK control |
| `CTRL_SOURCE_OTHER` | 3 | Other control |

---

### ControlLostInfo

```cpp
struct ControlLostInfo
```

**Description:**  
Control lost information structure.

---

### ControlAvailableInfo

```cpp
struct ControlAvailableInfo
```

**Description:**  
Control available information structure.

---

### TakeControlAck

```cpp
struct TakeControlAck
```

**Description:**  
Take control command acknowledgment information.

| Member | Type | Description |
|:--|:--|:--|
| `error_code` | `uint32_t` | Error code (0 indicates success, non-zero indicates failure) |
| `reason` | `std::string` | Failure reason description |

---

### ReleaseControlAck

```cpp
struct ReleaseControlAck
```

**Description:**  
Release control command acknowledgment information.

| Member | Type | Description |
|:--|:--|:--|
| `error_code` | `uint32_t` | Error code (0 indicates success, non-zero indicates failure) |
| `reason` | `std::string` | Failure reason description |

---

### CameraBitrateCmd

```cpp
struct CameraBitrateCmd
```

**Description:**  
Camera bitrate configuration parameters.

| Member | Type | Description |
|:--|:--|:--|
| `camera_name` | `std::string` | Camera name: "camera_front" for front camera, "camera_back" for back camera |
| `camera_bps` | `uint32_t` | Camera bitrate in bps (bit/s), range 50000-100000000 |

---

### CameraBitrateAck

```cpp
struct CameraBitrateAck
```

**Description:**  
Camera bitrate configuration acknowledgment.

| Member | Type | Description |
|:--|:--|:--|
| `camera_name` | `std::string` | Camera name: "camera_front" for front camera, "camera_back" for back camera |
| `camera_bps` | `uint32_t` | Camera bitrate in bps (bit/s), range 50000-100000000 |

---

### Speed

```cpp
struct Speed
```

**Description:**  
Robot current velocity information.

| Member | Type | Description |
|:--|:--|:--|
| `line` | `double` | Forward/backward velocity (m/s) |
| `translation` | `double` | Left/right lateral velocity (m/s) |
| `angle` | `double` | Rotation velocity (angular velocity) (rad/s) |

---

### Joint temperature description

```cpp
std::unordered_map<std::string, double>
```

**说明：**  
A mapping table between robot joint names and joint temperature information.

The names of the joints of the wheeled robot are shown in the table below.

| Name | Description |
|:--|:--|
| `fl1`, `fl2`, `fl3`, `fl4` |  FL Joints 1–4 (Front Left)|
| `fr1`, `fr2`, `fr3`, `fr4` |  FR Joints 1–4 (Front Right)|
| `bl1`, `bl2`, `bl3`, `bl4` |  BL Joints 1–4 (Back Left) |
| `br1`, `br2`, `br3`, `br4` |  BR Joints 1–4 (Back Right) |

---

### RobotState

```cpp
struct RobotState
```

**Description:**  
Robot comprehensive state information.

| Member | Type | Description |
|:--|:--|:--|
| `head_angle` | `double` | Head angle (rad) |
| `front_fill_light` | `FillLightStatus` | Front fill light status |
| `back_fill_light` | `FillLightStatus` | Back fill light status |
| `auto_mode_light` | `bool` | Whether in automatic light mode |
| `speed_level` | `SpeedLevel` | Current speed level |
| `software_emergency_status` | `EmergencyStatus` | Software emergency stop status |
| `hardware_emergency_status` | `EmergencyStatus` | Hardware emergency stop status |
| `head_direction` | `HeadDirection` | Current head/tail direction |
| `motion_status` | `MotionStatus` | Current motion state |
| `battery` | `BatteryData` | Battery information |
| `speed` | `Speed` | Current velocity |
| `mile_data` | `float` | Cumulative mileage data (m) |
| `joint_temps` | `std::unordered_map<std::string, double>` | Joint temperature<Joint name, Joint temperature> unit: °C |
| `sport_mode` | `SportMode` | Motion mode |
| `control_source` | `CtrlSource` | Control source |

---

### MotionData

```cpp
struct MotionData
```

**Description:**  
Motion control detailed data information.

| Member | Type | Description |
|:--|:--|:--|
| `quat[4]` | `float` | Quaternion [w, x, y, z] |
| `v_world[3]` | `float` | Velocity in world coordinate system [x, y, z] (m/s) |
| `position[3]` | `float` | Position in world coordinate system [x, y, z] (m) |
| `omega_world[3]` | `float` | Angular velocity in world coordinate system [x, y, z] (rad/s) |
| `v_body[3]` | `float` | Velocity in body coordinate system [x, y, z] (m/s) |
| `omega_body[3]` | `float` | Angular velocity in body coordinate system [x, y, z] (rad/s) |
| `time_stamp` | `uint64_t` | time stamp (ns) |

### SpeedData

```cpp
struct SpeedData
```

**Description:**  
Speed ​​description information.

| Member | Type | Description |
|:--|:--|:--|
| `x` | `float` | Forward/backward velocity (m/s) |
| `y` | `float` | Left/right lateral velocity (m/s) |
| `yaw` | `float` | Rotation velocity (angular velocity) (rad/s)|

---

---

## Related documentation

- [SDKClient API ](sdk_client_api_en.md) - Detailed Description of Client Interfaces
- [Connection config](sdk_connection_en.md) - Connection Parameters and Status Description
- [Callback](sdk_callback_en.md) - Definition of the callback interface
- [State define](sdk_state_en.md) - Detailed Explanation of Connection Status and Motion State

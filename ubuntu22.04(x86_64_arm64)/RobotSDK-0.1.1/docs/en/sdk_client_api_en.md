# Robot SDK — `SDKClient` Class Interface Documentation

## Overview

`SDKClient` is the core communication class of the Robot SDK, providing connection, disconnection, and command sending interfaces.  
Developers can use this class to implement robot motion control, mode switching, emergency stop, light control, data reporting configuration, and other functions.

**Working Modes:**
- **Synchronous Mode (Blocking)**: `timeout_ms > 0`, function blocks waiting for operation completion or returns after timeout
- **Asynchronous Mode (Non-blocking)**: `timeout_ms = 0`, function returns immediately, operation result notified through callback function

**Note:** The `block` parameter is only used for `Connect` and `Disconnect` functions. Other control functions use the `timeout_ms` parameter to distinguish between synchronous/asynchronous modes.

---

## Namespace

```cpp
namespace robot_sdk
```

---

## Class Definition

```cpp
class ROBOT_EXPORT_API SDKClient
```

---

## Error Code Mechanism

All interfaces return `std::error_code` type to indicate operation results:
- **Success**: `ec.value() == 0` or `!ec`
- **Failure**: `ec.value() != 0` or `ec` is true
- Use `ec.message()` to obtain human-readable error description

**Example:**

```cpp
std::error_code ec = sdk.Connect("192.168.234.1", "8082", true);
if (ec) {
    std::cerr << "Connection failed: " << ec.message() << std::endl;
} else {
    std::cout << "Connection successful" << std::endl;
}
```

### SDK Common Error Codes

Standard Error Codes

| Error Code Enum | Integer Value | Meaning |
|:--|:--|:--|
| `std::errc::success` | 0 | Operation successful |
| `std::errc::invalid_argument` | 22 | Invalid argument |
| `std::errc::already_connected` | 106 | Already connected |
| `std::errc::not_connected` | 107 | Not connected |
| `std::errc::timed_out` | 110 | Operation timeout |
| `std::errc::connection_refused` | 111 | Connection refused |
| `std::errc::connection_already_in_progress` | 114 | Connection already in progress |
| `std::errc::operation_in_progress` | 115 | Operation in progress |
| `std::errc::operation_canceled` | 125 | Operation canceled |

SDK Internal Extended Error Codes

| Error Code Enum | Integer Value | Meaning |
|:--|:--|:--|
| `robot_sdk::errc::ShakeHandFailed` | 10000 | Handshake failed, commonly seen when internal handshake message not received during connection |
| `robot_sdk::errc::ProtocolMismatch` | 10001 | Protocol mismatch, commonly seen when connecting to older robot versions |
| `robot_sdk::errc::ControlledDenial` | 10002 | Control denied, commonly seen when multiple clients connect to the robot |

---

## Type Definitions

| Type Name | Definition | Description |
|:--|:--|:--|
| `ConnectHandler` | `std::function<void(const std::error_code&)>` | Connection completion callback function |
| `DisConnectHandler` | `std::function<void(const std::error_code&)>` | Disconnection completion callback function |
| `ErrorHandler` | `std::function<void(const std::error_code&)>` | SDK internal error callback function |
| `WriteHandler` | `std::function<void(const std::error_code&, std::size_t)>` | Command sending completion callback function |

---

## Constructor and Destructor

### Constructor

```cpp
SDKClient(ErrorHandler error_callback = [](const std::error_code&) {},
          ConnectionConfig connection_config = ConnectionConfig(),
          TransportProtocol type = TransportProtocol::Udp)
```

**Description:**  
Construct an SDK client object.

**Parameters:**
| Parameter Name | Type | Default Value | Description |
|:--|:--|:--|:--|
| `error_callback` | `ErrorHandler` | Empty callback | SDK internal communication exception callback function |
| `connection_config` | `ConnectionConfig` | Default config | Connection configuration parameters |
| `type` | `TransportProtocol` | `Udp` | Transport protocol type |

**Related Types:**
- `ConnectionConfig`: Connection configuration, see [Connection Configuration Documentation](sdk_connection_en.md)
- `TransportProtocol`: Transport protocol type, see [Connection Configuration Documentation](sdk_connection_en.md)

---

### Destructor

```cpp
~SDKClient()
```

**Description:**  
Destructor, automatically releases resources and disconnects.

---

## Connection Management

### Connect - Connect to Robot

```cpp
std::error_code Connect(std::string ip, std::string port, 
                        bool block = false, 
                        ConnectHandler handler = [](const std::error_code&) {})
```

**Description:**  
Connect to the robot at the specified IP and port.

By default: when using WebSocket, you need to connect to port 8081 of the machine; when using UDP, you need to connect to port 8082 of the machine.

⚠️ **Note:** 

If the robot is already connected to the App, the SDK will not be able to control the robot.

**Parameters:**
| Parameter Name | Type | Default Value | Description |
|:--|:--|:--|:--|
| `ip` | `std::string` | - | Target IP address |
| `port` | `std::string` | - | Target port number |
| `block` | `bool` | `false` | `false`: asynchronous mode; `true`: synchronous mode |
| `handler` | `ConnectHandler` | Empty callback | Callback function after connection completion in asynchronous mode; not used in synchronous mode |

**Return Value:**  
- `std::errc::success`: Operation successful (asynchronous mode means request sent; synchronous mode means connection completed)
- Common failure error codes:
  - `std::errc::operation_in_progress`: Operation in progress
  - `std::errc::operation_canceled`: Operation canceled

**Note:**
- Asynchronous mode: Function return only indicates connection request sent; actual connection result notified through callback
- Synchronous mode: Function return indicates connection result

---

### Disconnect - Disconnect

```cpp
std::error_code Disconnect(bool block = false, 
                           DisConnectHandler handler = [](const std::error_code&) {})
```

**Description:**  
Disconnect from the robot.

**Parameters:**
| Parameter Name | Type | Default Value | Description |
|:--|:--|:--|:--|
| `block` | `bool` | `false` | `false`: asynchronous mode; `true`: synchronous mode |
| `handler` | `DisConnectHandler` | Empty callback | Callback function after disconnection in asynchronous mode; not used in synchronous mode |

**Return Value:**  
- `std::errc::success`: Operation successful
- Common failure error codes:
  - `std::errc::operation_in_progress`: Operation in progress
  - `std::errc::operation_canceled`: Operation canceled

**Note:**
- Asynchronous mode: Function return only indicates disconnect request sent; actual disconnect result notified through callback
- Synchronous mode: Function return indicates disconnect result

---

### IsConnected - Check Connection Status

```cpp
bool IsConnected() const
```

**Description:**  
Check whether currently connected to the robot.

**Return Value:**  
- `true`: Connected
- `false`: Not connected

---

### GetConnectionState - Get Connection State

```cpp
ConnectionState GetConnectionState() const
```

**Description:**  
Get current detailed connection state.

**Return Value:**  
`ConnectionState` enumeration value, see [Connection State Documentation](sdk_connection_en.md)

---

## Callback Configuration

### SetControlCallback - Set Control Callback

```cpp
void SetControlCallback(std::shared_ptr<IControlCallback> control_callback)
```

**Description:**  
Set control message notification function to receive command acknowledgments.

**Parameters:**
| Parameter Name | Type | Description |
|:--|:--|:--|
| `control_callback` | `std::shared_ptr<IControlCallback>` | Control callback object |

**Related Documentation:**  
See [Callback Reference](sdk_callback_en.md) for `IControlCallback` class description.

---

### SetDataCallback - Set Data Callback

```cpp
void SetDataCallback(std::shared_ptr<IDataCallback> data_callback)
```

**Description:**  
Set data message notification function, triggered when robot data is reported.

**Parameters:**
| Parameter Name | Type | Description |
|:--|:--|:--|
| `data_callback` | `std::shared_ptr<IDataCallback>` | Data callback object |

**Related Documentation:**  
See [Callback Reference](sdk_callback_en.md) for `IDataCallback` class description.

---

## Basic Control Interfaces

### SoftEmergencyStop - Emergency Stop

```cpp
std::error_code SoftEmergencyStop(bool on, int timeout_ms = 0,
                                   WriteHandler handler = [](const std::error_code&, std::size_t) {})
```

**Description:**  
Emergency stop control. After emergency stop is enabled, the system will not respond to other motion commands and speed will be set to 0.

**Parameters:**
| Parameter Name | Type | Default Value | Description |
|:--|:--|:--|:--|
| `on` | `bool` | - | `true`: activate emergency stop; `false`: deactivate emergency stop |
| `timeout_ms` | `int` | `0` | `0`: asynchronous mode; `> 0`: synchronous mode, maximum wait time (milliseconds) |
| `handler` | `WriteHandler` | Empty callback | Command sending result callback in asynchronous mode; not used in synchronous mode |

**Return Value:**  
- `std::errc::success`: Operation successful
- Common failure error codes:
  - `std::errc::timed_out`: Operation timeout
  - `std::errc::not_connected`: Not connected
  - `std::errc::operation_canceled`: Operation canceled

**Note:**
- Asynchronous mode: Function return only indicates command sent; send result notified through callback
- Synchronous mode: Function return indicates command send result

---

### StandUp - Stand Up

```cpp
std::error_code StandUp(int timeout_ms = 0,
                        WriteHandler handler = [](const std::error_code&, std::size_t) {})
```

**Description:**  
Robot stand up action.

**Parameters:**
| Parameter Name | Type | Default Value | Description |
|:--|:--|:--|:--|
| `timeout_ms` | `int` | `0` | `0`: asynchronous mode; `> 0`: synchronous mode, maximum wait time (milliseconds) |
| `handler` | `WriteHandler` | Empty callback | Command sending result callback in asynchronous mode; not used in synchronous mode |

**Return Value:**  
Same as `SoftEmergencyStop`

**Note:**  
After calling stand up, `MOTION_STATUS_STAND_UP` state is reported during the standing process. After standing completes, it automatically transitions to `MOTION_STATUS_GENERAL` or `MOTION_STATUS_IN_PLACE` state based on the current mode.

---

### LieDown - Lie Down

```cpp
std::error_code LieDown(int timeout_ms = 0,
                        WriteHandler handler = [](const std::error_code&, std::size_t) {})
```

**Description:**  
Robot lie down action.

**Parameters:**  
Same as `StandUp`

**Return Value:**  
Same as `SoftEmergencyStop`

---

### Crawl - Crawl

```cpp
std::error_code Crawl(int timeout_ms = 0,
                      WriteHandler handler = [](const std::error_code&, std::size_t) {})
```

**Description:**  
Robot crawl action.

**Parameters:**  
Same as `StandUp`

**Return Value:**  
Same as `SoftEmergencyStop`

---

### Climb - Climb

```cpp
std::error_code Climb(int timeout_ms = 0,
                      WriteHandler handler = [](const std::error_code&, std::size_t) {})
```

**Description:**  
Climb high platform action, only effective in **general mode**.

**Parameters:**  
Same as `StandUp`

**Return Value:**  
Same as `SoftEmergencyStop`

---

### Slim - Slim

```cpp
std::error_code Slim(int timeout_ms = 0,
                     WriteHandler handler = [](const std::error_code&, std::size_t) {})
```

**Description:**  
Slim (body compress) action, only effective in **general mode**.

**Parameters:**  
Same as `StandUp`

**Return Value:**  
Same as `SoftEmergencyStop`

---

### Gait - Gait

```cpp
std::error_code Gait(int timeout_ms = 0,
                     WriteHandler handler = [](const std::error_code&, std::size_t) {})
```

**Description:**  
Gait posture, only effective in **general mode**.

**Parameters:**  
Same as `StandUp`

**Return Value:**  
Same as `SoftEmergencyStop`

---

### DSB - Over the mouse barrier posture

```cpp
std::error_code DSB(int timeout_ms = 0,
                     WriteHandler handler = [](const std::error_code&, std::size_t) {})
```

**Description:**  
Over the mouse barrier posture, only effective in **general mode**.

**Parameters:**  
Same as `StandUp`

**Return Value:**  
Same as `SoftEmergencyStop`

---

### ReverseHeadTail - Reverse Head/Tail

```cpp
std::error_code ReverseHeadTail(int timeout_ms = 0,
                                 WriteHandler handler = [](const std::error_code&, std::size_t) {})
```

**Description:**  
Reverse the robot's head and tail direction.

**Parameters:**  
Same as `StandUp`

**Return Value:**  
Same as `SoftEmergencyStop`

---

### SetMode - Set Mode

```cpp
std::error_code SetMode(int mode, int timeout_ms = 0,
                        WriteHandler handler = [](const std::error_code&, std::size_t) {})
```

**Description:**  
Set robot operating mode, default is general mode.

**Parameters:**
| Parameter Name | Type | Default Value | Description |
|:--|:--|:--|:--|
| `mode` | `int` | - | `1`: general mode; `2`: in-place mode; `3`: stair-climbing mode |
| `timeout_ms` | `int` | `0` | `0`: asynchronous mode; `> 0`: synchronous mode, maximum wait time (milliseconds) |
| `handler` | `WriteHandler` | Empty callback | Command sending result callback in asynchronous mode; not used in synchronous mode |

**Return Value:**  
Same as `SoftEmergencyStop`

---

### SetSpeed - Set Speed Level

```cpp
std::error_code SetSpeed(int speed_level, int timeout_ms = 0,
                         WriteHandler handler = [](const std::error_code&, std::size_t) {})
```

**Description:**  
Set speed level, mainly affects the speed of the `Move` command in general mode, default is low speed.

Low speed effect on Move interface:

    forward_back:
        [-1.0, 1.0] maps to [-1.0m/s, +1.0m/s]

    left_right:
        [-1.0, 1.0] maps to [-0.5m/s, +0.5m/s]

    yaw:
        [-1.0, 1.0] maps to [-1.5rad/s, +1.5rad/s]

Medium speed effect on Move interface:

    forward_back:
        [-1.0, 1.0] maps to [-2.0m/s, +2.0m/s]

    left_right:
        When -1.0m/s < forward_back < 1.0m/s, [-1.0, 1.0] maps to [-0.5m/s, +0.5m/s]
        When forward_back > 1.0m/s or forward_back < -1.0m/s, limited to 0m/s

    yaw:
        When -1.0m/s < forward_back < 1.0m/s, [-1.0, 1.0] maps to [-1.5rad/s, +1.5rad/s]
        When forward_back > 1.0m/s or forward_back < -1.0m/s, [-1.0, 1.0] maps to [-1.0rad/s, +1.0rad/s]

High speed effect on Move interface:

    forward_back:
        [-1.0, 1.0] maps to [-3.0m/s, +3.0m/s]

    left_right:
        When -1.0m/s < forward_back < 1.0m/s, [-1.0, 1.0] maps to [-0.5m/s, +0.5m/s]
        When forward_back > 1.0m/s or forward_back < -1.0m/s, limited to 0m/s

    yaw:
        When -1.0m/s < forward_back < 1.0m/s, [-1.0, 1.0] maps to [-1.5rad/s, +1.5rad/s]
        When forward_back > 1.0m/s or forward_back < -1.0m/s, [-1.0, 1.0] maps to [-1.0rad/s, +1.0rad/s]
        When forward_back > 2.0m/s or forward_back < -2.0m/s, [-1.0, 1.0] maps to [-0.5rad/s, +0.5rad/s]

**Parameters:**
| Parameter Name | Type | Default Value | Description |
|:--|:--|:--|:--|
| `speed_level` | `int` | - | `1`: low speed; `2`: medium speed; `3`: high speed (range [1, 3]) |
| `timeout_ms` | `int` | `0` | `0`: asynchronous mode; `> 0`: synchronous mode, maximum wait time (milliseconds) |
| `handler` | `WriteHandler` | Empty callback | Command sending result callback in asynchronous mode; not used in synchronous mode |

**Return Value:**  
Same as `SoftEmergencyStop`

---

### Locked - Lock

```cpp
std::error_code Locked(int timeout_ms = 0,
                       WriteHandler handler = [](const std::error_code&, std::size_t) {})
```

**Description:**  
Lock the robot, keeping all joints at their current positions without movement.

**Parameters:**  
Same as `StandUp`

**Return Value:**  
Same as `SoftEmergencyStop`

**Note:**  
When issuing commands to transition to other motion states (stand up, crawl, lie down), the lock is automatically released and transitions to the corresponding action state.

---

## Light Control Interfaces

### FrontLight - Front Fill Light

```cpp
std::error_code FrontLight(bool on, int timeout_ms = 0,
                           WriteHandler handler = [](const std::error_code&, std::size_t) {})
```

**Description:**  
Set front fill light; automatic mode is disabled after setting.

**Parameters:**
| Parameter Name | Type | Default Value | Description |
|:--|:--|:--|:--|
| `on` | `bool` | - | `true`: on; `false`: off |
| `timeout_ms` | `int` | `0` | `0`: asynchronous mode; `> 0`: synchronous mode, maximum wait time (milliseconds) |
| `handler` | `WriteHandler` | Empty callback | Command sending result callback in asynchronous mode; not used in synchronous mode |

**Return Value:**  
Same as `SoftEmergencyStop`

---

### BackLight - Back Fill Light

```cpp
std::error_code BackLight(bool on, int timeout_ms = 0,
                          WriteHandler handler = [](const std::error_code&, std::size_t) {})
```

**Description:**  
Set back fill light; automatic mode is disabled after setting.

**Parameters:**  
Same as `FrontLight`

**Return Value:**  
Same as `SoftEmergencyStop`

---

### AutoModeLight - Auto Fill Light Mode

```cpp
std::error_code AutoModeLight(bool on, int timeout_ms = 0,
                              WriteHandler handler = [](const std::error_code&, std::size_t) {})
```

**Description:**  
Set automatic fill light mode.

**Parameters:**  
Same as `FrontLight`

**Return Value:**  
Same as `SoftEmergencyStop`

---

## Motion and Posture Control

### Move - Move

```cpp
std::error_code Move(float left_right, float forward_back, float yaw, 
                     int timeout_ms = 0,
                     WriteHandler handler = [](const std::error_code&, std::size_t) {})
```

**Description:**  
Used in **general mode** to move the robot, unit: percentage, speed limited by **SpeedLevel**.

The latest Move command will last for 1 second.

**Parameters:**
| Parameter Name | Type | Default Value | Description |
|:--|:--|:--|:--|
| `left_right` | `float` | - | Left/right lateral velocity, range [-1.0, 1.0]; positive for left, negative for right |
| `forward_back` | `float` | - | Forward/backward velocity, range [-1.0, 1.0]; positive for forward, negative for backward |
| `yaw` | `float` | - | Rotation velocity, range [-1.0, 1.0]; positive for left rotation, negative for right rotation |
| `timeout_ms` | `int` | `0` | `0`: asynchronous mode; `> 0`: synchronous mode, maximum wait time (milliseconds) |
| `handler` | `WriteHandler` | Empty callback | Command sending result callback in asynchronous mode; not used in synchronous mode |

**Return Value:**  
- `std::errc::success`: Operation successful
- Common failure error codes:
  - `std::errc::timed_out`: Operation timeout
  - `std::errc::not_connected`: Not connected
  - `std::errc::operation_canceled`: Operation canceled
  - `std::errc::invalid_argument`: Invalid argument

**Note:**  
Asynchronous mode: Function return only indicates command sent; send result notified through callback  
Synchronous mode: Function return indicates command send result

---

### Turn - Turn

```cpp
std::error_code Turn(int direction, int timeout_ms = 0,
                     WriteHandler handler = [](const std::error_code&, std::size_t) {})
```

**Description:**  
In-place left/right roll action (only effective in **in-place mode**).

**Parameters:**
| Parameter Name | Type | Default Value | Description |
|:--|:--|:--|:--|
| `direction` | `int` | - | 0: recover; 1: left roll; 2: right roll |
| `timeout_ms` | `int` | `0` | `0`: asynchronous mode; `> 0`: synchronous mode, maximum wait time (milliseconds) |
| `handler` | `WriteHandler` | Empty callback | Command sending result callback in asynchronous mode; not used in synchronous mode |

**Return Value:**  
- `std::errc::success`: Operation successful
- Common failure error codes:
  - `std::errc::timed_out`: Operation timeout
  - `std::errc::not_connected`: Not connected
  - `std::errc::operation_canceled`: Operation canceled

**Note:**  
Asynchronous mode: Function return only indicates command sent; send result notified through callback  
Synchronous mode: Function return indicates command send result

---

### ControlHead - Control Head

```cpp
std::error_code ControlHead(float left_right, float up_down, int timeout_ms = 0,
                            WriteHandler handler = [](const std::error_code&, std::size_t) {})
```

**Description:**  
Control robot "left/right head" and "head up/down" actions (only effective in **in-place mode**).

**Parameters:**
| Parameter Name | Type | Default Value | Description |
|:--|:--|:--|:--|
| `left_right` | `float` | - | Range [-1.0, 1.0]; positive: left head, negative: right head; unit rad/s |
| `up_down` | `float` | - | Range [-1.0, 1.0]; positive: head up, negative: head down; unit rad/s |
| `timeout_ms` | `int` | `0` | `0`: asynchronous mode; `> 0`: synchronous mode, maximum wait time (milliseconds) |
| `handler` | `WriteHandler` | Empty callback | Command sending result callback in asynchronous mode; not used in synchronous mode |

**Return Value:**  
- `std::errc::success`: Operation successful
- Common failure error codes:
  - `std::errc::timed_out`: Operation timeout
  - `std::errc::not_connected`: Not connected
  - `std::errc::operation_canceled`: Operation canceled
  - `std::errc::invalid_argument`: Invalid argument

**Note:**  
Asynchronous mode: Function return only indicates command sent; send result notified through callback  
Synchronous mode: Function return indicates command send result

---

### HighLowStance - High/Low Stance

```cpp
std::error_code HighLowStance(int stance, int timeout_ms = 0,
                              WriteHandler handler = [](const std::error_code&, std::size_t) {})
```

**Description:**  
Set high/low stance (only effective in **in-place mode**).

**Parameters:**
| Parameter Name | Type | Default Value | Description |
|:--|:--|:--|:--|
| `stance` | `int` | - | 0: recover; 1: high stance; 2: low stance |
| `timeout_ms` | `int` | `0` | `0`: asynchronous mode; `> 0`: synchronous mode, maximum wait time (milliseconds) |
| `handler` | `WriteHandler` | Empty callback | Command sending result callback in asynchronous mode; not used in synchronous mode |

**Return Value:**  
- `std::errc::success`: Operation successful
- Common failure error codes:
  - `std::errc::timed_out`: Operation timeout
  - `std::errc::not_connected`: Not connected
  - `std::errc::operation_canceled`: Operation canceled
  - `std::errc::invalid_argument`: Invalid argument

**Note:**  
Asynchronous mode: Function return only indicates command sent; send result notified through callback  
Synchronous mode: Function return indicates command send result

---

## Data Configuration Interfaces

### SetImuConfig - Configure IMU

```cpp
std::error_code SetImuConfig(int freq, int timeout_ms = 0,
                             WriteHandler handler = [](const std::error_code&, std::size_t) {})
```

**Description:**  
Configure IMU reporting frequency, disabled by default.

**Parameters:**
| Parameter Name | Type | Default Value | Description |
|:--|:--|:--|:--|
| `freq` | `int` | - | Range [0, 100]; 0 disables reporting |
| `timeout_ms` | `int` | `0` | `0`: asynchronous mode; `> 0`: synchronous mode, maximum wait time (milliseconds) |
| `handler` | `WriteHandler` | Empty callback | Command sending result callback in asynchronous mode; not used in synchronous mode |

**Return Value:**  
- `std::errc::success`: Operation successful
- Common failure error codes:
  - `std::errc::timed_out`: Operation timeout
  - `std::errc::not_connected`: Not connected
  - `std::errc::operation_canceled`: Operation canceled

**Note:**  
Asynchronous mode: Function return only indicates command sent; send result notified through callback  
Synchronous mode: Function return indicates command send result

---

### SetLuxConfig - Configure Light Intensity

```cpp
std::error_code SetLuxConfig(bool on, int timeout_ms = 0,
                             WriteHandler handler = [](const std::error_code&, std::size_t) {})
```

**Description:**  
Configure light intensity value reporting switch, disabled by default. After configuration, light intensity value is reported at 1Hz.

**Parameters:**
| Parameter Name | Type | Default Value | Description |
|:--|:--|:--|:--|
| `on` | `bool` | - | `true`: enable; `false`: disable |
| `timeout_ms` | `int` | `0` | `0`: asynchronous mode; `> 0`: synchronous mode, maximum wait time (milliseconds) |
| `handler` | `WriteHandler` | Empty callback | Command sending result callback in asynchronous mode; not used in synchronous mode |

**Return Value:**  
- `std::errc::success`: Operation successful
- Common failure error codes:
  - `std::errc::timed_out`: Operation timeout
  - `std::errc::not_connected`: Not connected
  - `std::errc::operation_canceled`: Operation canceled

**Note:**  
Asynchronous mode: Function return only indicates command sent; send result notified through callback  
Synchronous mode: Function return indicates command send result

---

### SetMcConfig - Configure Motion Data

```cpp
std::error_code SetMcConfig(bool on, int timeout_ms = 0,
                            WriteHandler handler = [](const std::error_code&, std::size_t) {})
```

**Description:**  
Configure motion data reporting switch, disabled by default. After configuration, motion data is reported at 50Hz.

**Parameters:**
| Parameter Name | Type | Default Value | Description |
|:--|:--|:--|:--|
| `on` | `bool` | - | `true`: enable; `false`: disable |
| `timeout_ms` | `int` | `0` | `0`: asynchronous mode; `> 0`: synchronous mode, maximum wait time (milliseconds) |
| `handler` | `WriteHandler` | Empty callback | Command sending result callback in asynchronous mode; not used in synchronous mode |

**Return Value:**  
- `std::errc::success`: Operation successful
- Common failure error codes:
  - `std::errc::timed_out`: Operation timeout
  - `std::errc::not_connected`: Not connected
  - `std::errc::operation_canceled`: Operation canceled

**Note:**  
Asynchronous mode: Function return only indicates command sent; send result notified through callback  
Synchronous mode: Function return indicates command send result

---

### SetSpeedReportConfig - Configure Speed Data

```cpp
std::error_code SetSpeedReportConfig(bool on, uint32_t frequency, int timeout_ms = 0,
                            WriteHandler handler = [](const std::error_code&, std::size_t) {})
```

**Description:**  
Configure speed data reporting switch, disabled by default. After configuration, speed data is reported.

**Parameters:**
| Parameter Name | Type | Default Value | Description |
|:--|:--|:--|:--|
| `on` | `bool` | - | `true`: enable; `false`: disable |
| `frequency` | `uint32_t` | - | report frequency. [1hz, 50hz] |
| `timeout_ms` | `int` | `0` | `0`: asynchronous mode; `> 0`: synchronous mode, maximum wait time (milliseconds) |
| `handler` | `WriteHandler` | Empty callback | Command sending result callback in asynchronous mode; not used in synchronous mode |

**Return Value:**  
- `std::errc::success`: Operation successful
- Common failure error codes:
  - `std::errc::timed_out`: Operation timeout
  - `std::errc::not_connected`: Not connected
  - `std::errc::operation_canceled`: Operation canceled

**Note:**  
Asynchronous mode: Function return only indicates command sent; send result notified through callback  
Synchronous mode: Function return indicates command send result

---

### TakeControl - Take Control

```cpp
std::error_code TakeControl(int timeout_ms = 0,
                            WriteHandler handler = [](const std::error_code&, std::size_t) {})
```

**Description:**  
Take control of the robot. If the APP is currently in control, you cannot forcibly take over.See [sdk_control_ownership](sdk_control_ownership_en.md)

**Parameters:**
| Parameter Name | Type | Default Value | Description |
|:--|:--|:--|:--|
| `timeout_ms` | `int` | `0` | `0`: asynchronous mode; `> 0`: synchronous mode, maximum wait time (milliseconds) |
| `handler` | `WriteHandler` | Empty callback | Command sending result callback in asynchronous mode; not used in synchronous mode |

**Return Value:**  
- `std::errc::success`: Operation successful
- Common failure error codes:
  - `std::errc::timed_out`: Operation timeout
  - `std::errc::not_connected`: Not connected
  - `std::errc::operation_canceled`: Operation canceled

**Note:**  
Asynchronous mode: Function return only indicates command sent; send result notified through callback  
Synchronous mode: Function return indicates command send result

---

### ReleaseControl - Release Control

```cpp
std::error_code ReleaseControl(int timeout_ms = 0,
                               WriteHandler handler = [](const std::error_code&, std::size_t) {})
```

**Description:**  
Release control of the robot.

**Parameters:**
| Parameter Name | Type | Default Value | Description |
|:--|:--|:--|:--|
| `timeout_ms` | `int` | `0` | `0`: asynchronous mode; `> 0`: synchronous mode, maximum wait time (milliseconds) |
| `handler` | `WriteHandler` | Empty callback | Command sending result callback in asynchronous mode; not used in synchronous mode |

**Return Value:**  
- `std::errc::success`: Operation successful
- Common failure error codes:
  - `std::errc::timed_out`: Operation timeout
  - `std::errc::not_connected`: Not connected
  - `std::errc::operation_canceled`: Operation canceled

**Note:**  
Asynchronous mode: Function return only indicates command sent; send result notified through callback  
Synchronous mode: Function return indicates command send result

---

### UpdateCameraBitrate - Update Camera Bitrate

```cpp
std::error_code UpdateCameraBitrate(
    CameraBitrateCmd cmd, int timeout_ms = 0,
    WriteHandler handler = [](const std::error_code&, std::size_t) {});
```

**Description:**  
Update camera bitrate.

**Parameters:**
| Parameter Name | Type | Default Value | Description |
|:--|:--|:--|:--|
| `cmd` | `CameraBitrateCmd` | - | Configure specific camera bitrate |
| `timeout_ms` | `int` | `0` | `0`: asynchronous mode; `> 0`: synchronous mode, maximum wait time (milliseconds) |
| `handler` | `WriteHandler` | Empty callback | Command sending result callback in asynchronous mode; not used in synchronous mode |

**Return Value:**  
- `std::errc::success`: Operation successful
- Common failure error codes:
  - `std::errc::timed_out`: Operation timeout
  - `std::errc::not_connected`: Not connected
  - `std::errc::operation_canceled`: Operation canceled

**Note:**  
Asynchronous mode: Function return only indicates command sent; send result notified through callback  
Synchronous mode: Function return indicates command send result

---

## Version Information

### `const std::string& Version() const`

Get SDK version string.

---

### `const std::string& ProtocolVersion() const`

Get protocol version string.

---

### `const std::string& SystemVersion() const`

Get system version string.

---

## Code Examples

```cpp
#include "robot_sdk/sdk_client.hpp"
using namespace robot_sdk;

int main() {
    SDKClient client([](const std::error_code& ec) {
        if (ec) std::cerr << "SDK Error: " << ec.message() << std::endl;
    });

    auto ec = client.Connect("192.168.234.1", "8082", true);
    if (ec) {
        std::cerr << "Connect failed: " << ec.message() << std::endl;
        return -1;
    }

    client.StandUp(2000);
    client.Move(0, 0.5, 0);
    client.Disconnect(true);
    return 0;
}
```

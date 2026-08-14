# Robot SDK — `SDKClient` 类接口文档

## 概述

`SDKClient` 是机器人 SDK 的核心通信类，提供连接、断开和命令发送接口。  
开发者通过该类可实现机器人运动控制、模式切换、急停、灯光控制、数据上报配置等功能。

**工作模式：**
- **同步模式（阻塞）**：`timeout_ms > 0`，函数阻塞等待操作完成或超时后返回结果
- **异步模式（非阻塞）**：`timeout_ms = 0`，函数立即返回，通过回调函数通知操作结果

**注意：**`block` 参数仅用于 `Connect` 和 `Disconnect` 函数，其他控制函数通过 `timeout_ms` 参数区分同步/异步模式。

---

## 命名空间

```cpp
namespace robot_sdk
```

---

## 类定义

```cpp
class ROBOT_EXPORT_API SDKClient
```

---

## 错误码机制

所有接口均返回 `std::error_code` 类型表示操作结果：
- **成功**：`ec.value() == 0` 或 `!ec`
- **失败**：`ec.value() != 0` 或 `ec` 为 true
- 通过 `ec.message()` 获取人类可读的错误描述

**示例：**

```cpp
std::error_code ec = sdk.Connect("192.168.234.1", "8082", true);
if (ec) {
    std::cerr << "连接失败: " << ec.message() << std::endl;
} else {
    std::cout << "连接成功" << std::endl;
}
```

### SDK 常用错误码

标准错误码

| 错误码枚举 | 整数值 | 含义 |
|:--|:--|:--|
| `std::errc::success` | 0 | 操作成功 |
| `std::errc::invalid_argument` | 22 | 参数无效 |
| `std::errc::already_connected` | 106 | 已连接 |
| `std::errc::not_connected` | 107 | 未连接 |
| `std::errc::timed_out` | 110 | 操作超时 |
| `std::errc::connection_refused` | 111 | 连接被拒绝 |
| `std::errc::connection_already_in_progress` | 114 | 连接正在进行中 |
| `std::errc::operation_in_progress` | 115 | 操作正在进行中 |
| `std::errc::operation_canceled` | 125 | 操作被取消 |

SDK内部扩展错误码
| 错误码枚举 | 整数值 | 含义 |
|:--|:--|:--|
| `robot_sdk::errc::ShakeHandFailed` | 10000 | 握手失败，常见于连接时内部未收到握手消息 |
| `robot_sdk::errc::ProtocolMismatch` | 10001 | 协议不匹配，常见于操作旧版本机器 |
| `robot_sdk::errc::ControlledDenial` | 10002 | 控制拒绝，常见于多客户端连接到机器 |

---

## 类型定义

| 类型名 | 定义 | 说明 |
|:--|:--|:--|
| `ConnectHandler` | `std::function<void(const std::error_code&)>` | 连接完成回调函数 |
| `DisConnectHandler` | `std::function<void(const std::error_code&)>` | 断开连接完成回调函数 |
| `ErrorHandler` | `std::function<void(const std::error_code&)>` | SDK 内部错误回调函数 |
| `WriteHandler` | `std::function<void(const std::error_code&, std::size_t)>` | 命令发送完成回调函数 |

---

## 构造与析构

### 构造函数

```cpp
SDKClient(ErrorHandler error_callback = [](const std::error_code&) {},
          ConnectionConfig connection_config = ConnectionConfig(),
          TransportProtocol type = TransportProtocol::Udp)
```

**说明：**  
构造一个 SDK 客户端对象。

**参数：**
| 参数名 | 类型 | 默认值 | 说明 |
|:--|:--|:--|:--|
| `error_callback` | `ErrorHandler` | 空回调 | SDK 内部通信异常回调函数 |
| `connection_config` | `ConnectionConfig` | 默认配置 | 连接配置参数 |
| `type` | `TransportProtocol` | `Udp` | 传输协议类型 |

**相关类型：**
- `ConnectionConfig`: 连接配置，详见 [连接配置文档](sdk_connection_zh.md)
- `TransportProtocol`: 传输协议类型，详见 [连接配置文档](sdk_connection_zh.md)

---

### 析构函数

```cpp
~SDKClient()
```

**说明：**  
析构函数，自动释放资源并断开连接。

---

## 连接管理

### Connect - 连接机器人

```cpp
std::error_code Connect(std::string ip, std::string port, 
                        bool block = false, 
                        ConnectHandler handler = [](const std::error_code&) {})
```

**说明：**  
连接到指定 IP 和端口的机器人。

默认情况下：当使用WebSocket时，需要连接机器的8081端口；当使用Udp时，需要连接机器的8082端口；

⚠️ **注意:** 

机器如果已连接至APP，那SDK将无法对狗进行控制。

**参数：**
| 参数名 | 类型 | 默认值 | 说明 |
|:--|:--|:--|:--|
| `ip` | `std::string` | - | 目标 IP 地址 |
| `port` | `std::string` | - | 目标端口号 |
| `block` | `bool` | `false` | `false`: 异步模式；`true`: 同步模式 |
| `handler` | `ConnectHandler` | 空回调 | 异步模式下连接完成后的回调函数；同步模式不使用 |

**返回值：**  
- `std::errc::success`: 操作成功（异步模式表示请求已发出，同步模式表示连接已完成）
- 常见失败错误码：
  - `std::errc::operation_in_progress`: 操作正在进行中
  - `std::errc::operation_canceled`: 操作已取消

**注意：**
- 异步模式：函数返回仅表示连接请求已发出，实际连接结果通过回调函数通知
- 同步模式：函数返回即表示连接结果

---

### Disconnect - 断开连接

```cpp
std::error_code Disconnect(bool block = false, 
                           DisConnectHandler handler = [](const std::error_code&) {})
```

**说明：**  
断开与机器人的连接。

**参数：**
| 参数名 | 类型 | 默认值 | 说明 |
|:--|:--|:--|:--|
| `block` | `bool` | `false` | `false`: 异步模式；`true`: 同步模式 |
| `handler` | `DisConnectHandler` | 空回调 | 异步模式下断开完成后的回调函数；同步模式不使用 |

**返回值：**  
- `std::errc::success`: 操作成功
- 常见失败错误码：
  - `std::errc::operation_in_progress`: 操作正在进行中
  - `std::errc::operation_canceled`: 操作已取消

**注意：**
- 异步模式：函数返回仅表示断开请求已发出，实际断开结果通过回调函数通知
- 同步模式：函数返回即表示断开结果

---

### IsConnected - 判断连接状态

```cpp
bool IsConnected() const
```

**说明：**  
判断当前是否已连接机器人。

**返回值：**  
- `true`: 已连接
- `false`: 未连接

---

### GetConnectionState - 获取连接状态

```cpp
ConnectionState GetConnectionState() const
```

**说明：**  
获取当前详细的连接状态。

**返回值：**  
`ConnectionState` 枚举值，详见 [连接状态文档](sdk_connection_zh.md)

---

## 回调设置

### SetControlCallback - 设置控制回调

```cpp
void SetControlCallback(std::shared_ptr<IControlCallback> control_callback)
```

**说明：**  
设置控制消息通知函数，用于接收控制命令的应答。

**参数：**
| 参数名 | 类型 | 说明 |
|:--|:--|:--|
| `control_callback` | `std::shared_ptr<IControlCallback>` | 控制回调对象 |

**相关文档：**  
详见 [Callback 参考](sdk_callback_zh.md) 中 `IControlCallback` 类的说明。

---

### SetDataCallback - 设置数据回调

```cpp
void SetDataCallback(std::shared_ptr<IDataCallback> data_callback)
```

**说明：**  
设置数据消息通知函数，当机器人数据上报时触发。

**参数：**
| 参数名 | 类型 | 说明 |
|:--|:--|:--|
| `data_callback` | `std::shared_ptr<IDataCallback>` | 数据回调对象 |

**相关文档：**  
详见 [Callback 参考](sdk_callback.md) 中 `IDataCallback` 类的说明。

---

## 基础控制接口

### SoftEmergencyStop - 急停

```cpp
std::error_code SoftEmergencyStop(bool on, int timeout_ms = 0,
                                   WriteHandler handler = [](const std::error_code&, std::size_t) {})
```

**说明：**  
急停控制。启用急停后，系统将不响应其他运动命令，并且速度设为 0。

**参数：**
| 参数名 | 类型 | 默认值 | 说明 |
|:--|:--|:--|:--|
| `on` | `bool` | - | `true`: 急停；`false`: 解除急停 |
| `timeout_ms` | `int` | `0` | `0`: 异步模式；`> 0`: 同步模式，最大等待时间（毫秒） |
| `handler` | `WriteHandler` | 空回调 | 异步模式下命令发送结果回调函数；同步模式不使用 |

**返回值：**  
- `std::errc::success`: 操作成功
- 常见失败错误码：
  - `std::errc::timed_out`: 操作超时
  - `std::errc::not_connected`: 未连接
  - `std::errc::operation_canceled`: 操作已取消

**注意：**
- 异步模式：函数返回仅表示命令已发送，发送结果通过回调函数通知
- 同步模式：函数返回即表示命令发送结果

---

### StandUp - 站立

```cpp
std::error_code StandUp(int timeout_ms = 0,
                        WriteHandler handler = [](const std::error_code&, std::size_t) {})
```

**说明：**  
机器人站立动作。

**参数：**
| 参数名 | 类型 | 默认值 | 说明 |
|:--|:--|:--|:--|
| `timeout_ms` | `int` | `0` | `0`: 异步模式；`> 0`: 同步模式，最大等待时间（毫秒） |
| `handler` | `WriteHandler` | 空回调 | 异步模式下命令发送结果回调函数；同步模式不使用 |

**返回值：**  
同 `SoftEmergencyStop`

**注意：**  
调用站立后，站立过程中会上报 `MOTION_STATUS_STAND_UP` 状态，站立完成后会根据模式自动进入 `MOTION_STATUS_GENERAL` 或 `MOTION_STATUS_IN_PLACE` 状态。

---

### LieDown - 卧倒

```cpp
std::error_code LieDown(int timeout_ms = 0,
                        WriteHandler handler = [](const std::error_code&, std::size_t) {})
```

**说明：**  
机器人卧倒动作。

**参数：**  
同 `StandUp`

**返回值：**  
同 `SoftEmergencyStop`

---

### Crawl - 匍匐

```cpp
std::error_code Crawl(int timeout_ms = 0,
                      WriteHandler handler = [](const std::error_code&, std::size_t) {})
```

**说明：**  
机器人匍匐动作。

**参数：**  
同 `StandUp`

**返回值：**  
同 `SoftEmergencyStop`

---

### Climb - 爬高台

```cpp
std::error_code Climb(int timeout_ms = 0,
                      WriteHandler handler = [](const std::error_code&, std::size_t) {})
```

**说明：**  
爬高台动作，仅在**通用模式**下使用。

**参数：**  
同 `StandUp`

**返回值：**  
同 `SoftEmergencyStop`

---

### Slim - 瘦身

```cpp
std::error_code Slim(int timeout_ms = 0,
                     WriteHandler handler = [](const std::error_code&, std::size_t) {})
```

**说明：**  
瘦身动作，仅在**通用模式**下使用。

**参数：**  
同 `StandUp`

**返回值：**  
同 `SoftEmergencyStop`

---

### Gait - 步态

```cpp
std::error_code Gait(int timeout_ms = 0,
                     WriteHandler handler = [](const std::error_code&, std::size_t) {})
```

**说明：**  
步态姿态，仅在**通用模式**下使用。

**参数：**  
同 `StandUp`

**返回值：**  
同 `SoftEmergencyStop`

---

### DSB - 过挡鼠板姿态

```cpp
std::error_code DSB(int timeout_ms = 0,
                     WriteHandler handler = [](const std::error_code&, std::size_t) {})
```

**说明：**  
过挡鼠板姿态，仅在**通用模式**下使用。

**参数：**  
同 `StandUp`

**返回值：**  
同 `SoftEmergencyStop`

---

### ReverseHeadTail - 调转头尾

```cpp
std::error_code ReverseHeadTail(int timeout_ms = 0,
                                 WriteHandler handler = [](const std::error_code&, std::size_t) {})
```

**说明：**  
调转机器人头尾方向。

**参数：**  
同 `StandUp`

**返回值：**  
同 `SoftEmergencyStop`

---

### SetMode - 设置模式

```cpp
std::error_code SetMode(int mode, int timeout_ms = 0,
                        WriteHandler handler = [](const std::error_code&, std::size_t) {})
```

**说明：**  
设置机器人运行模式，默认为通用模式。

**参数：**
| 参数名 | 类型 | 默认值 | 说明 |
|:--|:--|:--|:--|
| `mode` | `int` | - | `1`: 通用模式；`2`: 原地模式；`3`: 登阶模式 |
| `timeout_ms` | `int` | `0` | `0`: 异步模式；`> 0`: 同步模式，最大等待时间（毫秒） |
| `handler` | `WriteHandler` | 空回调 | 异步模式下命令发送结果回调函数；同步模式不使用 |

**返回值：**  
同 `SoftEmergencyStop`

---

### SetSpeed - 设置速度等级

```cpp
std::error_code SetSpeed(int speed_level, int timeout_ms = 0,
                         WriteHandler handler = [](const std::error_code&, std::size_t) {})
```

**说明：**  
设置速度等级，主要影响通用模式下 `Move` 命令的速度，默认为低速。

低速对Move接口的影响：

    forward_back：
        [-1.0, 1.0]对应[-1.0m/s , +1.0m/s]

    left_right：
        [-1.0, 1.0]对应[-0.5m/s , +0.5m/s]

    yaw：
        [-1.0, 1.0]对应[-1.5rad/s, +1.5rad/s]

中速对Move接口的影响：

    forward_back：
        [-1.0, 1.0]对应[-2.0m/s , +2.0m/s]

    left_right：
        当-1.0m/s < forward_back < 1.0m/s时，[-1.0, 1.0]对应[-0.5m/s , +0.5m/s]
        当forward_back> 1.0m/s 或者 forward_back< -1.0m/s时，限制为0m/s

    yaw：
        当-1.0m/s < forward_back < 1.0m/s时，[-1.0, 1.0]对应[-1.5rad/s, +1.5rad/s]
        当forward_back> 1.0m/s 或者forward_back< -1.0m/s时，[-1.0, 1.0]对应[-1.0rad/s, +1.0rad/s]

高速对Move接口的影响：

    forward_back：
        [-1.0, 1.0]对应[-3.0m/s , +3.0m/s]

    left_right：
        当-1.0m/s < forward_back < 1.0m/s时，[-1.0, 1.0]对应[-0.5m/s , +0.5m/s]
        当forward_back> 1.0m/s 或者 forward_back< -1.0m/s时，限制为0m/s

    yaw：
        当-1.0m/s < forward_back < 1.0m/s时，[-1.0, 1.0]对应[-1.5rad/s, +1.5rad/s]
        当forward_back> 1.0m/s 或者forward_back< -1.0m/s时，[-1.0, 1.0]对应[-1.0rad/s, +1.0rad/s]
        当forward_back> 2.0m/s 或者forward_back< -2.0m/s时，[-1.0, 1.0]对应[-0.5rad/s, +0.5rad/s]

**参数：**
| 参数名 | 类型 | 默认值 | 说明 |
|:--|:--|:--|:--|
| `speed_level` | `int` | - | `1`: 低速；`2`: 中速；`3`: 高速（范围 [1, 3]） |
| `timeout_ms` | `int` | `0` | `0`: 异步模式；`> 0`: 同步模式，最大等待时间（毫秒） |
| `handler` | `WriteHandler` | 空回调 | 异步模式下命令发送结果回调函数；同步模式不使用 |

**返回值：**  
同 `SoftEmergencyStop`

---

### Locked - 锁定

```cpp
std::error_code Locked(int timeout_ms = 0,
                       WriteHandler handler = [](const std::error_code&, std::size_t) {})
```

**说明：**  
锁定机器人，各个关节保持当前位置不动。

**参数：**  
同 `StandUp`

**返回值：**  
同 `SoftEmergencyStop`

**注意：**  
当下发切换到其他运动状态（站立、匍匐、卧倒）的命令时，会自动解锁并切换到相应动作状态。

---

## 灯光控制接口

### FrontLight - 前补光灯

```cpp
std::error_code FrontLight(bool on, int timeout_ms = 0,
                           WriteHandler handler = [](const std::error_code&, std::size_t) {})
```

**说明：**  
设置前补光灯，设置后自动模式关闭。

**参数：**
| 参数名 | 类型 | 默认值 | 说明 |
|:--|:--|:--|:--|
| `on` | `bool` | - | `true`: 开启；`false`: 关闭 |
| `timeout_ms` | `int` | `0` | `0`: 异步模式；`> 0`: 同步模式，最大等待时间（毫秒） |
| `handler` | `WriteHandler` | 空回调 | 异步模式下命令发送结果回调函数；同步模式不使用 |

**返回值：**  
同 `SoftEmergencyStop`

---

### BackLight - 后补光灯

```cpp
std::error_code BackLight(bool on, int timeout_ms = 0,
                          WriteHandler handler = [](const std::error_code&, std::size_t) {})
```

**说明：**  
设置后补光灯，设置后自动模式关闭。

**参数：**  
同 `FrontLight`

**返回值：**  
同 `SoftEmergencyStop`

---

### AutoModeLight - 自动补光灯模式

```cpp
std::error_code AutoModeLight(bool on, int timeout_ms = 0,
                              WriteHandler handler = [](const std::error_code&, std::size_t) {})
```

**说明：**  
设置自动补光灯模式。

**参数：**  
同 `FrontLight`

**返回值：**  
同 `SoftEmergencyStop`

---

## 运动与姿态控制

### Move - 移动

```cpp
std::error_code Move(float left_right, float forward_back, float yaw, 
                     int timeout_ms = 0,
                     WriteHandler handler = [](const std::error_code&, std::size_t) {})
```

**说明：**  
用在**通用模式**下进行移动，单位：百分比，根据**SpeedLevel**来限速。

最新的Move指令会维持1s。

**参数：**
| 参数名 | 类型 | 默认值 | 说明 |
|:--|:--|:--|:--|
| `left_right` | `float` | - | 左右平移速度，范围 [-1.0, 1.0]；正数左平移，负数右平移 |
| `forward_back` | `float` | - | 前后移动速度，范围 [-1.0, 1.0]；正数前进，负数后退 |
| `yaw` | `float` | - | 旋转速度，范围 [-1.0, 1.0]；正数左旋转，负数右旋转 |
| `timeout_ms` | `int` | `0` | `0`: 异步模式；`> 0`: 同步模式，最大等待时间（毫秒） |
| `handler` | `WriteHandler` | 空回调 | 异步模式下命令发送结果回调函数；同步模式不使用 |

**返回值：**  
- `std::errc::success`: 操作成功
- 常见失败错误码：
  - `std::errc::timed_out`: 操作超时
  - `std::errc::not_connected`: 未连接
  - `std::errc::operation_canceled`: 操作已取消
  - `std::errc::invalid_argument`: 参数无效

**注意：**  
异步模式：函数返回仅表示命令已发送，发送结果通过回调函数通知  
同步模式：函数返回即表示命令发送结果

---

### Turn - 翻滚

```cpp
std::error_code Turn(int direction, int timeout_ms = 0,
                     WriteHandler handler = [](const std::error_code&, std::size_t) {})
```

**说明：**  
原地左右翻滚动作（仅**原地模式**有效）。

**参数：**
| 参数名 | 类型 | 默认值 | 说明 |
|:--|:--|:--|:--|
| `direction` | `int` | - | 0: 恢复；1: 左翻滚；2: 右翻滚 |
| `timeout_ms` | `int` | `0` | `0`: 异步模式；`> 0`: 同步模式，最大等待时间（毫秒） |
| `handler` | `WriteHandler` | 空回调 | 异步模式下命令发送结果回调函数；同步模式不使用 |

**返回值：**  
- `std::errc::success`: 操作成功
- 常见失败错误码：
  - `std::errc::timed_out`: 操作超时
  - `std::errc::not_connected`: 未连接
  - `std::errc::operation_canceled`: 操作已取消

**注意：**  
异步模式：函数返回仅表示命令已发送，发送结果通过回调函数通知  
同步模式：函数返回即表示命令发送结果

---

### ControlHead - 控制头部

```cpp
std::error_code ControlHead(float left_right, float up_down, int timeout_ms = 0,
                            WriteHandler handler = [](const std::error_code&, std::size_t) {})
```

**说明：**  
控制机器人"左右探头"与"抬头/低头"动作（仅**原地模式**有效）。

**参数：**
| 参数名 | 类型 | 默认值 | 说明 |
|:--|:--|:--|:--|
| `left_right` | `float` | - | 范围 [-1.0, 1.0]；正数：左探头，负数：右探头；单位 rad/s |
| `up_down` | `float` | - | 范围 [-1.0, 1.0]；正数：抬头，负数：低头；单位 rad/s |
| `timeout_ms` | `int` | `0` | `0`: 异步模式；`> 0`: 同步模式，最大等待时间（毫秒） |
| `handler` | `WriteHandler` | 空回调 | 异步模式下命令发送结果回调函数；同步模式不使用 |

**返回值：**  
- `std::errc::success`: 操作成功
- 常见失败错误码：
  - `std::errc::timed_out`: 操作超时
  - `std::errc::not_connected`: 未连接
  - `std::errc::operation_canceled`: 操作已取消
  - `std::errc::invalid_argument`: 参数无效

**注意：**  
异步模式：函数返回仅表示命令已发送，发送结果通过回调函数通知  
同步模式：函数返回即表示命令发送结果

---

### HighLowStance - 高低站姿

```cpp
std::error_code HighLowStance(int stance, int timeout_ms = 0,
                              WriteHandler handler = [](const std::error_code&, std::size_t) {})
```

**说明：**  
设置高低站姿（仅**原地模式**有效）。

**参数：**
| 参数名 | 类型 | 默认值 | 说明 |
|:--|:--|:--|:--|
| `stance` | `int` | - | 0: 恢复；1: 高站姿；2: 低站姿 |
| `timeout_ms` | `int` | `0` | `0`: 异步模式；`> 0`: 同步模式，最大等待时间（毫秒） |
| `handler` | `WriteHandler` | 空回调 | 异步模式下命令发送结果回调函数；同步模式不使用 |

**返回值：**  
- `std::errc::success`: 操作成功
- 常见失败错误码：
  - `std::errc::timed_out`: 操作超时
  - `std::errc::not_connected`: 未连接
  - `std::errc::operation_canceled`: 操作已取消
  - `std::errc::invalid_argument`: 参数无效

**注意：**  
异步模式：函数返回仅表示命令已发送，发送结果通过回调函数通知  
同步模式：函数返回即表示命令发送结果

---

## 数据配置接口

### SetImuConfig - 配置IMU

```cpp
std::error_code SetImuConfig(int freq, int timeout_ms = 0,
                             WriteHandler handler = [](const std::error_code&, std::size_t) {})
```

**说明：**  
配置 IMU 上报频率，默认关闭。

**参数：**
| 参数名 | 类型 | 默认值 | 说明 |
|:--|:--|:--|:--|
| `freq` | `int` | - | 范围 [0, 100]；0 表示关闭上报 |
| `timeout_ms` | `int` | `0` | `0`: 异步模式；`> 0`: 同步模式，最大等待时间（毫秒） |
| `handler` | `WriteHandler` | 空回调 | 异步模式下命令发送结果回调函数；同步模式不使用 |

**返回值：**  
- `std::errc::success`: 操作成功
- 常见失败错误码：
  - `std::errc::timed_out`: 操作超时
  - `std::errc::not_connected`: 未连接
  - `std::errc::operation_canceled`: 操作已取消

**注意：**  
异步模式：函数返回仅表示命令已发送，发送结果通过回调函数通知  
同步模式：函数返回即表示命令发送结果

---

### SetLuxConfig - 配置光强值

```cpp
std::error_code SetLuxConfig(bool on, int timeout_ms = 0,
                             WriteHandler handler = [](const std::error_code&, std::size_t) {})
```

**说明：**  
配置光强值上报开关，默认关闭。配置后光强值以1Hz上报。

**参数：**
| 参数名 | 类型 | 默认值 | 说明 |
|:--|:--|:--|:--|
| `on` | `bool` | - | `true`: 开启；`false`: 关闭 |
| `timeout_ms` | `int` | `0` | `0`: 异步模式；`> 0`: 同步模式，最大等待时间（毫秒） |
| `handler` | `WriteHandler` | 空回调 | 异步模式下命令发送结果回调函数；同步模式不使用 |

**返回值：**  
- `std::errc::success`: 操作成功
- 常见失败错误码：
  - `std::errc::timed_out`: 操作超时
  - `std::errc::not_connected`: 未连接
  - `std::errc::operation_canceled`: 操作已取消

**注意：**  
异步模式：函数返回仅表示命令已发送，发送结果通过回调函数通知  
同步模式：函数返回即表示命令发送结果

---

### SetMcConfig - 配置运动数据

```cpp
std::error_code SetMcConfig(bool on, int timeout_ms = 0,
                            WriteHandler handler = [](const std::error_code&, std::size_t) {})
```

**说明：**  
配置运动数据上报开关，默认关闭。配置后运动数据以50Hz上报。

**参数：**
| 参数名 | 类型 | 默认值 | 说明 |
|:--|:--|:--|:--|
| `on` | `bool` | - | `true`: 开启；`false`: 关闭 |
| `timeout_ms` | `int` | `0` | `0`: 异步模式；`> 0`: 同步模式，最大等待时间（毫秒） |
| `handler` | `WriteHandler` | 空回调 | 异步模式下命令发送结果回调函数；同步模式不使用 |

**返回值：**  
- `std::errc::success`: 操作成功
- 常见失败错误码：
  - `std::errc::timed_out`: 操作超时
  - `std::errc::not_connected`: 未连接
  - `std::errc::operation_canceled`: 操作已取消

**注意：**  
异步模式：函数返回仅表示命令已发送，发送结果通过回调函数通知  
同步模式：函数返回即表示命令发送结果

---

### SetSpeedReportConfig - 配置速度数据

```cpp
std::error_code SetSpeedReportConfig(bool on, uint32_t frequency, int timeout_ms = 0,
                            WriteHandler handler = [](const std::error_code&, std::size_t) {})
```

**Description:**  
配置速度数据上报开关，默认关闭。配置完成后，将上报速度数据。

**Parameters:**
| Parameter Name | Type | Default Value | Description |
|:--|:--|:--|:--|
| `on` | `bool` | - | `true`: 开启; `false`: 关闭 |
| `frequency` | `uint32_t` | - | 上报频率. [1hz, 50hz] |
| `timeout_ms` | `int` | `0` | `0`: 异步模式；`> 0`: 同步模式，最大等待时间（毫秒） |
| `handler` | `WriteHandler` | 空回调 | 异步模式下命令发送结果回调函数；同步模式不使用 |

**返回值：**  
- `std::errc::success`: 操作成功
- 常见失败错误码：
  - `std::errc::timed_out`: 操作超时
  - `std::errc::not_connected`: 未连接
  - `std::errc::operation_canceled`: 操作已取消

**注意：**  
异步模式：函数返回仅表示命令已发送，发送结果通过回调函数通知  
同步模式：函数返回即表示命令发送结果

---

### TakeControl - 获取控制权

```cpp
std::error_code TakeControl(int timeout_ms = 0,
                            WriteHandler handler = [](const std::error_code&, std::size_t) {})
```

**说明：**  
获取机器人的控制权。如果当前 APP 在控制，则无法抢占接管。详见 [sdk控制权归属](sdk_control_ownership_zh.md)

**参数：**
| 参数名 | 类型 | 默认值 | 说明 |
|:--|:--|:--|:--|
| `timeout_ms` | `int` | `0` | `0`: 异步模式；`> 0`: 同步模式，最大等待时间（毫秒） |
| `handler` | `WriteHandler` | 空回调 | 异步模式下命令发送结果回调函数；同步模式不使用 |

**返回值：**  
- `std::errc::success`: 操作成功
- 常见失败错误码：
  - `std::errc::timed_out`: 操作超时
  - `std::errc::not_connected`: 未连接
  - `std::errc::operation_canceled`: 操作已取消

**注意：**  
异步模式：函数返回仅表示命令已发送，发送结果通过回调函数通知  
同步模式：函数返回即表示命令发送结果

---

### ReleaseControl - 释放控制权

```cpp
std::error_code ReleaseControl(int timeout_ms = 0,
                               WriteHandler handler = [](const std::error_code&, std::size_t) {})
```

**说明：**  
释放机器人的控制权。

**参数：**
| 参数名 | 类型 | 默认值 | 说明 |
|:--|:--|:--|:--|
| `timeout_ms` | `int` | `0` | `0`: 异步模式；`> 0`: 同步模式，最大等待时间（毫秒） |
| `handler` | `WriteHandler` | 空回调 | 异步模式下命令发送结果回调函数；同步模式不使用 |

**返回值：**  
- `std::errc::success`: 操作成功
- 常见失败错误码：
  - `std::errc::timed_out`: 操作超时
  - `std::errc::not_connected`: 未连接
  - `std::errc::operation_canceled`: 操作已取消

**注意：**  
异步模式：函数返回仅表示命令已发送，发送结果通过回调函数通知  
同步模式：函数返回即表示命令发送结果

---

### UpdateCameraBitrate - 更新摄像头码率

```cpp
  std::error_code UpdateCameraBitrate(
      CameraBitrateCmd cmd, int timeout_ms = 0,
      WriteHandler handler = [](const std::error_code&, std::size_t) {});
```

**说明：**  
更新摄像头码率。

**参数：**
| 参数名 | 类型 | 默认值 | 说明 |
|:--|:--|:--|:--|
| `cmd` | `CameraBitrateCmd` |  | 配置具体摄像头的码率 |
| `timeout_ms` | `int` | `0` | `0`: 异步模式；`> 0`: 同步模式，最大等待时间（毫秒） |
| `handler` | `WriteHandler` | 空回调 | 异步模式下命令发送结果回调函数；同步模式不使用 |

**返回值：**  
- `std::errc::success`: 操作成功
- 常见失败错误码：
  - `std::errc::timed_out`: 操作超时
  - `std::errc::not_connected`: 未连接
  - `std::errc::operation_canceled`: 操作已取消

**注意：**  
异步模式：函数返回仅表示命令已发送，发送结果通过回调函数通知  
同步模式：函数返回即表示命令发送结果

---

## 版本信息

### `const std::string& Version() const`

获取 SDK 版本号字符串。

---

### `const std::string& ProtocolVersion() const`

获取协议版本号字符串。

---

### `const std::string& SystemVersion() const`

获取系统版本号字符串。

---

## 示例代码

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


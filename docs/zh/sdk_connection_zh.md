# Robot SDK — 连接配置文档

## 概述

本文档描述 Robot SDK 中的连接状态枚举和连接配置参数。

---

## 命名空间

```cpp
namespace robot_sdk
```

---

## 连接状态枚举

### ConnectionState

```cpp
enum class ConnectionState : uint8_t
```

**说明：**  
描述 SDK 客户端与机器人之间的连接状态。

**枚举值：**

| 枚举值 | 整数值 | 说明 |
|:--|:--|:--|
| `DISCONNECTING` | 0 | 正在断开连接 |
| `DISCONNECTED` | 1 | 已断开连接 |
| `CONNECTING` | 2 | 正在连接 |
| `HANDSHAKING` | 3 | 正在握手 |
| `CONNECTED` | 4 | 已连接 |
| `RECONNECTING` | 5 | 正在重连 |

**使用示例：**

```cpp
#include "robot_sdk/sdk_client.hpp"
using namespace robot_sdk;

SDKClient client;
client.Connect("192.168.234.1", "8082", true);

auto state = client.GetConnectionState();
switch (state) {
    case ConnectionState::CONNECTED:
        std::cout << "已连接到机器人" << std::endl;
        break;
    case ConnectionState::CONNECTING:
        std::cout << "正在连接..." << std::endl;
        break;
    case ConnectionState::DISCONNECTED:
        std::cout << "连接已断开" << std::endl;
        break;
    default:
        break;
}
```

---

## 连接配置结构

### ConnectionConfig

```cpp
struct ConnectionConfig
```

**说明：**  
配置 SDK 客户端的连接参数和重连行为。

**成员变量：**

| 成员名 | 类型 | 默认值 | 说明 |
|:--|:--|:--|:--|
| `connect_timeout_ms` | `int` | `5000` | 连接超时时间（毫秒），最小值 500ms |
| `auto_reconnect` | `bool` | `false` | 是否启用自动重连 |
| `reconnect_interval_ms` | `int` | `1000` | 重连间隔时间（毫秒），最小值 500ms |

---

### 成员详细说明

#### connect_timeout_ms

**类型：** `int`  
**默认值：** `5000`  
**说明：**  
连接操作的超时时间，单位为毫秒。如果在此时间内未能成功建立连接，连接操作将失败。

**限制：**
- 最小值：500ms
- 建议值：3000-10000ms，取决于网络环境

---

#### auto_reconnect

**类型：** `bool`  
**默认值：** `false`  
**说明：**  
是否启用自动重连功能。当启用时，如果连接意外断开，SDK 将自动尝试重新连接。

**行为：**
- `false`: 禁用自动重连，连接断开后需手动调用 `Connect()`
- `true`: 启用自动重连，连接断开后 SDK 自动尝试重连

**注意：**
- 自动重连仅在意外断开时触发，主动调用 `Disconnect()` 不会触发重连
- 重连期间连接状态为 `ConnectionState::RECONNECTING`

---

#### reconnect_interval_ms

**类型：** `int`  
**默认值：** `1000`  
**说明：**  
自动重连的间隔时间，单位为毫秒。仅在 `auto_reconnect` 为 `true` 时有效。

**限制：**
- 最小值：500ms
- 建议值：1000-5000ms

---

## 传输协议类型

### TransportProtocol

```cpp
enum class TransportProtocol
```

**说明：**  
SDK 支持的传输协议类型。

**枚举值：**

| 枚举值 | 说明 |
|:--|:--|
| `WebSocket` | WebSocket 协议（默认） |
| `Udp` | UDP 协议 |

**使用场景：**
- **WebSocket**: 适用于大多数场景，提供可靠的双向通信
- **UDP**: 适用于低延迟场景，但不保证数据可靠性

---

## 使用示例

### 示例 1：基本连接配置

```cpp
#include "robot_sdk/sdk_client.hpp"
using namespace robot_sdk;

int main() {
    // 使用默认配置
    SDKClient client;
    
    auto ec = client.Connect("192.168.234.1", "8082", true);
    if (!ec) {
        std::cout << "连接成功" << std::endl;
    }
    
    return 0;
}
```

### 示例 2：自定义连接超时

```cpp
#include "robot_sdk/sdk_client.hpp"
using namespace robot_sdk;

int main() {
    // 配置10秒连接超时
    ConnectionConfig config;
    config.connect_timeout_ms = 10000;
    
    SDKClient client([](const std::error_code& ec) {}, config);
    
    auto ec = client.Connect("192.168.234.1", "8082", true);
    if (!ec) {
        std::cout << "连接成功" << std::endl;
    }
    
    return 0;
}
```

### 示例 3：启用自动重连

```cpp
#include "robot_sdk/sdk_client.hpp"
#include <iostream>

using namespace robot_sdk;

int main() {
    // 配置自动重连
    ConnectionConfig config;
    config.auto_reconnect = true;           // 启用自动重连
    config.reconnect_interval_ms = 2000;    // 每2秒尝试重连一次
    config.connect_timeout_ms = 5000;       // 5秒连接超时
    
    // 设置错误回调，监控连接状态
    SDKClient client([](const std::error_code& ec) {
        if (ec) {
            std::cerr << "SDK 错误: " << ec.message() << std::endl;
        }
    }, config);
    
    // 连接机器人
    auto ec = client.Connect("192.168.234.1", "8082", true);
    if (ec) {
        std::cerr << "初始连接失败: " << ec.message() << std::endl;
        return -1;
    }
    
    std::cout << "已连接，自动重连已启用" << std::endl;
    
    // 程序继续运行，即使连接断开也会自动重连
    // ...
    
    return 0;
}
```

### 示例 4：使用 UDP 协议

```cpp
#include "robot_sdk/sdk_client.hpp"
using namespace robot_sdk;

int main() {
    ConnectionConfig config;
    config.connect_timeout_ms = 3000;
    
    // 使用 UDP 协议
    SDKClient client(
        [](const std::error_code& ec) {},
        config,
        TransportProtocol::Udp
    );
    
    auto ec = client.Connect("192.168.234.1", "8082", true);
    if (!ec) {
        std::cout << "通过 UDP 连接成功" << std::endl;
    }
    
    return 0;
}
```

### 示例 5：监控连接状态变化

```cpp
#include "robot_sdk/sdk_client.hpp"
#include <iostream>
#include <thread>
#include <chrono>

using namespace robot_sdk;

int main() {
    ConnectionConfig config;
    config.auto_reconnect = true;
    config.reconnect_interval_ms = 1000;
    
    SDKClient client([](const std::error_code& ec) {}, config);
    
    // 异步连接
    client.Connect("192.168.234.1", "8082", false, 
                   [](const std::error_code& ec) {
        if (!ec) {
            std::cout << "连接成功" << std::endl;
        }
    });
    
    // 定期检查连接状态
    for (int i = 0; i < 60; ++i) {
        auto state = client.GetConnectionState();
        
        std::cout << "当前状态: ";
        switch (state) {
            case ConnectionState::DISCONNECTED:
                std::cout << "已断开" << std::endl;
                break;
            case ConnectionState::CONNECTING:
                std::cout << "正在连接" << std::endl;
                break;
            case ConnectionState::CONNECTED:
                std::cout << "已连接" << std::endl;
                break;
            case ConnectionState::RECONNECTING:
                std::cout << "正在重连" << std::endl;
                break;
            default:
                std::cout << "其他状态" << std::endl;
        }
        
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    return 0;
}
```

---

## 最佳实践

### 连接超时设置

- **局域网环境**: 3000-5000ms
- **无线网络**: 5000-10000ms
- **网络不稳定**: 10000-15000ms

### 自动重连配置

**推荐配置：**
```cpp
ConnectionConfig config;
config.auto_reconnect = true;
config.reconnect_interval_ms = 2000;  // 2秒重连间隔
config.connect_timeout_ms = 5000;     // 5秒连接超时
```

**注意事项：**
1. 重连间隔不宜过短（建议 ≥ 1000ms），避免频繁重连消耗资源
2. 连接超时应大于网络往返时间的 2-3 倍
3. 在生产环境建议启用自动重连以提高系统稳定性
4. 通过错误回调函数监控连接异常情况

---

## 相关文档

- [SDKClient API 文档](sdk_client_api_zh.md) - 客户端接口详细说明
- [状态定义文档](sdk_state_zh.md) - 连接状态和运动状态详解
- [类型定义文档](sdk_type_zh.md) - 数据类型和结构体定义

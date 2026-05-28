# Robot SDK — Connection Configuration Documentation

## Overview

This document describes the connection state enumeration and connection configuration parameters in Robot SDK.

---

## Namespace

```cpp
namespace robot_sdk
```

---

## Connection State Enumeration

### ConnectionState

```cpp
enum class ConnectionState : uint8_t
```

**Description:**  
Describes the connection state between the SDK client and the robot.

**Enumeration Values:**

| Enum Value | Integer Value | Description |
|:--|:--|:--|
| `DISCONNECTING` | 0 | Disconnecting |
| `DISCONNECTED` | 1 | Disconnected |
| `CONNECTING` | 2 | Connecting |
| `HANDSHAKING` | 3 | Handshaking |
| `CONNECTED` | 4 | Connected |
| `RECONNECTING` | 5 | Reconnecting |

**Usage Example:**

```cpp
#include "robot_sdk/sdk_client.hpp"
using namespace robot_sdk;

SDKClient client;
client.Connect("192.168.234.1", "8081", true);

auto state = client.GetConnectionState();
switch (state) {
    case ConnectionState::CONNECTED:
        std::cout << "Connected to robot" << std::endl;
        break;
    case ConnectionState::CONNECTING:
        std::cout << "Connecting..." << std::endl;
        break;
    case ConnectionState::DISCONNECTED:
        std::cout << "Connection disconnected" << std::endl;
        break;
    default:
        break;
}
```

---

## Connection Configuration Structure

### ConnectionConfig

```cpp
struct ConnectionConfig
```

**Description:**  
Configures SDK client connection parameters and reconnection behavior.

**Member Variables:**

| Member Name | Type | Default Value | Description |
|:--|:--|:--|:--|
| `connect_timeout_ms` | `int` | `5000` | Connection timeout in milliseconds, minimum value 500ms |
| `auto_reconnect` | `bool` | `false` | Whether to enable auto-reconnection |
| `reconnect_interval_ms` | `int` | `1000` | Reconnection interval in milliseconds, minimum value 500ms |

---

### Member Detailed Description

#### connect_timeout_ms

**Type:** `int`  
**Default Value:** `5000`  
**Description:**  
The timeout duration for connection operations, measured in milliseconds. If a successful connection cannot be established within this time, the connection operation will fail.

**Limitations:**
- Minimum value: 500ms
- Recommended value: 3000-10000ms, depending on network environment

---

#### auto_reconnect

**Type:** `bool`  
**Default Value:** `false`  
**Description:**  
Whether to enable auto-reconnection. When enabled, if the connection unexpectedly disconnects, the SDK will automatically attempt to reconnect.

**Behavior:**
- `false`: Auto-reconnection disabled; connection must be reestablished by manually calling `Connect()`
- `true`: Auto-reconnection enabled; SDK automatically attempts to reconnect on unexpected disconnection

**Note:**
- Auto-reconnection is only triggered on unexpected disconnection, not on intentional `Disconnect()` calls
- During reconnection attempts, the connection state is `ConnectionState::RECONNECTING`

---

#### reconnect_interval_ms

**Type:** `int`  
**Default Value:** `1000`  
**Description:**  
The interval time for auto-reconnection attempts, measured in milliseconds. Only effective when `auto_reconnect` is `true`.

**Limitations:**
- Minimum value: 500ms
- Recommended value: 1000-5000ms

---

## Transport Protocol Type

### TransportProtocol

```cpp
enum class TransportProtocol
```

**Description:**  
Transport protocol types supported by the SDK.

**Enumeration Values:**

| Enum Value | Description |
|:--|:--|
| `WebSocket` | WebSocket protocol (default) |
| `Udp` | UDP protocol |

**Use Cases:**
- **WebSocket**: Suitable for most scenarios, providing reliable bidirectional communication
- **UDP**: Suitable for low-latency scenarios, but does not guarantee data reliability

---

## Usage Examples

### Example 1: Basic Connection Configuration

```cpp
#include "robot_sdk/sdk_client.hpp"
using namespace robot_sdk;

int main() {
    // Using default configuration
    SDKClient client;
    
    auto ec = client.Connect("192.168.234.1", "8081", true);
    if (!ec) {
        std::cout << "Connection successful" << std::endl;
    }
    
    return 0;
}
```

### Example 2: Custom Connection Timeout

```cpp
#include "robot_sdk/sdk_client.hpp"
using namespace robot_sdk;

int main() {
    // Configure 10-second connection timeout
    ConnectionConfig config;
    config.connect_timeout_ms = 10000;
    
    SDKClient client([](const std::error_code& ec) {}, config);
    
    auto ec = client.Connect("192.168.234.1", "8081", true);
    if (!ec) {
        std::cout << "Connection successful" << std::endl;
    }
    
    return 0;
}
```

### Example 3: Enable Auto-Reconnection

```cpp
#include "robot_sdk/sdk_client.hpp"
#include <iostream>

using namespace robot_sdk;

int main() {
    // Configure auto-reconnection
    ConnectionConfig config;
    config.auto_reconnect = true;           // Enable auto-reconnection
    config.reconnect_interval_ms = 2000;    // Attempt to reconnect every 2 seconds
    config.connect_timeout_ms = 5000;       // 5-second connection timeout
    
    // Set error callback to monitor connection status
    SDKClient client([](const std::error_code& ec) {
        if (ec) {
            std::cerr << "SDK Error: " << ec.message() << std::endl;
        }
    }, config);
    
    // Connect to robot
    auto ec = client.Connect("192.168.234.1", "8081", true);
    if (ec) {
        std::cerr << "Initial connection failed: " << ec.message() << std::endl;
        return -1;
    }
    
    std::cout << "Connected with auto-reconnection enabled" << std::endl;
    
    // Program continues running, connection will auto-reconnect on disconnection
    // ...
    
    return 0;
}
```

### Example 4: Use UDP Protocol

```cpp
#include "robot_sdk/sdk_client.hpp"
using namespace robot_sdk;

int main() {
    ConnectionConfig config;
    config.connect_timeout_ms = 3000;
    
    // Use UDP protocol
    SDKClient client(
        [](const std::error_code& ec) {},
        config,
        TransportProtocol::Udp
    );
    
    auto ec = client.Connect("192.168.234.1", "8081", true);
    if (!ec) {
        std::cout << "Connected via UDP successfully" << std::endl;
    }
    
    return 0;
}
```

### Example 5: Monitor Connection State Changes

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
    
    // Asynchronous connection
    client.Connect("192.168.234.1", "8081", false, 
                   [](const std::error_code& ec) {
        if (!ec) {
            std::cout << "Connection successful" << std::endl;
        }
    });
    
    // Check connection status periodically
    for (int i = 0; i < 60; ++i) {
        auto state = client.GetConnectionState();
        
        std::cout << "Current state: ";
        switch (state) {
            case ConnectionState::DISCONNECTED:
                std::cout << "Disconnected" << std::endl;
                break;
            case ConnectionState::CONNECTING:
                std::cout << "Connecting" << std::endl;
                break;
            case ConnectionState::CONNECTED:
                std::cout << "Connected" << std::endl;
                break;
            case ConnectionState::RECONNECTING:
                std::cout << "Reconnecting" << std::endl;
                break;
            default:
                std::cout << "Other state" << std::endl;
        }
        
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    return 0;
}
```

---

## Best Practices

### Connection Timeout Configuration

- **LAN environment**: 3000-5000ms
- **Wireless network**: 5000-10000ms
- **Unstable network**: 10000-15000ms

### Auto-Reconnection Configuration

**Recommended Configuration:**
```cpp
ConnectionConfig config;
config.auto_reconnect = true;
config.reconnect_interval_ms = 2000;  // 2-second reconnection interval
config.connect_timeout_ms = 5000;     // 5-second connection timeout
```

**Notes:**
1. Reconnection interval should not be too short (recommended ≥ 1000ms) to avoid excessive resource consumption
2. Connection timeout should be 2-3 times the network round-trip time
3. It is recommended to enable auto-reconnection in production environments to improve system stability
4. Monitor connection exceptions through error callback functions

---

## Related Documentation

- [SDKClient API Documentation](sdk_client_api_en.md) - Detailed client interface documentation
- [State Definition Documentation](sdk_state_en.md) - Connection state and motion state details
- [Type Definition Documentation](sdk_type_en.md) - Data types and structure definitions


#pragma once
#include <cstdint>

namespace robot_sdk {
/// @brief 连接状态枚举
enum class ConnectionState : uint8_t {
  DISCONNECTING = 0,  ///< 断开连接中
  DISCONNECTED = 1,   ///< 断开连接
  CONNECTING = 2,     ///< 连接中
  HANDSHAKING = 3,    ///< 握手中
  CONNECTED = 4,      ///< 已连接
  RECONNECTING = 5,   ///< 重连中
};

struct ConnectionConfig {
  // 连接配置
  int connect_timeout_ms =
      5000;  ///< 连接超时时间，单位：毫秒，最小500ms，默认值5000ms

  // 重连配置
  bool auto_reconnect = false;  ///< 是否自动重连，默认值false
  int reconnect_interval_ms =
      1000;  ///< 重连间隔时间，单位：毫秒, 最小500ms，默认值1000ms
};
}  // namespace robot_sdk
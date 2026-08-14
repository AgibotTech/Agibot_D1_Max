
#pragma once
#include <cstdint>

namespace robot_sdk {
/// @brief Connection state enumeration.
enum class ConnectionState : uint8_t {
  DISCONNECTING = 0,  ///< Disconnecting.
  DISCONNECTED = 1,   ///< Disconnected.
  CONNECTING = 2,     ///< Connecting.
  HANDSHAKING = 3,    ///< Handshaking.
  CONNECTED = 4,      ///< Connected.
  RECONNECTING = 5,   ///< Reconnecting.
};

struct ConnectionConfig {
  // Connection configuration.
  int connect_timeout_ms = 5000;  ///< Connection timeout in milliseconds.
                                  ///< Minimum: 500ms. Default: 5000ms.

  // Reconnection configuration.
  bool auto_reconnect =
      false;  ///< Whether to enable auto-reconnection. Default: false.
  int reconnect_interval_ms = 1000;  ///< Reconnection interval in milliseconds.
                                     ///< Minimum: 500ms. Default: 1000ms.
};
}  // namespace robot_sdk
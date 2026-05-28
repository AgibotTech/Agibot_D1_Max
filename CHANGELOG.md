# Changelog

This document records the feature updates, improvements, and bug fixes for each version of the Robot SDK.

---
## [v0.1.1] - 2026

### 📝 Notes
- Improve Documentation.
- The default transport layer protocol has been changed from WebSocket to UDP.
- Add original communication protocol document.
### ✨ New Features
- Add over the mouse barrier posture.(DSB)


---
## [v0.1.0] - 2026

### ✨ New Features
- Add timestamp data to MC data.
- Add joint state reporting.

### 🐛 Bug Fixes
- Fixed the issue of empty speed data.

---
## [v0.0.9] - 2026

### ✨ New Features
- Replaced the previous motor temperature structure with a joint-indexed temperature mapping. The SDK now represents temperatures as a mapping from joint identifier to temperature in degrees Celsius, enabling flexible reporting for robots with variable joint sets.
- Add a configurable frequency speed data reporting interface.

### ⚠️ Migration Note
- This change is backward-incompatible for code that accessed fixed motor temperature fields. Applications should be updated to read from the new mapping (iterate the mapping or query by joint key).
- Example migration:
	- Old: `data.motor_temp.fl1`
	- New: `data.joint_temps["fl1"]`
- The adaptation is straightforward and typically requires only a small update in application code. No change to the communication protocol is required.

---
## [v0.0.8] - 2026

### ✨ New Features
- English documentation support added

---

## [v0.0.7] - 2026

### ✨ New Features
- Added dynamic camera bitrate adjustment interface

---

## [v0.0.6] - 2026

### ✨ New Features
- Added control ownership switching interface and related examples

### 🐛 Bug Fixes
- Fixed IMU accelerometer/gyroscope inversion issue

---

## [v0.0.5] - 2026

### ✨ New Features
- Added gait posture interface (Gait(...))
- Added slim (body compress) posture interface (Slim(...))
- Added auto-reconnection configuration interface after disconnection
- Added interface to get current connection state
- Added auto-reconnection/manual reconnection examples
- Added callback parameter to get send results in asynchronous interfaces

### 🔧 Improvements
- Optimized SDKClient destructor handling
- Optimized connection/disconnection mechanism

---

## [v0.0.4] - 2025

### ✨ New Features
- Added interface to get system version number
- Added UDP communication protocol support

### 🔧 Improvements
- Improved error message display on connection failure
- Internal adaptation to new communication protocol version

---

## [v0.0.3] - 2025

### ✨ New Features
- Added stair-climbing mode interface
- Added high platform climbing interface in motion mode
- Added mode state query interface

---

## [v0.0.2] - 2025

### ✨ New Features
- Provided basic robot control interfaces
- Supported robot state data callbacks
- Supported fault information callbacks

---

## [v0.0.1] - 2025

### 📝 Notes
- Initial pre-release version

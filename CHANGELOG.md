# Changelog

This document records the feature updates, improvements, and bug fixes for each version of the Robot SDK.

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

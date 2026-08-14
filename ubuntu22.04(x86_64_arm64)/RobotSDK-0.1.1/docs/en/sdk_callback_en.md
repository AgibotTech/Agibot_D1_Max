# SDK Callback Interface Documentation

This document defines the data callback and control command callback interfaces in the Robot SDK.  
Namespace: `robot_sdk`

---

## Interface Overview

- [`IDataCallback`](#idatacallback): Data reporting callback interface  
- [`IControlCallback`](#icontrolcallback): Control command callback interface (non-blocking mode)

---

## IDataCallback

Users need to implement this interface as required to receive data from the robot system.

> ⚠️ Callback functions must be **lightweight** and cannot perform time-consuming operations.  
> In callback functions, it is recommended to perform only fast operations such as data copying and validation.  
> If time-consuming tasks need to be performed (such as database writes, file I/O, complex calculations, network transmission, etc.), please **copy the data to an independent thread or thread pool** within the callback function for processing.

### Definition

```cpp
class ROBOT_EXPORT_API IDataCallback {
 public:
  virtual void OnImuData(const ImuData& data) {}
  virtual void OnLuxData(const LuxData& data) {}
  virtual void OnMcData(const MotionData& data) {}
  virtual void OnSpeedData(const SpeedData& data) {}
  virtual void OnRobotStateData(const RobotState& data) {}
  virtual void OnFaultData(const FaultDatas& data) {}
  virtual void OnControlLost(const ControlLostInfo& info) {}
  virtual void OnControlAvailable(const ControlAvailableInfo& info) {}

  virtual ~IDataCallback() = default;
};
```

---

### Callback List

#### 🧭 `OnImuData`
- **Description**: IMU data callback (reported at fixed frequency after configuration)  
- **Parameter**: `const ImuData& data` — IMU data structure  
- **Call Frequency**: Reported at configured frequency  

#### 💡 `OnLuxData`
- **Description**: Light intensity data callback (reported at fixed frequency of 1Hz after configuration)  
- **Parameter**: `const LuxData& data` — Light intensity data structure  

#### 🤖 `OnMcData`
- **Description**: Robot motion data callback (reported at fixed frequency of 50Hz after configuration)  
- **Parameter**: `const MotionData& data` — Motion data structure  

#### 🤖 `OnSpeedData`
- **Description**: Robot speed data callback 
- **Parameter**: `const SpeedData& data` — Speed data structure  

#### 🤖 `OnRobotStateData`
- **Description**: Robot state data callback (actively reported at 1Hz)  
- **Parameter**: `const RobotState& data` — Robot state structure  

#### ⚠️ `OnFaultData`
- **Description**: Fault information data callback (actively reported when a fault occurs)  
- **Parameter**: `const FaultDatas& data` — Collection of fault data  

#### 🔓 `OnControlLost`
- **Description**: Control lost callback  
- **Parameter**: `const ControlLostInfo& info` — Control lost information  

#### 🔒 `OnControlAvailable`
- **Description**: Control available callback  
- **Parameter**: `const ControlAvailableInfo& info` — Control available information  

---

## IControlCallback

This interface is used to receive callbacks confirming that the robot has "received" control commands.  
Only used in **non-blocking mode**.

### Definition

```cpp
class ROBOT_EXPORT_API IControlCallback {
 public:
  virtual void OnSoftEmergencyStop(bool on) {}
  virtual void OnStandUp() {}
  virtual void OnLieDown() {}
  virtual void OnCrawl() {}
  virtual void OnClimb() {}
  virtual void OnSlim() {}
  virtual void OnGait() {}
  virtual void OnReverseHeadTail() {}
  virtual void OnMode(int mode) {}
  virtual void OnSpeed(int speed_level) {}
  virtual void OnLocked() {}
  virtual void OnFrontLight(bool on) {}
  virtual void OnBackLight(bool on) {}
  virtual void OnAutoModeLight(bool on) {}
  virtual void OnLuxConfig(bool on) {}
  virtual void OnImuConfig(int freq) {}
  virtual void OnMcConfig(bool on) {}
  virtual void OnSpeedReportConfig(bool on, uint32_t frequency) {}
  virtual void OnTakeControlAck(const TakeControlAck& ack) {}
  virtual void OnReleaseControlAck(const ReleaseControlAck& ack) {}
  virtual void OnUpdateCameraBitrateAck(const CameraBitrateAck& ack) {}

  virtual ~IControlCallback() = default;
};
```

---

### Callback List

| Callback Function | Description | Parameter |
|-----------|------|------|
| `OnSoftEmergencyStop(bool on)` | Emergency stop command received | `on = true`: activate emergency stop; `on = false`: deactivate emergency stop |
| `OnStandUp()` | Stand up command received | — |
| `OnLieDown()` | Lie down command received | — |
| `OnCrawl()` | Crawl command received | — |
| `OnClimb()` | Climb command received | — |
| `OnSlim()` | Slim command received | — |
| `OnGait()` | Gait command received | — |
| `OnReverseHeadTail()` | Reverse head/tail command received | — |
| `OnMode(int mode)` | Mode switch command received | Mode number |
| `OnSpeed(int speed_level)` | Speed level switch command received | Speed level |
| `OnLocked()` | Lock command received | — |
| `OnFrontLight(bool on)` | Front fill light command received | `true`: activate; `false`: deactivate |
| `OnBackLight(bool on)` | Back fill light command received | `true`: activate; `false`: deactivate |
| `OnAutoModeLight(bool on)` | Auto fill light mode command received | `true`: activate; `false`: deactivate |
| `OnLuxConfig(bool on)` | Light intensity configuration command received | `true`: enable; `false`: disable |
| `OnImuConfig(int freq)` | IMU configuration command received | Frequency value |
| `OnMcConfig(bool on)` | Motion data configuration command received | `true`: enable; `false`: disable |
| `OnSpeedReportConfig(bool on, uint32_t frequency)` | Speed data configuration command received | `true`: enable; `false`: disable; `frequency`: frequency|
| `OnTakeControlAck(const TakeControlAck& ack)` | Take control command acknowledgment received | Acknowledgment information |
| `OnReleaseControlAck(const ReleaseControlAck& ack)` | Release control command acknowledgment received | Acknowledgment information |
| `OnUpdateCameraBitrateAck(const CameraBitrateAck& ack)` | Camera bitrate update command received | Acknowledgment information, updated bitrate |

---

## Implementation Example

```cpp
#include "robot_sdk/sdk_callback.hpp"

class MyDataCallback : public robot_sdk::IDataCallback {
 public:
  void OnImuData(const ImuData& data) override {
    // Lightweight processing, e.g., caching
  }
};

class MyControlCallback : public robot_sdk::IControlCallback {
 public:
  void OnStandUp() override {
    // Handle stand up command acknowledgment
  }
};
```

---

## Notes

- Do not perform time-consuming tasks in callbacks.
- If time-consuming processing is needed, copy the data and submit it to an independent thread in the callback.

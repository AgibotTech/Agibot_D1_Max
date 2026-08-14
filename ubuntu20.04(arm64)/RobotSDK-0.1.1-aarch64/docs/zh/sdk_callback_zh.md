# SDK 回调接口文档

本文档定义了机器人 SDK 中的数据回调与控制命令回调接口。  
命名空间：`robot_sdk`

---

## 接口概览

- [`IDataCallback`](#idatacallback)：数据上报回调接口  
- [`IControlCallback`](#icontrolcallback)：控制命令回调接口（非阻塞模式）

---

## IDataCallback

用户需根据需要实现此接口以接收来自机器人系统的数据。

> ⚠️ 回调函数必须**轻量级**，不能执行耗时操作。  
> 在回调函数中仅建议进行数据复制、校验等快速操作。  
> 若需执行耗时任务（如数据库写入、文件I/O、复杂计算、网络发送等），请在回调函数内部**将数据复制到独立线程或线程池中**处理。

### 定义

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

### 回调列表

#### 🧭 `OnImuData`
- **描述**：IMU 数据回调（配置后定频上报）  
- **参数**：`const ImuData& data` — IMU 数据结构  
- **调用频率**：按配置频率上报  

#### 💡 `OnLuxData`
- **描述**：光照强度数据回调（配置后定频 1Hz 上报）  
- **参数**：`const LuxData& data` — 光照强度数据结构  

#### 🤖 `OnMcData`
- **描述**：机器人运动数据回调（配置后定频 50Hz 上报）  
- **参数**：`const MotionData& data` — 运动数据结构  

#### 🤖 `OnSpeedData`
- **描述**：机器人速度数据回调
- **参数**：`const SpeedData& data` — 速度数据结构  

#### 🤖 `OnRobotStateData`
- **描述**：机器人状态数据回调（1Hz 主动上报）  
- **参数**：`const RobotState& data` — 机器人状态结构  

#### ⚠️ `OnFaultData`
- **描述**：故障信息数据回调（发生故障时主动上报）  
- **参数**：`const FaultDatas& data` — 故障数据集合  

#### 🔓 `OnControlLost`
- **描述**：控制权丢失回调  
- **参数**：`const ControlLostInfo& info` — 控制权丢失信息  

#### 🔒 `OnControlAvailable`
- **描述**：控制权可用回调  
- **参数**：`const ControlAvailableInfo& info` — 控制权可用信息  
---

## IControlCallback

该接口用于接收机器人确认“已收到”控制命令的回调。  
仅在**非阻塞模式**下使用。

### 定义

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

### 回调列表

| 回调函数 | 说明 | 参数 |
|-----------|------|------|
| `OnSoftEmergencyStop(bool on)` | 已收到急停命令 | `on = true` 开启急停；`on = false` 关闭急停 |
| `OnStandUp()` | 已收到站立命令 | — |
| `OnLieDown()` | 已收到卧倒命令 | — |
| `OnCrawl()` | 已收到匍匐命令 | — |
| `OnClimb()` | 已收到爬高台命令 | — |
| `OnSlim()` | 已收到瘦身命令 | — |
| `OnGait()` | 已收到步态命令 | — |
| `OnReverseHeadTail()` | 已收到调转头尾命令 | — |
| `OnMode(int mode)` | 已收到模式切换命令 | 模式编号 |
| `OnSpeed(int speed_level)` | 已收到速度切换命令 | 速度档位 |
| `OnLocked()` | 已收到锁定命令 | — |
| `OnFrontLight(bool on)` | 已收到前补光灯命令 | `true` 开启，`false` 关闭 |
| `OnBackLight(bool on)` | 已收到后补光灯命令 | `true` 开启，`false` 关闭 |
| `OnAutoModeLight(bool on)` | 已收到自动补光灯模式命令 | `true` 开启，`false` 关闭 |
| `OnLuxConfig(bool on)` | 已收到光强值配置命令 | `true` 开启，`false` 关闭 |
| `OnImuConfig(int freq)` | 已收到 IMU 配置命令 | 频率值 |
| `OnMcConfig(bool on)` | 已收到运动数据配置命令 | `true` 开启，`false` 关闭 |
| `OnSpeedReportConfig(bool on, uint32_t frequency)` | 已收到速度数据配置命令 | `true` 开启，`false` 关闭；`frequency` 频率值|
| `OnTakeControlAck(const TakeControlAck& ack)` | 已收到获取控制权命令应答 | 应答信息 |
| `OnReleaseControlAck(const ReleaseControlAck& ack)` | 已收到释放控制权命令应答 | 应答信息 |
| `OnUpdateCameraBitrateAck(const CameraBitrateAck& ack)` | 已收到码率更新命令 | 应答信息，更新后的码率 |

---

## 实现示例

```cpp
#include "robot_sdk/sdk_callback.hpp"

class MyDataCallback : public robot_sdk::IDataCallback {
 public:
  void OnImuData(const ImuData& data) override {
    // 轻量处理，例如缓存
  }
};

class MyControlCallback : public robot_sdk::IControlCallback {
 public:
  void OnStandUp() override {
    // 收到站立命令应答
  }
};
```

---

## 注意事项

- 不要在回调中执行耗时任务。
- 若需要耗时处理，请在回调中将数据复制并投递至独立线程。
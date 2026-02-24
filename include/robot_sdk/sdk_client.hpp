#pragma once

#include <functional>
#include <memory>
#include <string>
#include <system_error>

#include "robot_sdk/sdk_callback.hpp"
#include "robot_sdk/sdk_connection.hpp"
#include "robot_sdk/sdk_export.hpp"
#include "robot_sdk/sdk_type.hpp"

namespace robot_sdk {

/**
 * @brief SDKClient，提供连接、断开和命令发送接口。
 */
class ROBOT_EXPORT_API SDKClient {
 public:
  using ConnectHandler = std::function<void(const std::error_code&)>;
  using DisConnectHandler = std::function<void(const std::error_code&)>;
  using ErrorHandler = std::function<void(const std::error_code&)>;
  using WriteHandler = std::function<void(const std::error_code&, std::size_t)>;

  /// @brief 构造函数
  /// @param error_callback SDK内部通信时异常回调
  /// @param connection_config 连接配置参数
  /// @param type 传输协议类型，默认Websocket
  SDKClient(
      ErrorHandler error_callback = [](const std::error_code&) {},
      ConnectionConfig connection_config = ConnectionConfig(),
      TransportProtocol type = TransportProtocol::WebSocket);
  ~SDKClient();

  /// @brief 连接Robot
  /// @param ip 目标IP
  /// @param port 目标端口
  /// @param block false:异步模式连接; true:同步模式连接;
  /// @param handler 异步模式连接完成后调用;同步模式不用此参数;
  /// @return std::error_code
  /// @retval std::errc::success 操作成功
  ///       常见失败错误码包括：
  ///       std::errc::operation_in_progress 操作在进行中
  ///       std::errc::operation_canceled 操作已取消
  ///       std::errc::already_connected 已经连接
  ///       std::errc::connect_refused 连接被拒绝
  ///       robot_sdk::errc::ControlledDenial 控制拒绝：已有其他客户端连接机器
  ///       robot_sdk::errc::ProtocolMismatch 协议不匹配：与机器协议版本不兼容
  ///       robot_sdk::errc::ShakeHandFailed 握手失败：握手过程中出现错误
  /// @note
  /// 异步模式下，Connect函数返回时仅表示连接请求已发出，回调函数将在连接完成后被调用；同步模式下，Connect函数返回时即表示连接结果。
  std::error_code Connect(
      std::string ip, std::string port, bool block = false,
      ConnectHandler handler = [](const std::error_code&) {});

  /// @brief 断开连接Robot
  /// @param block false:异步模式; true:同步模式;
  /// @param handler 异步模式断开连接完成后调用;同步模式不用此参数;
  /// @return std::error_code
  /// @retval std::errc::success 操作成功
  ///       常见失败错误码包括：
  ///       std::errc::operation_in_progress 操作在进行中
  ///       std::errc::operation_canceled 操作已取消
  /// @note
  /// 异步模式下，Disconnect函数返回时仅表示连接请求已发出，回调函数将在断开完成后被调用；同步模式下，Disconnect函数返回时即表示连接结果。
  std::error_code Disconnect(
      bool block = false,
      DisConnectHandler handler = [](const std::error_code&) {});

  /// @brief 判断是否连接
  /// @return true:连接状态; false:未连接状态
  bool IsConnected() const;

  /// @brief 获取当前连接状态
  ConnectionState GetConnectionState() const;

  /// @brief 设置控制消息通知函数，用来通知接收到控制命令的应答
  /// @param control_callback
  void SetControlCallback(std::shared_ptr<IControlCallback> control_callback);

  /// @brief 设置数据消息通知函数，用来通知数据到达
  /// @param data_callback
  void SetDataCallback(std::shared_ptr<IDataCallback> data_callback);

  /// @brief 急停
  /// @param on true:急停;false:关闭急停
  /// @param timeout_ms 0:异步模式; 大于0:同步模式;
  /// @param handler 异步模式下命令发送结果回调函数;同步模式不用此参数;
  /// @return std::error_code
  /// @retval std::errc::success 操作成功
  ///       常见失败错误码包括：
  ///       std::errc::timed_out 操作超时
  ///       std::errc::not_connected 未连接
  ///       std::errc::operation_canceled 操作已取消
  /// @note
  /// 异步模式下，SoftEmergencyStop函数返回时仅表示命令请求已发送，命令发送结果通过回调函数通知；同步模式下，SoftEmergencyStop函数返回时即表示命令发送结果。
  std::error_code SoftEmergencyStop(
      bool on, int timeout_ms = 0,
      WriteHandler handler = [](const std::error_code&, std::size_t) {});

  /// @brief 站立
  /// @param timeout_ms 0:异步模式; 大于0:同步模式;
  /// @param handler 异步模式下命令发送结果回调函数;同步模式不用此参数;
  /// @return std::error_code
  /// @retval std::errc::success 操作成功
  ///       常见失败错误码包括：
  ///       std::errc::timed_out 操作超时
  ///       std::errc::not_connected 未连接
  ///       std::errc::operation_canceled 操作已取消
  /// @note
  /// 异步模式下，StandUp函数返回时仅表示命令请求已发送，命令发送结果通过回调函数通知；同步模式下，StandUp函数返回时即表示命令发送结果。
  std::error_code StandUp(
      int timeout_ms = 0,
      WriteHandler handler = [](const std::error_code&, std::size_t) {});

  /// @brief 卧倒
  /// @param timeout_ms 0:异步模式; 大于0:同步模式;
  /// @param handler 异步模式下命令发送结果回调函数;同步模式不用此参数;
  /// @return std::error_code
  /// @retval std::errc::success 操作成功
  ///       常见失败错误码包括：
  ///       std::errc::timed_out 操作超时
  ///       std::errc::not_connected 未连接
  ///       std::errc::operation_canceled 操作已取消
  /// @note
  /// 异步模式下，LieDown函数返回时仅表示命令请求已发送，命令发送结果通过回调函数通知；同步模式下，LieDown函数返回时即表示命令发送结果。
  std::error_code LieDown(
      int timeout_ms = 0,
      WriteHandler handler = [](const std::error_code&, std::size_t) {});

  /// @brief 匍匐
  /// @param timeout_ms 0:异步模式; 大于0:同步模式;
  /// @param handler 异步模式下命令发送结果回调函数;同步模式不用此参数;
  /// @return std::error_code
  /// @retval std::errc::success 操作成功
  ///       常见失败错误码包括：
  ///       std::errc::timed_out 操作超时
  ///       std::errc::not_connected 未连接
  ///       std::errc::operation_canceled 操作已取消
  /// @note
  /// 异步模式下，Crawl函数返回时仅表示命令请求已发送，命令发送结果通过回调函数通知；同步模式下，Crawl函数返回时即表示命令发送结果。
  std::error_code Crawl(
      int timeout_ms = 0,
      WriteHandler handler = [](const std::error_code&, std::size_t) {});

  /// @brief 爬高台，只能在通用模式下使用。
  /// @param timeout_ms 0:异步模式; 大于0:同步模式;
  /// @param handler 异步模式下命令发送结果回调函数;同步模式不用此参数;
  /// @return std::error_code
  /// @retval std::errc::success 操作成功
  ///       常见失败错误码包括：
  ///       std::errc::timed_out 操作超时
  ///       std::errc::not_connected 未连接
  ///       std::errc::operation_canceled 操作已取消
  /// @note
  /// 异步模式下，Climb函数返回时仅表示命令请求已发送，命令发送结果通过回调函数通知；同步模式下，Climb函数返回时即表示命令发送结果。
  std::error_code Climb(
      int timeout_ms = 0,
      WriteHandler handler = [](const std::error_code&, std::size_t) {});

  /// @brief 步态，只能在通用模式下使用。
  /// @param timeout_ms 0:异步模式; 大于0:同步模式;
  /// @param handler 异步模式下命令发送结果回调函数;同步模式不用此参数;
  /// @return std::error_code
  /// @retval std::errc::success 操作成功
  ///       常见失败错误码包括：
  ///       std::errc::timed_out 操作超时
  ///       std::errc::not_connected 未连接
  ///       std::errc::operation_canceled 操作已取消
  /// @note
  /// 异步模式下，Gait函数返回时仅表示命令请求已发送，命令发送结果通过回调函数通知；同步模式下，Gait函数返回时即表示命令发送结果。
  std::error_code Gait(
      int timeout_ms = 0,
      WriteHandler handler = [](const std::error_code&, std::size_t) {});

  /// @brief 瘦身，只能在通用模式下使用。
  /// @param timeout_ms 0:异步模式; 大于0:同步模式;
  /// @param handler 异步模式下命令发送结果回调函数;同步模式不用此参数;
  /// @return std::error_code
  /// @retval std::errc::success 操作成功
  ///       常见失败错误码包括：
  ///       std::errc::timed_out 操作超时
  ///       std::errc::not_connected 未连接
  ///       std::errc::operation_canceled 操作已取消
  /// @note
  /// 异步模式下，Slim函数返回时仅表示命令请求已发送，命令发送结果通过回调函数通知；同步模式下，Slim函数返回时即表示命令发送结果。
  std::error_code Slim(
      int timeout_ms = 0,
      WriteHandler handler = [](const std::error_code&, std::size_t) {});

  /// @brief 调转头尾
  /// @param timeout_ms 0:异步模式; 大于0:同步模式;
  /// @param handler 异步模式下命令发送结果回调函数;同步模式不用此参数;
  /// @return std::error_code
  /// @retval std::errc::success 操作成功
  ///       常见失败错误码包括：
  ///       std::errc::timed_out 操作超时
  ///       std::errc::not_connected 未连接
  ///       std::errc::operation_canceled 操作已取消
  /// @note
  /// 异步模式下，ReverseHeadTail函数返回时仅表示命令请求已发送，命令发送结果通过回调函数通知；同步模式下，ReverseHeadTail函数返回时即表示命令发送结果。
  std::error_code ReverseHeadTail(
      int timeout_ms = 0,
      WriteHandler handler = [](const std::error_code&, std::size_t) {});

  /// @brief 设置模式;默认是通用模式
  /// @param mode 1:通用模式; 2:原地模式; 3:登阶模式
  /// @param timeout_ms 0:异步模式; 大于0:同步模式;
  /// @param handler 异步模式下命令发送结果回调函数;同步模式不用此参数;
  /// @return std::error_code
  /// @retval std::errc::success 操作成功
  ///       常见失败错误码包括：
  ///       std::errc::timed_out 操作超时
  ///       std::errc::not_connected 未连接
  ///       std::errc::operation_canceled 操作已取消
  ///       std::errc::invalid_argument 参数无效
  /// @note
  /// 异步模式下，SetMode函数返回时仅表示命令请求已发送，命令发送结果通过回调函数通知；同步模式下，SetMode函数返回时即表示命令发送结果。
  std::error_code SetMode(
      int mode, int timeout_ms = 0,
      WriteHandler handler = [](const std::error_code&, std::size_t) {});

  /// @brief 设置速度等级，主要影响通用模式下的Move速度限速，默认为低速
  /// @param speed_level 范围[1，3]
  ///        参数为1:表示低速
  ///        参数为2:表示中速
  ///        参数为3:表示高速
  /// @param timeout_ms 0:异步模式; 大于0:同步模式;
  /// @param handler 异步模式下命令发送结果回调函数;同步模式不用此参数;
  /// @return std::error_code
  /// @retval std::errc::success 操作成功
  ///       常见失败错误码包括：
  ///       std::errc::timed_out 操作超时
  ///       std::errc::not_connected 未连接
  ///       std::errc::operation_canceled 操作已取消
  ///       std::errc::invalid_argument 参数无效
  /// @note
  /// 异步模式下，SetSpeed函数返回时仅表示命令请求已发送，命令发送结果通过回调函数通知；同步模式下，SetSpeed函数返回时即表示命令发送结果。
  std::error_code SetSpeed(
      int speed_level, int timeout_ms = 0,
      WriteHandler handler = [](const std::error_code&, std::size_t) {});

  /// @brief 锁定，各个关节保持不动
  /// @param timeout_ms 0:异步模式; 大于0:同步模式;
  /// @param handler 异步模式下命令发送结果回调函数;同步模式不用此参数;
  /// @return std::error_code
  /// @retval std::errc::success 操作成功
  ///       常见失败错误码包括：
  ///       std::errc::timed_out 操作超时
  ///       std::errc::not_connected 未连接
  ///       std::errc::operation_canceled 操作已取消
  /// @note
  /// 异步模式下，Locked函数返回时仅表示命令请求已发送，命令发送结果通过回调函数通知；同步模式下，Locked函数返回时即表示命令发送结果。
  std::error_code Locked(
      int timeout_ms = 0,
      WriteHandler handler = [](const std::error_code&, std::size_t) {});

  /// @brief 设置前补光灯，设置后自动模式关闭
  /// @param on true:开启;false:关闭
  /// @param timeout_ms 0:异步模式; 大于0:同步模式;
  /// @param handler 异步模式下命令发送结果回调函数;同步模式不用此参数;
  /// @return std::error_code
  /// @retval std::errc::success 操作成功
  ///       常见失败错误码包括：
  ///       std::errc::timed_out 操作超时
  ///       std::errc::not_connected 未连接
  ///       std::errc::operation_canceled 操作已取消
  /// @note
  /// 异步模式下，FrontLight函数返回时仅表示命令请求已发送，命令发送结果通过回调函数通知；同步模式下，FrontLight函数返回时即表示命令发送结果。
  std::error_code FrontLight(
      bool on, int timeout_ms = 0,
      WriteHandler handler = [](const std::error_code&, std::size_t) {});

  /// @brief 设置后补光灯，设置后自动模式关闭
  /// @param on true:开启;false:关闭
  /// @param timeout_ms 0:异步模式; 大于0:同步模式;
  /// @param handler 异步模式下命令发送结果回调函数;同步模式不用此参数;
  /// @return std::error_code
  /// @retval std::errc::success 操作成功
  ///       常见失败错误码包括：
  ///       std::errc::timed_out 操作超时
  ///       std::errc::not_connected 未连接
  ///       std::errc::operation_canceled 操作已取消
  /// @note
  /// 异步模式下，BackLight函数返回时仅表示命令请求已发送，命令发送结果通过回调函数通知；同步模式下，BackLight函数返回时即表示命令发送结果。
  std::error_code BackLight(
      bool on, int timeout_ms = 0,
      WriteHandler handler = [](const std::error_code&, std::size_t) {});

  /// @brief 设置自动补光灯模式
  /// @param on true:开启;false:关闭
  /// @param timeout_ms 0:异步模式; 大于0:同步模式;
  /// @param handler 异步模式下命令发送结果回调函数;同步模式不用此参数;
  /// @return std::error_code
  /// @retval std::errc::success 操作成功
  ///       常见失败错误码包括：
  ///       std::errc::timed_out 操作超时
  ///       std::errc::not_connected 未连接
  ///       std::errc::operation_canceled 操作已取消
  /// @note
  /// 异步模式下，AutoModeLight函数返回时仅表示命令请求已发送，命令发送结果通过回调函数通知；同步模式下，AutoModeLight函数返回时即表示命令发送结果。
  std::error_code AutoModeLight(
      bool on, int timeout_ms = 0,
      WriteHandler handler = [](const std::error_code&, std::size_t) {});

  /// @brief 移动，通用模式下生效； 单位：百分比，根据SpeedLevel来限速
  /// @param left_right 范围[-1.0, 1.0], 正数左平移，负数右平移
  /// @param forward_back 范围[-1.0, 1.0], 正数前进，负数后退
  /// @param yaw 范围[-1.0, 1.0], 正数左旋转，负数右旋转
  /// @param timeout_ms 0:异步模式; 大于0:同步模式;
  /// @param handler 异步模式下命令发送结果回调函数;同步模式不用此参数;
  /// @return std::error_code
  /// @retval std::errc::success 操作成功
  ///       常见失败错误码包括：
  ///       std::errc::timed_out 操作超时
  ///       std::errc::not_connected 未连接
  ///       std::errc::operation_canceled 操作已取消
  ///       std::errc::invalid_argument 参数无效
  /// @note
  /// 异步模式下，Move函数返回时仅表示命令请求已发送，命令发送结果通过回调函数通知；同步模式下，Move函数返回时即表示命令发送结果。
  std::error_code Move(
      float left_right, float forward_back, float yaw, int timeout_ms = 0,
      WriteHandler handler = [](const std::error_code&, std::size_t) {});

  /// @brief 翻滚；原地模式下生效；
  /// @param direction 0:恢复 1：左翻滚 2：右翻滚
  /// @param timeout_ms 0:异步模式; 大于0:同步模式;
  /// @param handler 异步模式下命令发送结果回调函数;同步模式不用此参数;
  /// @return std::error_code
  /// @retval std::errc::success 操作成功
  ///       常见失败错误码包括：
  ///       std::errc::timed_out 操作超时
  ///       std::errc::not_connected 未连接
  ///       std::errc::operation_canceled 操作已取消
  /// @note
  /// 异步模式下，Turn函数返回时仅表示命令请求已发送，命令发送结果通过回调函数通知；同步模式下，Turn函数返回时即表示命令发送结果。
  std::error_code Turn(
      int direction, int timeout_ms = 0,
      WriteHandler handler = [](const std::error_code&, std::size_t) {});

  /// @brief 左右探头，抬头低头；原地模式下生效；
  /// @param left_right 范围[-1.0, 1.0]，正数是左探头，负数是右探头；单位是rad/s
  /// @param up_down 范围[-1.0, 1.0]，正数是抬头，负数是低头；单位是rad/s
  /// @param timeout_ms 0:异步模式; 大于0:同步模式;
  /// @param handler 异步模式下命令发送结果回调函数;同步模式不用此参数;
  /// @return std::error_code
  /// @retval std::errc::success 操作成功
  ///       常见失败错误码包括：
  ///       std::errc::timed_out 操作超时
  ///       std::errc::not_connected 未连接
  ///       std::errc::operation_canceled 操作已取消
  ///       std::errc::invalid_argument 参数无效
  /// @note
  /// 异步模式下，ControlHead函数返回时仅表示命令请求已发送，命令发送结果通过回调函数通知；同步模式下，ControlHead函数返回时即表示命令发送结果。
  std::error_code ControlHead(
      float left_right, float up_down, int timeout_ms = 0,
      WriteHandler handler = [](const std::error_code&, std::size_t) {});

  /// @brief 高低站姿；原地模式下生效，没有阻塞模式；
  /// @param stance 0:恢复 1：高站姿 2：低站姿
  /// @param timeout_ms 0:异步模式; 大于0:同步模式;
  /// @param handler 异步模式下命令发送结果回调函数;同步模式不用此参数;
  /// @retval std::errc::success 操作成功
  ///       常见失败错误码包括：
  ///       std::errc::timed_out 操作超时
  ///       std::errc::not_connected 未连接
  ///       std::errc::operation_canceled 操作已取消
  ///       std::errc::invalid_argument 参数无效
  /// @note
  /// 异步模式下，HighLowStance函数返回时仅表示命令请求已发送，命令发送结果通过回调函数通知；同步模式下，HighLowStance函数返回时即表示命令发送结果。
  std::error_code HighLowStance(
      int stance, int timeout_ms = 0,
      WriteHandler handler = [](const std::error_code&, std::size_t) {});

  /// @brief 配置IMU上报频率，默认关闭
  /// @param freq 范围[0, 100], 0:关闭
  /// @param timeout_ms 0:异步模式; 大于0:同步模式;
  /// @param handler 异步模式下命令发送结果回调函数;同步模式不用此参数;
  /// @return std::error_code
  /// @retval std::errc::success 操作成功
  ///       常见失败错误码包括：
  ///       std::errc::timed_out 操作超时
  ///       std::errc::not_connected 未连接
  ///       std::errc::operation_canceled 操作已取消
  /// @note
  /// 异步模式下，SetImuConfig函数返回时仅表示命令请求已发送，命令发送結果通过回调函数通知；同步模式下，SetImuConfig函数返回时即表示命令发送结果。
  std::error_code SetImuConfig(
      int freq, int timeout_ms = 0,
      WriteHandler handler = [](const std::error_code&, std::size_t) {});

  /// @brief 配置光强值上报，默认关闭
  /// @param on true:开启;false:关闭
  /// @param timeout_ms 0:异步模式; 大于0:同步模式;
  /// @param handler 异步模式下命令发送结果回调函数;同步模式不用此参数;
  /// @return std::error_code
  /// @retval std::errc::success 操作成功
  ///       常见失败错误码包括：
  ///       std::errc::timed_out 操作超时
  ///       std::errc::not_connected 未连接
  ///       std::errc::operation_canceled 操作已取消
  /// @note
  /// 异步模式下，SetLuxConfig函数返回时仅表示命令请求已发送，命令发送结果通过回调函数通知；同步模式下，SetLuxConfig函数返回时即表示命令发送结果。
  std::error_code SetLuxConfig(
      bool on, int timeout_ms = 0,
      WriteHandler handler = [](const std::error_code&, std::size_t) {});

  /// @brief 配置运动数据上报，默认关闭
  /// @param on true:开启;false:关闭
  /// @param timeout_ms 0:异步模式; 大于0:同步模式;
  /// @param handler 异步模式下命令发送结果回调函数;同步模式不用此参数;
  /// @return std::error_code
  /// @retval std::errc::success 操作成功
  ///       常见失败错误码包括：
  ///       std::errc::timed_out 操作超时
  ///       std::errc::not_connected 未连接
  ///       std::errc::operation_canceled 操作已取消
  /// @note
  /// 异步模式下，SetMcConfig函数返回时仅表示命令请求已发送，命令发送结果通过回调函数通知；同步模式下，SetMcConfig函数返回时即表示命令发送结果。
  std::error_code SetMcConfig(
      bool on, int timeout_ms = 0,
      WriteHandler handler = [](const std::error_code&, std::size_t) {});

  /// @brief 获取控制权
  /// @param timeout_ms 0:异步模式; 大于0:同步模式;
  /// @param handler 异步模式下命令发送结果回调函数;同步模式不用此参数;
  /// @return std::error_code
  /// @retval std::errc::success 操作成功
  ///       常见失败错误码包括：
  ///       std::errc::timed_out 操作超时
  ///       std::errc::not_connected 未连接
  ///       std::errc::operation_canceled 操作已取消
  /// @note
  /// 异步模式下，TakeControl函数返回时仅表示命令请求已发送，命令发送结果通过回调函数通知；同步模式下，TakeControl函数返回时即表示命令发送结果。
  std::error_code TakeControl(
      int timeout_ms = 0,
      WriteHandler handler = [](const std::error_code&, std::size_t) {});

  /// @brief 释放控制权
  /// @param timeout_ms 0:异步模式; 大于0:同步模式;
  /// @param handler 异步模式下命令发送结果回调函数;同步模式不用此参数;
  /// @return std::error_code
  /// @retval std::errc::success 操作成功
  ///       常见失败错误码包括：
  ///       std::errc::timed_out 操作超时
  ///       std::errc::not_connected 未连接
  ///       std::errc::operation_canceled 操作已取消
  /// @note
  /// 异步模式下，ReleaseControl函数返回时仅表示命令请求已发送，命令发送结果通过回调函数通知；同步模式下，ReleaseControl函数返回时即表示命令发送结果。
  std::error_code ReleaseControl(
      int timeout_ms = 0,
      WriteHandler handler = [](const std::error_code&, std::size_t) {});

  /// @brief 获取SDK版本
  /// @return SDK版本
  const std::string& Version() const;

  /// @brief 获取协议版本
  /// @return 协议版本
  const std::string& ProtocolVersion() const;

  /// @brief 获取平台软件系统版本（成功连接后）
  /// @return 平台软件系统版本
  const std::string& SystemVersion() const;

 private:
  SDKClient(const SDKClient&) = delete;
  SDKClient& operator=(const SDKClient&) = delete;

 private:
  class Impl;
  std::shared_ptr<Impl> pImpl_;
};

}  // namespace robot_sdk
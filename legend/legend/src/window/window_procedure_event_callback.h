#ifndef LEGEND_DEVICE_WINDOW_PROCEDURE_EVENT_CALLBACK_H_
#define LEGEND_DEVICE_WINDOW_PROCEDURE_EVENT_CALLBACK_H_

/**
 * @file window_procedure_event_callback.h
 * @brief ウィンドウプロシージャイベント通知先定義
 */

namespace legend {
namespace window {
/**
 * @class IWindowProcedureEventCallback
 * @brief ウィンドウプロシージャイベント通知先
 */
class IWindowProcedureEventCallback {
 public:
  /**
   * @brief 仮想デストラクタ
   */
  virtual ~IWindowProcedureEventCallback() = default;
  /**
   * @brief ウィンドウ破壊
   */
  virtual void Destroy() = 0;
  /**
   * @brief 描画イベント
   */
  virtual void Paint() = 0;
};

}  // namespace window
}  // namespace legend

#endif  // !LEGEND_DEVICE_WINDOW_PROCEDURE_EVENT_CALLBACK_H_

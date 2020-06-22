#ifndef LEGEND_DEVICE_WINDOW_PROCEDURE_EVENT_CALLBACK_H_
#define LEGEND_DEVICE_WINDOW_PROCEDURE_EVENT_CALLBACK_H_

/**
 * @file window_procedure_event_callback.h
 * @brief �E�B���h�E�v���V�[�W���C�x���g�ʒm���`
 */

namespace dx12_lib {
namespace window {
/**
 * @class IWindowProcedureEventCallback
 * @brief �E�B���h�E�v���V�[�W���C�x���g�ʒm��
 */
class IWindowProcedureEventCallback {
 public:
  /**
   * @brief ���z�f�X�g���N�^
   */
  virtual ~IWindowProcedureEventCallback() = default;
  /**
   * @brief �E�B���h�E�j��
   */
  virtual void Destroy() = 0;
  /**
   * @brief �`��C�x���g
   */
  virtual void Paint() = 0;
};

}  // namespace window
}  // namespace dx12_lib

#endif  // !LEGEND_DEVICE_WINDOW_PROCEDURE_EVENT_CALLBACK_H_

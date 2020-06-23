#ifndef LEGEND_DEVICE_APPLICATION_H_
#define LEGEND_DEVICE_APPLICATION_H_

/**
 * @file application.h
 * @brief �Q�[���A�v���P�[�V�����N���X��`
 */

#include "src/directx/directx12_device.h"
#include "src/window/window.h"
#include "src/window/window_procedure_event_callback.h"

namespace legend {
namespace device {
/**
 * @brief �Q�[���A�v���P�[�V�����N���X
 */
class Application : public window::IWindowProcedureEventCallback {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  Application();
  /**
   * @brief �f�X�g���N�^
   */
  ~Application();
  /**
   * @brief �A�v���P�[�V�����œ������E�B���h�E��o�^����
   */
  void RegisterWindow(std::shared_ptr<window::Window> window);
  /**
   * @brief �A�v���P�[�V�����̎��s
   */
  void Run();

 public:
  /**
   * @brief �E�B���h�E�j�󎞂ɌĂ΂��
   */
  void Destroy() override;
  /**
   * @brief �`��C�x���g���ɌĂ΂��
   */
  void Paint() override;

  /**
   * @brief �A�v���P�[�V����������
   */
  virtual bool Init();
  /**
   * @brief �A�v���P�[�V�����I������
   */
  virtual void Finalize();
  /**
   * @brief �X�V����
   */
  virtual bool Update();
  /**
   * @brief �`�揈��
   */
  virtual bool Draw();

 public:
  /**
   * @brief �f�o�C�X���擾����
   */
  directx::DirectX12Device& GetDirectX12Device() const { return *device_; }

 private:
  /**
   * @brief �t���[���J�n���C�x���g
   */
  bool FrameBegin();
  /**
   * @brief �t���[���I�����C�x���g
   */
  bool FrameEnd();

 protected:
  //! ���C���E�B���h�E
  std::shared_ptr<window::Window> main_window_;
  //! DX12�f�o�C�X
  std::unique_ptr<directx::DirectX12Device> device_;

 public:
  //�R�s�[�֎~�A���[�u�֎~
  Application(const Application&) = delete;
  Application& operator=(const Application&) = delete;
  Application(Application&&) = delete;
  Application& operator=(Application&&) = delete;
};
}  // namespace device
}  // namespace legend

#endif  // !LEGEND_DEVICE_APPLICATION_H_

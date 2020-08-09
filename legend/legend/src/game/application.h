#ifndef LEGEND_DEVICE_APPLICATION_H_
#define LEGEND_DEVICE_APPLICATION_H_

/**
 * @file application.h
 * @brief �Q�[���A�v���P�[�V�����N���X��`
 */

#include "src/util/imgui_manager.h"
#include "src/window/window.h"
#include "src/window/window_procedure_event_callback.h"

namespace legend {
namespace device {
/**
 * @brief �Q�[���A�v���P�[�V�����N���X
 */
class Application : public window::IWindowProcedureEventCallback {
 public:
  Application();
  ~Application();
  void Run();

  virtual bool Init();
  virtual bool Update();
  virtual bool Render();
  virtual void Destroy();

  virtual bool BeginFrame();
  virtual bool EndFrame();
  virtual void Paint() override;

 private:
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

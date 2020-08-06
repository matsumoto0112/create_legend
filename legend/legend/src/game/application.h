#ifndef LEGEND_DEVICE_APPLICATION_H_
#define LEGEND_DEVICE_APPLICATION_H_

/**
 * @file application.h
 * @brief ゲームアプリケーションクラス定義
 */

#include "src/util/imgui_manager.h"
#include "src/window/window.h"
#include "src/window/window_procedure_event_callback.h"

namespace legend {
namespace device {
/**
 * @brief ゲームアプリケーションクラス
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
  //コピー禁止、ムーブ禁止
  Application(const Application&) = delete;
  Application& operator=(const Application&) = delete;
  Application(Application&&) = delete;
  Application& operator=(Application&&) = delete;
};
}  // namespace device
}  // namespace legend

#endif  // !LEGEND_DEVICE_APPLICATION_H_

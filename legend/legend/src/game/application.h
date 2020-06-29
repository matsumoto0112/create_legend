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
  /**
   * @brief コンストラクタ
   */
  Application();
  /**
   * @brief デストラクタ
   */
  ~Application();
  /**
   * @brief アプリケーションで動かすウィンドウを登録する
   */
  void RegisterWindow(std::shared_ptr<window::Window> window);
  /**
   * @brief アプリケーションの実行
   */
  void Run();

 public:
  /**
   * @brief ウィンドウ破壊時に呼ばれる
   */
  void Destroy() override;
  /**
   * @brief 描画イベント時に呼ばれる
   */
  void Paint() override;

  /**
   * @brief アプリケーション初期化
   */
  virtual bool Init();
  /**
   * @brief アプリケーション終了処理
   */
  virtual void Finalize();
  /**
   * @brief 更新処理
   */
  virtual bool Update();
  /**
   * @brief 描画処理
   */
  virtual bool Draw();

 private:
  /**
   * @brief フレーム開始時イベント
   */
  bool FrameBegin();
  /**
   * @brief フレーム終了時イベント
   */
  bool FrameEnd();

 protected:
  //! メインウィンドウ
  std::shared_ptr<window::Window> main_window_;
  //! Imgui管理
  util::ImguiManager imgui_manager_;

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

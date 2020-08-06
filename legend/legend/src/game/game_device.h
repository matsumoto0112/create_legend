#ifndef LEGEND_GAME_GAME_DEVICE_H_
#define LEGEND_GAME_GAME_DEVICE_H_

/**
 * @file game_device.h
 * @brief ゲーム機器管理クラス定義
 */

#include "src/audio/audio_manager.h"
#include "src/directx/device/directx_device.h"
#include "src/input/input_manager.h"
#include "src/util/fps_counter.h"
#include "src/util/random.h"
#include "src/util/singleton.h"
#include "src/window/window.h"

namespace legend {
namespace game {

/**
 * @class GameDevice
 * @brief ゲームにおける機器管理
 */
class GameDevice : public util::Singleton<GameDevice> {
 public:
  bool Init(window::IWindowProcedureEventCallback* callback);

  bool BeginFrame();
  bool EndFrame();

  void Finalize();

 public:
  /**
   * @brief ウィンドウを取得する
   */
  window::Window& GetWindow() const { return *window_; }

  /**
   * @brief FPSカウンターを取得する
   */
  const util::FPSCounter& GetFPSCounter() const { return fps_counter_; }
  /**
   * @brief 入力デバイスを取得する
   */
  input::InputManager& GetInput() const { return *input_manager_; }
  /**
   * @brief 音響管理デバイスを取得する
   */
  audio::AudioManager& GetAudioManager() const { return *audio_manager; }
  /**
   * @brief 乱数デバイスを取得する
   */
  util::Random& GetRandom() const { return *random_; }

 protected:
  /**
   * @brief コンストラクタ
   */
  GameDevice();
  /**
   * @brief デストラクタ
   */
  ~GameDevice();

 private:
  util::FPSCounter fps_counter_;

  std::unique_ptr<window::Window> window_;
  std::unique_ptr<directx::device::DirectXDevice> device_;
  //! 入力管理
  std::unique_ptr<input::InputManager> input_manager_;
  //! 音響管理
  std::unique_ptr<audio::AudioManager> audio_manager;
  //! 乱数デバイス
  std::unique_ptr<util::Random> random_;
};

}  // namespace game
}  // namespace legend

#endif  //! LEGEND_GAME_GAME_DEVICE_H_

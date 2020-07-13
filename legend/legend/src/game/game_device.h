#pragma once
#ifndef LEGEND_GAME_GAME_DEVICE_H_
#define LEGEND_GAME_GAME_DEVICE_H_

/**
 * @file game_device.h
 * @brief ゲーム機器管理クラス定義
 */

#include "src/audio/audio_manager.h"
#include "src/directx/directx12_device.h"
#include "src/draw/sprite_renderer.h"
#include "src/input/input_manager.h"
#include "src/util/fps_counter.h"
#include "src/util/random.h"
#include "src/util/singleton.h"

namespace legend {
namespace game {

/**
 * @class GameDevice
 * @brief ゲームにおける機器管理
 */
class GameDevice : public util::Singleton<GameDevice> {
 public:
  /**
   * @brief デストラクタ
   */
  ~GameDevice();
  /**
   * @brief 初期化
   * @param target_window 現在描画対象となっているウィンドウ
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(std::shared_ptr<window::Window> target_window);
  /**
   * @brief フレーム更新処理
   */
  void Update();

 public:
  /**
   * @brief ウィンドウを取得する
   */
  window::Window& GetWindow() const { return *main_window_; }
  /**
   * @brief FPSカウンターを取得する
   */
  const util::FPSCounter& GetFPSCounter() const { return fps_counter_; }
  /**
   * @brief DirectX12デバイスを取得する
   */
  directx::DirectX12Device& GetDevice() const { return *device_; }
  /**
   * @brief 入力デバイスを取得する
   */
  input::InputManager& GetInput() const { return *input_manager_; }
  /**
   * @brief 音響管理デバイスを取得する
   */
  audio::AudioManager& GetAudioManager() const { return *audio_manager; }
  /**
   * @brief スプライト描画デバイスを取得する
   */
  draw::SpriteRenderer& GetSpriteRenderer() const { return *sprite_renderer_; }
  /**
   * @brief 乱数デバイスを取得する
   */
  util::Random& GetRandom() const { return *random_; }

 protected:
  /**
   * @brief コンストラクタ
   */
  GameDevice();

 private:
  //! メインウィンドウ
  std::shared_ptr<window::Window> main_window_;
  //! FPS計測
  util::FPSCounter fps_counter_;
  //! DX12デバイス
  std::unique_ptr<directx::DirectX12Device> device_;
  //! 入力管理
  std::unique_ptr<input::InputManager> input_manager_;
  //! 音響管理
  std::unique_ptr<audio::AudioManager> audio_manager;
  //! スプライト描画デバイス
  std::unique_ptr<draw::SpriteRenderer> sprite_renderer_;
  //! 乱数デバイス
  std::unique_ptr<util::Random> random_;
};

}  // namespace game
}  // namespace legend

#endif  //! LEGEND_GAME_GAME_DEVICE_H_

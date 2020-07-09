#pragma once
#ifndef LEGEND_GAME_GAME_DEVICE_H_
#define LEGEND_GAME_GAME_DEVICE_H_

/**
 * @file game_device.h
 * @brief ゲーム機器管理クラス定義
 */

#include "src/directx/directx12_device.h"
#include "src/input/input_manager.h"
#include "src/util/fps_counter.h"
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
  const util::FPSCounter& GetFPSCounter() const { return fps_counter_; }
  /**
   * @brief DirectX12デバイスを取得する
   */
  directx::DirectX12Device& GetDevice() const { return *device_; }
  /**
   * @brief 入力デバイスを取得する
   */
  input::InputManager& GetInput() const { return *input_manager_; }

 protected:
  /**
   * @brief コンストラクタ
   */
  GameDevice();

 private:
  //! FPS計測
  util::FPSCounter fps_counter_;
  //! DX12デバイス
  std::unique_ptr<directx::DirectX12Device> device_;
  //! 入力管理
  std::unique_ptr<input::InputManager> input_manager_;
  //! 音響管理
  // AudioManager
};

}  // namespace game
}  // namespace legend

#endif  //! LEGEND_GAME_GAME_DEVICE_H_

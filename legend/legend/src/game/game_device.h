#ifndef LEGEND_GAME_GAME_DEVICE_H_
#define LEGEND_GAME_GAME_DEVICE_H_

/**
 * @file game_device.h
 * @brief ゲーム機器管理クラス定義
 */

#include "src/audio/audio_manager.h"
#include "src/directx/device/directx_device.h"
#include "src/draw/particle/particle_command_list.h"
#include "src/draw/sprite_renderer.h"
#include "src/input/input_manager.h"
#include "src/util/fps_counter.h"
#include "src/util/imgui_manager.h"
#include "src/util/random.h"
#include "src/util/resource/resource.h"
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
  bool MidFrame();
  bool EndFrame();

  void Finalize();

 public:
  /**
   * @brief ウィンドウを取得する
   */
  window::Window& GetWindow() const { return *window_; }

  directx::device::DirectXDevice& GetDevice() { return *device_; }
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
  /**
   * @brief リソースを取得する
   */
  util::resource::Resource& GetResource() { return resource_; }
  /**
   * @brief スプライト描画器を取得する
   */
  draw::SpriteRenderer& GetSpriteRenderer() { return sprite_renderer_; }
  /**
   * @brief パーティクルコマンドリストを取得する
   */
  draw::particle::ParticleCommandList& GetParticleCommandList() {
    return particle_command_list_;
  }

 protected:
  /**
   * @brief コンストラクタ
   */
  GameDevice();
  /**
   * @brief デストラクタ
   */
  virtual ~GameDevice();

 private:
  static constexpr legend::u32 WINDOW_WIDTH = 1280;
  static constexpr legend::u32 WINDOW_HEIGHT = 720;
  static constexpr const wchar_t* GAME_TITLE = L"Armed Eraser";

  //! FPS計測
  util::FPSCounter fps_counter_;
  //! メインウィンドウ
  std::unique_ptr<window::Window> window_;
  //! デバイス
  std::unique_ptr<directx::device::DirectXDevice> device_;
  //! 入力管理
  std::unique_ptr<input::InputManager> input_manager_;
  //! 音響管理
  std::unique_ptr<audio::AudioManager> audio_manager;
  //! 乱数デバイス
  std::unique_ptr<util::Random> random_;
  //! ImGui管理
  util::ImguiManager imgui_manager_;
  //! リソース管理
  util::resource::Resource resource_;
  //! スプライト描画
  draw::SpriteRenderer sprite_renderer_;
  //! パーティクルコマンドリスト
  draw::particle::ParticleCommandList particle_command_list_;

  //シェーダーで共通して使用するバッファ
  directx::buffer::ConstantBuffer<
      directx::buffer::constant_buffer_structure::GlobalData>
      global_cb_;
};

}  // namespace game
}  // namespace legend

#endif  //! LEGEND_GAME_GAME_DEVICE_H_

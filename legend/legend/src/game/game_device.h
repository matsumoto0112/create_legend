#ifndef LEGEND_GAME_GAME_DEVICE_H_
#define LEGEND_GAME_GAME_DEVICE_H_

/**
 * @file game_device.h
 * @brief �Q�[���@��Ǘ��N���X��`
 */

#include "src/audio/audio_manager.h"
#include "src/directx/device/directx_device.h"
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
 * @brief �Q�[���ɂ�����@��Ǘ�
 */
class GameDevice : public util::Singleton<GameDevice> {
 public:
  bool Init(window::IWindowProcedureEventCallback* callback);

  bool BeginFrame();
  bool EndFrame();

  void Finalize();

 public:
  /**
   * @brief �E�B���h�E���擾����
   */
  window::Window& GetWindow() const { return *window_; }

  directx::device::DirectXDevice& GetDevice() { return *device_; }
  /**
   * @brief FPS�J�E���^�[���擾����
   */
  const util::FPSCounter& GetFPSCounter() const { return fps_counter_; }
  /**
   * @brief ���̓f�o�C�X���擾����
   */
  input::InputManager& GetInput() const { return *input_manager_; }
  /**
   * @brief �����Ǘ��f�o�C�X���擾����
   */
  audio::AudioManager& GetAudioManager() const { return *audio_manager; }
  /**
   * @brief �����f�o�C�X���擾����
   */
  util::Random& GetRandom() const { return *random_; }

  util::resource::Resource& GetResource() { return resource_; }

 protected:
  /**
   * @brief �R���X�g���N�^
   */
  GameDevice();
  /**
   * @brief �f�X�g���N�^
   */
  ~GameDevice();

 private:
  util::FPSCounter fps_counter_;

  std::unique_ptr<window::Window> window_;
  std::unique_ptr<directx::device::DirectXDevice> device_;
  //! ���͊Ǘ�
  std::unique_ptr<input::InputManager> input_manager_;
  //! �����Ǘ�
  std::unique_ptr<audio::AudioManager> audio_manager;
  //! �����f�o�C�X
  std::unique_ptr<util::Random> random_;
  util::ImguiManager imgui_manager_;
  util::resource::Resource resource_;
};

}  // namespace game
}  // namespace legend

#endif  //! LEGEND_GAME_GAME_DEVICE_H_

#pragma once
#ifndef LEGEND_GAME_GAME_DEVICE_H_
#define LEGEND_GAME_GAME_DEVICE_H_

/**
 * @file game_device.h
 * @brief �Q�[���@��Ǘ��N���X��`
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
 * @brief �Q�[���ɂ�����@��Ǘ�
 */
class GameDevice : public util::Singleton<GameDevice> {
 public:
  /**
   * @brief �f�X�g���N�^
   */
  ~GameDevice();
  /**
   * @brief ������
   * @param target_window ���ݕ`��ΏۂƂȂ��Ă���E�B���h�E
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init(std::shared_ptr<window::Window> target_window);
  /**
   * @brief �t���[���X�V����
   */
  void Update();

 public:
  /**
   * @brief �E�B���h�E���擾����
   */
  window::Window& GetWindow() const { return *main_window_; }
  /**
   * @brief FPS�J�E���^�[���擾����
   */
  const util::FPSCounter& GetFPSCounter() const { return fps_counter_; }
  /**
   * @brief DirectX12�f�o�C�X���擾����
   */
  directx::DirectX12Device& GetDevice() const { return *device_; }
  /**
   * @brief ���̓f�o�C�X���擾����
   */
  input::InputManager& GetInput() const { return *input_manager_; }
  /**
   * @brief �����Ǘ��f�o�C�X���擾����
   */
  audio::AudioManager& GetAudioManager() const { return *audio_manager; }
  /**
   * @brief �X�v���C�g�`��f�o�C�X���擾����
   */
  draw::SpriteRenderer& GetSpriteRenderer() const { return *sprite_renderer_; }
  /**
   * @brief �����f�o�C�X���擾����
   */
  util::Random& GetRandom() const { return *random_; }

 protected:
  /**
   * @brief �R���X�g���N�^
   */
  GameDevice();

 private:
  //! ���C���E�B���h�E
  std::shared_ptr<window::Window> main_window_;
  //! FPS�v��
  util::FPSCounter fps_counter_;
  //! DX12�f�o�C�X
  std::unique_ptr<directx::DirectX12Device> device_;
  //! ���͊Ǘ�
  std::unique_ptr<input::InputManager> input_manager_;
  //! �����Ǘ�
  std::unique_ptr<audio::AudioManager> audio_manager;
  //! �X�v���C�g�`��f�o�C�X
  std::unique_ptr<draw::SpriteRenderer> sprite_renderer_;
  //! �����f�o�C�X
  std::unique_ptr<util::Random> random_;
};

}  // namespace game
}  // namespace legend

#endif  //! LEGEND_GAME_GAME_DEVICE_H_

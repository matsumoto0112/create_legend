#pragma once
#ifndef LEGEND_GAME_GAME_DEVICE_H_
#define LEGEND_GAME_GAME_DEVICE_H_

/**
 * @file game_device.h
 * @brief �Q�[���@��Ǘ��N���X��`
 */

#include "src/directx/directx12_device.h"
#include "src/input/input_manager.h"
#include "src/util/fps_counter.h"
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
  const util::FPSCounter& GetFPSCounter() const { return fps_counter_; }
  /**
   * @brief DirectX12�f�o�C�X���擾����
   */
  directx::DirectX12Device& GetDevice() const { return *device_; }
  /**
   * @brief ���̓f�o�C�X���擾����
   */
  input::InputManager& GetInput() const { return *input_manager_; }

 protected:
  /**
   * @brief �R���X�g���N�^
   */
  GameDevice();

 private:
  //! FPS�v��
  util::FPSCounter fps_counter_;
  //! DX12�f�o�C�X
  std::unique_ptr<directx::DirectX12Device> device_;
  //! ���͊Ǘ�
  std::unique_ptr<input::InputManager> input_manager_;
  //! �����Ǘ�
  // AudioManager
};

}  // namespace game
}  // namespace legend

#endif  //! LEGEND_GAME_GAME_DEVICE_H_

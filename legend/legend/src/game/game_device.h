#pragma once
#ifndef LEGEND_GAME_GAME_DEVICE_H_
#define LEGEND_GAME_GAME_DEVICE_H_

/**
 * @file game_device.h
 * @brief �Q�[���@��Ǘ�
 */

#include "src/directx/directx12_device.h"
#include "src/input/inputManager.h"
#include "src/util/singleton.h"

namespace legend {
namespace game {
class GameDevice : public util::Singleton<GameDevice> {
 public:
  GameDevice();
  ~GameDevice();

  bool Init(std::shared_ptr<window::Window> target_window);
  void Update();

  directx::DirectX12Device& GetDevice() const { return *device_; }

 private:
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

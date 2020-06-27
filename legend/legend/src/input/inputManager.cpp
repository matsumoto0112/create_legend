#include "src/input/inputManager.h"

namespace legend {
namespace input {
//�R���X�g���N�^
InputManager::InputManager(HWND hWnd)
    : mKeyboard_(std::make_unique<Keyboard>(hWnd)),
      mMouse_(std::make_unique<Mouse>(hWnd)),
      mGamePad_(std::make_unique<GamePad>(hWnd)) {}
//�f�X�g���N�^
InputManager::~InputManager() {}
//���͏��̍X�V
void InputManager::Update() {
  mKeyboard_->Update();
  mMouse_->Update();
  mGamePad_->Update();
}
}  // namespace input
}  // namespace legend

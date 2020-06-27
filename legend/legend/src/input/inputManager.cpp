#include "src/input/inputManager.h"

namespace legend {
namespace input {
//コンストラクタ
InputManager::InputManager(HWND hWnd)
    : mKeyboard_(std::make_unique<Keyboard>(hWnd)),
      mMouse_(std::make_unique<Mouse>(hWnd)),
      mGamePad_(std::make_unique<GamePad>(hWnd)) {}
//デストラクタ
InputManager::~InputManager() {}
//入力情報の更新
void InputManager::Update() {
  mKeyboard_->Update();
  mMouse_->Update();
  mGamePad_->Update();
}
}  // namespace input
}  // namespace legend

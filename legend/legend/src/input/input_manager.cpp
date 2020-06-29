#include "src/input/input_manager.h"

namespace legend {
namespace input {
//コンストラクタ
InputManager::InputManager(HWND hWnd)
    : input_keyboard_(std::make_unique<Keyboard>(hWnd)),
      input_mouse_(std::make_unique<Mouse>(hWnd)),
      input_game_pad_(std::make_unique<GamePad>(hWnd)) {}
//デストラクタ
InputManager::~InputManager() {}
//入力情報の更新
void InputManager::Update() {
  input_keyboard_->Update();
  input_mouse_->Update();
  input_game_pad_->Update();
}
}  // namespace input
}  // namespace legend

#include "src/input/input_manager.h"

namespace legend {
namespace input {
//�R���X�g���N�^
InputManager::InputManager(HWND hWnd)
    : input_keyboard_(std::make_unique<Keyboard>(hWnd)),
      input_mouse_(std::make_unique<Mouse>(hWnd)),
      input_game_pad_(std::make_unique<GamePad>(hWnd)),
      input_command_(std::make_unique<InputCommand>()) {}
//�f�X�g���N�^
InputManager::~InputManager() {}
//���͏��̍X�V
void InputManager::Update() {
  input_keyboard_->Update();
  input_mouse_->Update();
  input_game_pad_->Update();

  if (GetCommand(InputCode::Decide)) {
    MY_LOG(L"���́i����j\n");
  }
  if (GetCommand(InputCode::End)) {
    MY_LOG(L"���́i�I���j\n");
  }
}
bool InputManager::GetCommand(InputCode code) const {
  return input_command_->GetCommand(code, GetKeyboard(), GetMouse(),
                                    GetGamepad());
}
// �����ړ����͎擾
float InputManager::GetHorizontal() const {
  float input_keyboard_a = input_keyboard_->GetKey(key_code::A) ? -1.0f : 0.0f;
  float input_keyboard_d = input_keyboard_->GetKey(key_code::D) ? 1.0f : 0.0f;
  float input_keyboard_left =
      input_keyboard_->GetKey(key_code::Left) ? -1.0f : 0.0f;
  float input_keyboard_right =
      input_keyboard_->GetKey(key_code::Right) ? 1.0f : 0.0f;
  float input_gamepad_horizontal = input_game_pad_->GetStickLeft().x;

  float result =
      std::clamp((input_keyboard_a + input_keyboard_d + input_keyboard_left +
                  input_keyboard_right + input_gamepad_horizontal),
                 -1.0f, 1.0f);
  return result;
}
// �c���ړ����͎擾
float InputManager::GetVertical() const {
  float input_keyboard_w = input_keyboard_->GetKey(key_code::W) ? 1.0f : 0.0f;
  float input_keyboard_s = input_keyboard_->GetKey(key_code::S) ? -1.0f : 0.0f;
  float input_keyboard_up =
      input_keyboard_->GetKey(key_code::Up) ? -1.0f : 0.0f;
  float input_keyboard_down =
      input_keyboard_->GetKey(key_code::Down) ? 1.0f : 0.0f;
  float input_gamepad_vertical = input_game_pad_->GetStickLeft().y;

  float result =
      std::clamp((input_keyboard_w + input_keyboard_s + input_keyboard_up +
                  input_keyboard_down + input_gamepad_vertical),
                 -1.0f, 1.0f);
  return result;
}
}  // namespace input
}  // namespace legend

#include "src/ui/ui_board.h"

#include "src/game/game_device.h"

namespace legend {
namespace ui {

//�R���X�g���N�^
UIBoard::UIBoard() {}

//�f�X�g���N�^
UIBoard::~UIBoard() { Reset(); }

//�R���|�[�l���g�̃��Z�b�g
void UIBoard::Reset() { ui_components_.clear(); }

//�R���|�[�l���g�̒ǉ�
UIComponent* UIBoard::AddComponent(std::unique_ptr<UIComponent> component) {
  return ui_components_.emplace_back(std::move(component)).get();
}

//�`��
void UIBoard::Draw() {
  auto& sprite_renderer = game::GameDevice::GetInstance()->GetSpriteRenderer();
  for (auto&& comp : ui_components_) {
    comp->StackOwnItems(sprite_renderer);
  }
}

}  // namespace ui
}  // namespace legend
#include "src/ui/ui_board.h"

#include "src/game/game_device.h"

namespace legend {
namespace ui {

//コンストラクタ
UIBoard::UIBoard() {}

//デストラクタ
UIBoard::~UIBoard() { Reset(); }

//コンポーネントのリセット
void UIBoard::Reset() { ui_components_.clear(); }

//コンポーネントの追加
UIComponent* UIBoard::AddComponent(std::unique_ptr<UIComponent> component) {
  return ui_components_.emplace_back(std::move(component)).get();
}

//描画
void UIBoard::Draw() {
  auto& sprite_renderer = game::GameDevice::GetInstance()->GetSpriteRenderer();
  for (auto&& comp : ui_components_) {
    comp->StackOwnItems(sprite_renderer);
  }
}

}  // namespace ui
}  // namespace legend
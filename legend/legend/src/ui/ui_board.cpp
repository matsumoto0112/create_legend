#include "src/ui/ui_board.h"

#include "src/game/game_device.h"

namespace legend {
namespace ui {

UIBoard::UIBoard() {}

UIBoard::~UIBoard() { Reset(); }

void UIBoard::Reset() { ui_components_.clear(); }

UIComponent* UIBoard::AddComponent(std::unique_ptr<UIComponent> component) {
  return ui_components_.emplace_back(std::move(component)).get();
}

void UIBoard::Draw() {
  auto& sprite_renderer = game::GameDevice::GetInstance()->GetSpriteRenderer();
  for (auto&& comp : ui_components_) {
    comp->StackOwnItems(sprite_renderer);
  }
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& command_list = device.GetCurrentFrameResource()->GetCommandList();
  sprite_renderer.DrawItems(command_list);
}

}  // namespace ui
}  // namespace legend
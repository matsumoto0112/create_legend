#include "skill_select_ui.h"

#include "src/game/game_device.h"
#include "src/util/resource/resource_names.h"

namespace legend {
namespace skill {

SkillSelectUI::SkillSelectUI() {
  icon_base_position_ = math::Vector2(64.0f, 64.0f);
  icon_scale_ = math::Vector2(0.05f, 0.05f);

  is_select_mode_ = false;
  select_number_ = 0;
}

SkillSelectUI::~SkillSelectUI() {}

void SkillSelectUI::Init() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();

  directx::device::CommandList command_list;
  if (!command_list.Init(
          device, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT)) {
    return;
  }

  if (!command_list.Close()) {
    return;
  }
  device.ExecuteCommandList({command_list});
  device.WaitExecute();
}

void SkillSelectUI::Update() {}

void SkillSelectUI::Draw() {
  legend::draw::SpriteRenderer& sprite_renderer =
      game::GameDevice::GetInstance()->GetSpriteRenderer();
  for (auto&& sprite : skill_icons_) {
    sprite_renderer.AddDrawItems(&sprite);
  }
  sprite_renderer.DrawItems(game::GameDevice::GetInstance()
                                ->GetDevice()
                                .GetCurrentFrameResource()
                                ->GetCommandList());
}

void SkillSelectUI::AddSkill(/*const Skill* skill*/) {
  draw::Sprite2D sprite;
  if (!sprite.Init(
          game::GameDevice::GetInstance()->GetResource().GetTexture().Get(
              util::resource::resource_names::texture::TEX),
          directx::descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID)) {
    MY_LOG(L"スキル取得時のスキルアイコンの初期化に失敗しました。");
  }

  //表示位置の設定
  const float icon_distance_ = 64.0f;
  sprite.SetPosition(math::Vector2(
      icon_base_position_.x + icon_distance_ * skill_icons_.size(),
      icon_base_position_.y));
  //表示サイズの設定
  sprite.SetScale(icon_scale_);
  sprite.SetRect(math::Rect(0.0f, 0.0f, 1.0f, 1.0f));
  skill_icons_.push_back(sprite);
}

}  // namespace skill
}  // namespace legend
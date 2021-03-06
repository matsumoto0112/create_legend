#include "src/skill/skill_select_ui.h"

#include "src/game/game_device.h"
#include "src/util/resource/resource_names.h"

namespace legend {
namespace skill {
namespace resource_name = util::resource::resource_names;
//コンストラクタ
SkillSelectUI::SkillSelectUI() {
  icon_base_position_ = math::Vector2(64.0f, 32.0f);
  icon_scale_ = math::Vector2::kUnitVector;

  is_select_mode_ = false;
  select_number_ = 0;

  auto& resource = game::GameDevice::GetInstance()->GetResource();

  if (!skill_select_frame_.Init(
          resource.GetTexture().Get(
              resource_name::texture::UI_SKILL_SELECT_FRAME),
          directx::descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID)) {
    MY_LOG(L"スキル選択アイコンの初期化に失敗しました。");
  }
  skill_select_frame_.SetRect(math::Rect(0, 0, 1, 1));

  for (i32 i = 0; i < 5; i++) {
    if (!skill_frame_icons_[i].Init(
            resource.GetTexture().Get(resource_name::texture::UI_SKILL_FRAME),
            directx::descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID)) {
      MY_LOG(L"スキルの空アイコンの初期化に失敗しました。");
    }

    skill_frame_icons_[i].SetPosition(math::Vector2(
        icon_base_position_.x + 64.0f * i, icon_base_position_.y));
    skill_frame_icons_[i].SetScale(icon_scale_);
    skill_frame_icons_[i].SetRect(math::Rect(0, 0, 1, 1));
    skill_frame_icons_[i].SetZOrder(0.8f);
  }
}

//デストラクタ
SkillSelectUI::~SkillSelectUI() {}

void SkillSelectUI::Init() {}

//更新
void SkillSelectUI::Update() {}

//描画
void SkillSelectUI::Draw(bool is_production) {
  draw::SpriteRenderer& sprite_renderer =
      game::GameDevice::GetInstance()->GetSpriteRenderer();
  for (auto&& sprite : skill_icons_) {
    sprite_renderer.AddDrawItems(&sprite);
  }

  if (is_select_mode_) {
    sprite_renderer.AddDrawItems(&skill_select_frame_);
    if (!is_production) {
      sprite_renderer.AddDrawItems(&skill_explanatories_[select_number_]);
    }
  }

  for (auto&& frame : skill_frame_icons_) {
    sprite_renderer.AddDrawItems(&frame);
  }
}

//スキルの追加
void SkillSelectUI::AddSkill(const Skill* skill) {
  draw::Sprite2D sprite;
  if (!sprite.Init(
          skill->GetIconTexture(),
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
  sprite.SetZOrder(0.5f);
  skill_icons_.push_back(sprite);
}

//削除処理
void SkillSelectUI::RemoveSkillUI(i32 index_num) {
  skill_icons_.erase(skill_icons_.begin() + index_num);
  //表示位置の設定
  const float icon_distance_ = 64.0f;
  for (i32 i = 0; i < skill_icons_.size(); i++) {
    skill_icons_[i].SetPosition(math::Vector2(
        icon_base_position_.x + icon_distance_ * i, icon_base_position_.y));
  }
}

//選択中かを取得
bool SkillSelectUI::GetIsSelectMode() const { return is_select_mode_; }

//選択モードの切替
void SkillSelectUI::ChangeIsSelectMode() {
  if (skill_icons_.size() == 0) return;

  auto& audio = game::GameDevice::GetInstance()->GetAudioManager();
  if (is_select_mode_) {
    is_select_mode_ = false;
    skill_select_frame_.SetScale(math::Vector2::kZeroVector);
    audio.Start(resource_name::audio::PLAYER_RETURN_FROM_SELECT_SKILL_MODE,
                1.0f);
  } else {
    is_select_mode_ = true;
    skill_select_frame_.SetPosition(skill_icons_[select_number_].GetPosition());
    skill_select_frame_.SetScale(icon_scale_);
    audio.Start(resource_name::audio::PLAYER_MOVE_SELECT_SKILL_MODE, 1.0f);
  }
}

//選択中のスキル番号を取得
i32 SkillSelectUI::GetSkillNumber() { return select_number_; }

//選択中のスキル番号更新
void SkillSelectUI::SelectSkillNumber(i32 select_number) {
  if (skill_icons_.size() == 0) {
    skill_select_frame_.SetScale(math::Vector2(0, 0));
    is_select_mode_ = false;
    return;
  }

  if (select_number_ == select_number) return;
  select_number_ = select_number;

  skill_select_frame_.SetPosition(skill_icons_[select_number_].GetPosition());
  auto& audio = game::GameDevice::GetInstance()->GetAudioManager();
  audio.Start(resource_name::audio::SKILL_SELECT, 1.0f);
}

//説明画像の追加
void SkillSelectUI::AddSkillExplanatory(
    std::shared_ptr<directx::buffer::Texture2D> texture) {
  draw::Sprite2D sprite;

  if (!sprite.Init(
          texture,
          directx::descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID)) {
    MY_LOG(L"スキル説明画像の初期化に失敗しました。");
  }
  sprite.SetPosition(math::Vector2(768.0f, 32.0f));
  sprite.SetScale(math::Vector2::kUnitVector);
  sprite.SetRect(math::Rect(0, 0, 1, 1));

  skill_explanatories_.push_back(sprite);
}

//説明画像の削除
void SkillSelectUI::RemoveSkillExplanatory(i32 index_num) {
  skill_explanatories_.erase(skill_explanatories_.begin() + index_num);
}

//使用状態のアイコンへの差し替え
void SkillSelectUI::ChangeSkillIcon(const Skill* skill, i32 index_num) {
  draw::Sprite2D sprite;
  if (!sprite.Init(
          skill->GetIconUsingTexture(),
          directx::descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID)) {
    MY_LOG(L"スキル使用中画像の初期化に失敗しました。");
  }
  sprite.SetPosition(skill_icons_[index_num].GetPosition());
  sprite.SetScale(math::Vector2::kUnitVector);
  sprite.SetRect(math::Rect(0, 0, 1, 1));

  skill_icons_[index_num] = sprite;
}

}  // namespace skill
}  // namespace legend
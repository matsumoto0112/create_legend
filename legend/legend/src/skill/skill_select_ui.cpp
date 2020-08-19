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

  if (!select_skill_frame_.Init(
          game::GameDevice::GetInstance()->GetResource().GetTexture().Get(
              util::resource::resource_names::texture::TEX),
          directx::descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID)) {
    MY_LOG(L"�X�L���I���A�C�R���̏������Ɏ��s���܂����B");
  }
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
  // sprite_renderer.DrawItems(game::GameDevice::GetInstance()
  //                              ->GetDevice()
  //                              .GetCurrentFrameResource()
  //                              ->GetCommandList());

  if (is_select_mode_) sprite_renderer.AddDrawItems(&select_skill_frame_);
}

void SkillSelectUI::AddSkill(/*const Skill* skill*/) {
  draw::Sprite2D sprite;
  if (!sprite.Init(
          game::GameDevice::GetInstance()->GetResource().GetTexture().Get(
              util::resource::resource_names::texture::TEX),
          directx::descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID)) {
    MY_LOG(L"�X�L���擾���̃X�L���A�C�R���̏������Ɏ��s���܂����B");
  }

  //�\���ʒu�̐ݒ�
  const float icon_distance_ = 64.0f;
  sprite.SetPosition(math::Vector2(
      icon_base_position_.x + icon_distance_ * skill_icons_.size(),
      icon_base_position_.y));
  //�\���T�C�Y�̐ݒ�
  sprite.SetScale(icon_scale_);
  sprite.SetRect(math::Rect(0.0f, 0.0f, 1.0f, 1.0f));
  sprite.SetZOrder(0.5f);
  skill_icons_.push_back(sprite);
}

//�폜����
void SkillSelectUI::RemoveSkillUI(i32 index_num) {
  skill_icons_.erase(skill_icons_.begin() + index_num);
  //�\���ʒu�̐ݒ�
  const float icon_distance_ = 64.0f;
  for (i32 i = 0; i < skill_icons_.size(); i++) {
    skill_icons_[i].SetPosition(math::Vector2(
        icon_base_position_.x + icon_distance_ * i, icon_base_position_.y));
  }
}

//�I�𒆂����擾
bool SkillSelectUI::GetIsSelectMode() const { return is_select_mode_; }

//�I�����[�h�̐ؑ�
void SkillSelectUI::ChangeIsSelectMode() {
  if (skill_icons_.size() == 0) return;

  if (is_select_mode_) {
    is_select_mode_ = false;
    select_skill_frame_.SetScale(math::Vector2(0, 0));
  } else {
    is_select_mode_ = true;
    SelectSkillNumber(select_number_);
    select_skill_frame_.SetScale(icon_scale_);
  }
}

//�I�𒆂̃X�L���ԍ����擾
i32 SkillSelectUI::GetSkillNumber() { return select_number_; }

//�I�𒆂̃X�L���X�V
void SkillSelectUI::SelectSkillNumber(i32 select_number) {
  if (skill_icons_.size() == 0) {
    select_skill_frame_.SetScale(math::Vector2(0, 0));
    is_select_mode_ = false;
    return;
  }

  if (select_number < 0)
    select_number = static_cast<i32>(skill_icons_.size() - 1);
  if (select_number >= skill_icons_.size()) select_number = 0;

  select_number_ = select_number;

  select_skill_frame_.SetPosition(skill_icons_[select_number_].GetPosition());
  select_skill_frame_.SetScale(icon_scale_ / 2);
  select_skill_frame_.SetRect(math::Rect(0, 0, 1, 1));
}

}  // namespace skill
}  // namespace legend
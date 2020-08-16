#include "skill_pencil.h"

#include "src/game/game_device.h"
#include "src/util/resource/resource_names.h"
#include "src\\stdafx.h"

namespace legend {
namespace skill {

SkillPencil::SkillPencil() {
  //�e�X�e�[�^�X�̏����l��ݒ�
}

SkillPencil::~SkillPencil() {}

void SkillPencil::Init(const player::Player& player) {
  if (!Parent::InitBuffer()) {
    return;
  }
  //! �K��g�p�\��
  usable_count_ = 1;
  //! �c��g�p�\��
  remaining_usable_count_ = usable_count_;
  //! �Ďg�p�܂ŋK��̃^�[����
  recast_turn_ = 1;
  //! �c��Ďg�p�܂ł̃^�[����
  remaining_recast_turn_ = 0;
  //! �X�L���̔����^�C�~���O
  activetion_timing_ = SkillActivationTiming::NOW;
  //! �X�L���̌��ʏI���^�C�~���O
  end_timing_ = SkillEffectEndTiming::NOW;
  //! �g�p����Ă��邩�̃t���O
  is_use_ = false;

  player_ = &player;

  transform_.SetPosition(player.GetPosition());
  transform_.SetRotation(player.GetRotation());
  transform_.SetScale(math::Vector3(1, 1, 1));
  collision_.SetPosition(transform_.GetPosition());
  collision_.SetRotation(transform_.GetRotation());
  collision_.SetScale(transform_.GetScale());
  collision_.SetLength(transform_.GetScale());

  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& resource = game::GameDevice::GetInstance()->GetResource();

  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();
  //���f���̏�����
  model_ = resource.GetModel().Get(
      util::resource::resource_names::model::STATIONARY_01);
}

bool SkillPencil::Update() {
  if (player_ == nullptr) {
    return false;
  }

  transform_.SetPosition(player_->GetPosition());
  transform_.SetRotation(player_->GetRotation());
  transform_.SetScale(math::Vector3(1, 1, 1));
  collision_.SetPosition(transform_.GetPosition());
  collision_.SetRotation(transform_.GetRotation());
  collision_.SetScale(transform_.GetScale());
  collision_.SetLength(transform_.GetScale());

  return true;
}

void SkillPencil::Draw() { actor::Actor::Draw(); }

void SkillPencil::Use() {
  is_use_ = true;
  remaining_usable_count_--;
  Action();
}

void SkillPencil::Action() { is_production_ = true; }

void SkillPencil::ProductionUpdate() {}

void SkillPencil::EndAction() {}

}  // namespace skill
}  // namespace legend

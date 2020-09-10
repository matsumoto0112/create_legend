#include "src/skill/skill_paste_stick.h"

#include "src/game/game_device.h"
#include "src/player/player.h"
#include "src/util/resource/resource_names.h"

namespace legend {
namespace skill {
namespace resource_name = util::resource::resource_names;

//�R���X�g���N�^
SkillPasteStick::SkillPasteStick() {}

//�f�X�g���N�^
SkillPasteStick::~SkillPasteStick() {}

//������
bool SkillPasteStick::Init(actor::IActorMediator* mediator,
                           player::Player* player) {
  if (!Parent::Init(mediator, player)) {
    return false;
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
  activetion_timing_ = SkillActivationTiming::PLAYER_TURN_END;
  //! �X�L���̌��ʏI���^�C�~���O
  end_timing_ = SkillEffectEndTiming::ENEMY_TURN_END;
  //! �g�p����Ă��邩�̃t���O
  is_use_ = false;
  is_production_ = false;
  instance_count_ = 6;
  elapsed_time_ = 0.0f;

  player_ = player;

  SetName(skill_name::SKILLPASTESTICK);
  transform_.SetPosition(player->GetPosition());
  transform_.SetRotation(player->GetRotation());
  transform_.SetScale(math::Vector3::kUnitVector);
  bullet::BoundingBox::InitializeParameter params;
  params.position = transform_.GetPosition();
  params.rotation = transform_.GetRotation();
  params.scale = math::Vector3(0.5f, 0.5f, 5.0f);
  params.mass = 0.0f;

  box_ = std::make_shared<bullet::BoundingBox>(this, params);
  box_->SetFlags(box_->GetFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
  mediator_->AddCollider(box_);

  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& resource = game::GameDevice::GetInstance()->GetResource();

  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();
  //���f���̏�����
  model_ = resource.GetModel().Get(resource_name::model::STATIONERY_02);

  //�X�L���A�C�R���̃e�N�X�`���ݒ�
  skill_icon_texture_ =
      resource.GetTexture().Get(resource_name::texture::UI_SKILL_ICON_2);

  //�X�L���g�p�A�C�R���̃e�N�X�`���ݒ�
  skill_icon_using_texture_ =
      resource.GetTexture().Get(resource_name::texture::UI_SKILL_ICON_2_USING);

  //�X�L�������̃e�N�X�`���ݒ�
  skill_explanation_texture_ =
      resource.GetTexture().Get(resource_name::texture::UI_SKILL_EXPLANATION_2);

  //�����p�e�N�X�`���ݒ�
  equipment_texture_ =
      resource.GetTexture().Get(resource_name::texture::UI_PLAYERFORM_SKILL_2);

  return true;
}

//�X�V
bool SkillPasteStick::Update() {
  if (player_ == nullptr) {
    return false;
  }

  for (auto&& paste : pastes_) {
    paste->Update();
  }

  return true;
}

//�`��
void SkillPasteStick::Draw() {
  for (auto&& paste : pastes_) {
    paste->Draw();
  }

  actor::Actor::Draw();
}

//�g�p
void SkillPasteStick::Use() { is_use_ = true; }

//����
void SkillPasteStick::Action() {
  //���o��Ԃւ̐؂�ւ�莞�ɍ��W���ړ�
  if (!is_production_) {
    math::Vector3 position =
        player_->GetPosition() + math::Vector3::kUpVector * 1.5f;
    transform_.SetPosition(position);
    box_->SetTransform(transform_);

    is_production_ = true;
    mediator_->PlayerSkillActivate();
  }

  ProductionUpdate();
}

//���o�̍X�V
void SkillPasteStick::ProductionUpdate() {
  float update_time =
      game::GameDevice::GetInstance()->GetFPSCounter().GetDeltaSeconds<float>();
  // y���ɉ�]��������
  math::Vector3 angle(0.0f, 360.0f, .0f);
  math::Quaternion rotation = math::Quaternion::FromEular(
      (angle * update_time) * math::util::DEG_2_RAD);
  transform_.SetRotation(transform_.GetRotation() * rotation);
  box_->SetTransform(transform_);

  // 1�b�Ԃ�360�x��]����̂ŁA1�b�o�ߌ�ɐ�������
  elapsed_time_ += update_time;
  if (elapsed_time_ >= 1.0f) {
    for (i32 i = 0; i < instance_count_; i++) {
      math::Vector3 position;
      if (i == 0) {
        position = (math::Vector3::kRightVector + math::Vector3::kForwardVector)
                       .Normalized() *
                   10;
      } else if (i == 1) {
        position = math::Vector3::kRightVector.Normalized() * 10;
      } else if (i == 2) {
        position =
            (math::Vector3::kRightVector + math::Vector3::kBackwardVector)
                .Normalized() *
            10;
      } else if (i == 3) {
        position = (math::Vector3::kLeftVector + math::Vector3::kBackwardVector)
                       .Normalized() *
                   10;
      } else if (i == 4) {
        position = math::Vector3::kLeftVector.Normalized() * 10;
      } else {
        position = (math::Vector3::kLeftVector + math::Vector3::kForwardVector)
                       .Normalized() *
                   10;
      }

      position += player_->GetTransform().GetPosition();
      position.y = 1.01f;
      std::shared_ptr<SkillPaste> paste = std::make_shared<SkillPaste>();
      paste->Init(position, mediator_);
      pastes_.emplace_back(std::move(paste));
    }
    mediator_->PlayerSkillDeactivate();
    is_production_ = false;
  }
}

//�I��
void SkillPasteStick::EndAction() {
  remaining_usable_count_--;
  for (auto&& paste : pastes_) {
    paste->Destroy(mediator_);
  }
  if (remaining_usable_count_ <= 0) mediator_->RemoveActor(this);
}
}  // namespace skill
}  // namespace legend
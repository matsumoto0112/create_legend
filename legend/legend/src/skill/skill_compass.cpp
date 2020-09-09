#include "src/skill/skill_compass.h"

#include "src/enemy/boss.h"
#include "src/enemy/enemy.h"
#include "src/game/game_device.h"
#include "src/player/player.h"
#include "src/util/resource/resource_names.h"

namespace legend {
namespace skill {
namespace resource_name = util::resource::resource_names;

//�R���X�g���N�^
SkillCompass::SkillCompass() {}

//�f�X�g���N�^
SkillCompass::~SkillCompass() {}

//������
bool SkillCompass::Init(actor::IActorMediator* mediator,
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
  activetion_timing_ = SkillActivationTiming::NOW;
  //! �X�L���̌��ʏI���^�C�~���O
  end_timing_ = SkillEffectEndTiming::NOW;
  //! �g�p����Ă��邩�̃t���O
  is_use_ = false;
  is_production_ = false;
  flip_off_power_ = 3.0f;
  elapsed_time_ = 0.0f;

  player_ = player;

  SetName(skill_name::SKILLCOMPASS);
  transform_.SetPosition(player->GetPosition() + math::Vector3::kUpVector);
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
  model_ = resource.GetModel().Get(resource_name::model::STATIONERY_04_CLOSE);

  //�X�L���A�C�R���̃e�N�X�`���ݒ�
  skill_icon_texture_ =
      resource.GetTexture().Get(resource_name::texture::UI_SKILL_ICON_4);

  //�X�L�������̃e�N�X�`���ݒ�
  skill_explanation_texture_ =
      resource.GetTexture().Get(resource_name::texture::UI_SKILL_EXPLANATION_1);

  //�����p�e�N�X�`���ݒ�
  equipment_texture_ =
      resource.GetTexture().Get(resource_name::texture::UI_PLAYERFORM_SKILL_1);

  return true;
}

//�X�V
bool SkillCompass::Update() {
  if (player_ == nullptr) {
    return false;
  }

  if (is_production_) {
    ProductionUpdate();
    return true;
  }

  return false;
}

//�`��
void SkillCompass::Draw() { actor::Actor::Draw(); }

//�g�p
void SkillCompass::Use() {
  is_use_ = true;
  Action();
}

//����
void SkillCompass::Action() {
  is_production_ = true;
  //�v���C���[�̐^��Ɉړ����A����������
  const math::Vector3 angle(-90.0f, 0.0f, 0.0f);
  math::Vector3 position =
      transform_.GetPosition() + math::Vector3::kUpVector * 6.0f;
  transform_.SetPosition(position);
  math::Quaternion rotation =
      math::Quaternion::FromEular(angle * math::util::DEG_2_RAD);
  transform_.SetRotation(rotation);

  auto& resource = game::GameDevice::GetInstance()->GetResource();
  //�J�����R���p�X�Ƀ��f����ύX
  model_ = resource.GetModel().Get(resource_name::model::STATIONERY_04_OPEN);
  mediator_->PlayerSkillActivate();

  //��x�R���C�_�[���폜���āA�V���ɐݒ肵�ǉ�����
  mediator_->RemoveCollider(box_);
  bullet::BoundingBox::InitializeParameter params;
  params.position = transform_.GetPosition();
  params.rotation = transform_.GetRotation();
  params.scale = math::Vector3(5.0f, 5.0f, 10.0f);
  params.mass = 0.0f;

  box_ = std::make_shared<bullet::BoundingBox>(this, params);
  box_->SetCollisionCallBack([&](bullet::Collider* other) { OnHit(other); });
  box_->SetFlags(box_->GetFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
  mediator_->AddCollider(box_);
}

//���o�X�V
void SkillCompass::ProductionUpdate() {
  float update_time =
      game::GameDevice::GetInstance()->GetFPSCounter().GetDeltaSeconds<float>();
  // x����]�Œ��������Ă���̂ŁAz���ɉ�]��������
  math::Vector3 angle(0.0f, 0.0f, 360.0f);
  math::Quaternion rotation = math::Quaternion::FromEular(
      (angle * update_time * 3) * math::util::DEG_2_RAD);
  transform_.SetRotation(transform_.GetRotation() * rotation);
  box_->SetTransform(transform_);

  // 3�b�o�߂�����A�I��
  elapsed_time_ += update_time;
  if (elapsed_time_ >= 3.0f) {
    EndAction();
  }
}

//�I��
void SkillCompass::EndAction() {
  remaining_usable_count_--;
  is_production_ = false;
  mediator_->PlayerSkillDeactivate();
  if (remaining_usable_count_ <= 0) mediator_->RemoveActor(this);
}

//�Փ˔���
void SkillCompass::OnHit(bullet::Collider* other) {
  {
    enemy::Enemy* enemy = dynamic_cast<enemy::Enemy*>(other->GetOwner());
    if (enemy) {
      math::Vector3 explosion_position = transform_.GetPosition();
      math::Vector3 enemy_position = enemy->GetTransform().GetPosition();
      math::Vector3 direction =
          (enemy_position - explosion_position).Normalized();
      direction = direction * flip_off_power_;
      //�l�͓K��
      direction.y = 2.0f;

      enemy->GetCollider()->ApplyCentralImpulse(direction);
    }
  }
  {
    enemy::Boss* boss = dynamic_cast<enemy::Boss*>(other->GetOwner());
    if (boss) {
      math::Vector3 explosion_position = transform_.GetPosition();
      math::Vector3 boss_position = boss->GetTransform().GetPosition();
      math::Vector3 direction =
          (boss_position - explosion_position).Normalized();
      direction = direction * flip_off_power_;
      //�l�͓K��
      direction.y = 2.0f;

      boss->GetCollider()->ApplyCentralImpulse(direction * 0.5f);
    }
  }
}
}  // namespace skill
}  // namespace legend
#include "src/skill/skill_pencil.h"

#include "src/enemy/boss.h"
#include "src/enemy/enemy.h"
#include "src/game/game_device.h"
#include "src/object/desk.h"
#include "src/object/obstacle.h"
#include "src/player/player.h"
#include "src/util/resource/resource_names.h"

namespace legend {
namespace skill {
namespace resource_name = util::resource::resource_names;

//�R���X�g���N�^
SkillPencil::SkillPencil() {
  //�e�X�e�[�^�X�̏����l��ݒ�
}

//�f�X�g���N�^
SkillPencil::~SkillPencil() {}

//������
bool SkillPencil::Init(actor::IActorMediator* mediator,
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
  is_explosion_ = false;

  player_ = player;

  SetName(skill_name::SKILLPENCIL);
  transform_.SetPosition(player->GetPosition() + math::Vector3::kUpVector);
  transform_.SetRotation(player->GetRotation());
  transform_.SetScale(math::Vector3(1, 1, 1));

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
  model_ = resource.GetModel().Get(resource_name::model::STATIONERY_01);

  //�X�L���A�C�R���̃e�N�X�`���ݒ�
  skill_icon_texture_ =
      resource.GetTexture().Get(resource_name::texture::UI_SKILL_ICON_1);

  //�X�L�������̃e�N�X�`���ݒ�
  skill_explanation_texture_ =
      resource.GetTexture().Get(resource_name::texture::UI_SKILL_EXPLANATION_1);

  //�����p�e�N�X�`���ݒ�
  equipment_texture_ =
      resource.GetTexture().Get(resource_name::texture::UI_PLAYERFORM_SKILL_1);

  return true;
}

//�X�V
bool SkillPencil::Update() {
  if (player_ == nullptr) {
    return false;
  }

  if (is_explosion_) {
    ExplosionUpdate();
    return true;
  }

  if (is_production_) {
    ProductionUpdate();
    return true;
  }

  return true;
}

//�`��
void SkillPencil::Draw() {
  if (!is_explosion_) actor::Actor::Draw();
}

//�X�L���̎g�p
void SkillPencil::Use() {
  is_use_ = true;
  Action();
  auto& audio = game::GameDevice::GetInstance()->GetAudioManager();
  audio.Start(util::resource::resource_names::audio::SKILL_PENCIL_SHOT, 1.0f);
}

//����
void SkillPencil::Action() {
  is_production_ = true;
  //�X�L�����g�p�����^�C�~���O�Ŏˏo�������擾����
  shoot_theta_ =
      mediator_->GetMainCameraThetaAngle() + math::util::DEG_2_RAD * 90.0f;
  mediator_->PlayerSkillActivate();
  math::Vector3 position =
      transform_.GetPosition() + math::Vector3::kUpVector * 2.0f;
  transform_.SetPosition(position);

  //��x�R���C�_�[���폜���āA�V���ɐݒ肵�ǉ�����
  mediator_->RemoveCollider(box_);
  bullet::BoundingBox::InitializeParameter params;
  params.position = transform_.GetPosition();
  params.rotation = transform_.GetRotation();
  params.scale = math::Vector3(0.5f, 0.5f, 5.0f);
  params.mass = 0.1f;

  box_ = std::make_shared<bullet::BoundingBox>(this, params);
  box_->SetCollisionCallBack([&](bullet::Collider* other) { OnHit(other); });
  box_->SetFlags(box_->GetFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
  mediator_->AddCollider(box_);
  math::Vector3 velocity = math::Matrix4x4::MultiplyCoord(
      math::Vector3::kForwardVector + math::Vector3::kUpVector,
      math::Matrix4x4::CreateRotationY(-shoot_theta_));
  //�R���C�_�[�𐳖ʏ�����ɒe��
  box_->ApplyCentralImpulse(velocity);
}

//���o�̍X�V
void SkillPencil::ProductionUpdate() {
  math::Vector3 target = box_->GetVelocity() - transform_.GetPosition();
  transform_.SetRotation(LookAt(target));
  box_->SetTransform(transform_);

  if (transform_.GetPosition().y <= -2.0f) EndAction();
}

//�I��
void SkillPencil::EndAction() {
  remaining_usable_count_--;
  is_production_ = false;
  mediator_->PlayerSkillDeactivate();
  if (is_explosion_) explosion_pencil_->Destroy(mediator_);
  if (remaining_usable_count_ <= 0) mediator_->RemoveActor(this);
}

//�Փ˔���
void SkillPencil::OnHit(bullet::Collider* other) {
  if (!is_production_) return;

  //�G���G�A�{�X�A���A��Q���ɓ��������甚��
  if (dynamic_cast<enemy::Enemy*>(other->GetOwner()) ||
      dynamic_cast<enemy::Boss*>(other->GetOwner()) ||
      dynamic_cast<object::Desk*>(other->GetOwner()) ||
      dynamic_cast<object::Obstacle*>(other->GetOwner())) {
    //�Փ˓_���甚��������
    util::Transform transform = transform_;
    transform.SetPosition(box_->GetHitPositions().at(other));
    Explosion(transform);
  }
}

//�����J�n
void SkillPencil::Explosion(util::Transform transform) {
  if (is_explosion_) return;
  auto& audio = game::GameDevice::GetInstance()->GetAudioManager();

  explosion_timer_.Init(2.0f, [&]() { EndAction(); });

  //���͂̓G�𐁂���΂�����
  is_explosion_ = true;
  audio.Start(resource_name::audio::SKILL_PENCIL_HIT, 1.0f);

  explosion_pencil_ = std::make_shared<ExplosionPencil>();
  explosion_pencil_->Init(transform, mediator_);
}

//�����X�V
void SkillPencil::ExplosionUpdate() {
  //�������͍X�V
  if (!explosion_timer_.Update()) explosion_pencil_->Update();
}

math::Quaternion SkillPencil::LookAt(const math::Vector3& target) {
  const float dot =
      math::Vector3::Dot(math::Vector3::kForwardVector, target.Normalized());
  const float theta = math::util::Acos(dot);
  const math::Vector3 cross =
      math::Vector3::Cross(math::Vector3::kForwardVector, target).Normalized();

  return math::Quaternion(cross, theta);
}

}  // namespace skill
}  // namespace legend

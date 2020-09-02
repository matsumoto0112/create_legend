#include "skill_item_box.h"

#include "src/directx/shader/shader_register_id.h"
#include "src/draw/particle/particle_factory.h"
#include "src/game/game_device.h"
#include "src/player/player.h"
#include "src/util/resource/resource_names.h"

namespace legend {
namespace skill {
//�R���X�g���N�^
SkillItemBox::SkillItemBox() : Parent(L"ItemBox") {}

//�f�X�g���N�^
SkillItemBox::~SkillItemBox() { move_particle_->Delete(); }

//������
bool SkillItemBox::Init(actor::IActorMediator* mediator,
                        const InitializeParameter& parameter,
                        std::shared_ptr<Skill> skill) {
  if (!Parent::Init(mediator)) {
    return false;
  }

  this->transform_ = parameter.transform;

  //�R���C�_�[�̐ݒ�
  bullet::BoundingBox::InitializeParameter params;
  params.position = transform_.GetPosition();
  params.rotation = transform_.GetRotation();
  params.scale = parameter.bounding_box_length;

  // TODO: ������ӂ��������p�����[�^�ɂ����
  params.mass = 0.0f;
  box_ = std::make_shared<bullet::BoundingBox>(this, params);
  mediator_->AddCollider(box_);
  box_->SetFlags(box_->GetFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);

  delete_time_.Init(1.0f);
  skill_ = skill;
  is_dead_ = false;

  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();

  auto& resource = game::GameDevice::GetInstance()->GetResource();
  model_ = resource.GetModel().Get(
      util::resource::resource_names::model::ITEM_CAPSULE_01);

  skill_icon_model_ = parameter.skill_icon_model;

  velocity_ = math::Vector3::kUpVector;

  move_particle_ = draw::particle::particle_factory::CreatePlayerMoveParticle();
  move_particle_->GetTransform().SetPosition(transform_.GetPosition());
  move_particle_->SetEmitEnable(false);

  return true;
}

//�X�V
bool SkillItemBox::Update() {
  if (is_dead_) {
    //���ɔ�΂�
    util::Transform transform = transform_;
    transform.SetPosition(transform.GetPosition() + velocity_);
    box_->SetTransform(transform);

    auto ParticleUpdate = [&]() {
      const math::Vector3 MOVE_PARTICLE_OFFSET = velocity_.Normalized() * -3;
      const math::Vector3 move_particle_position =
          transform_.GetPosition() + MOVE_PARTICLE_OFFSET;
      move_particle_->GetTransformRef().SetPosition(move_particle_position);
      move_particle_->SetEmitEnable(true);
    };

    ParticleUpdate();

    //���̍����܂ōs������폜
    if (transform_.GetPosition().y > 50.0f) {
      if (delete_time_.Update()) {
        move_particle_->Delete();
        if (box_) mediator_->RemoveCollider(box_);
        mediator_->RemoveActor(this);
        return true;
      }
    }
  }

  return true;
}
//�`��
void SkillItemBox::Draw() {
  //���̍����܂ōs������`�悵�Ȃ�
  if (transform_.GetPosition().y > 100.0f) return;
  Parent::Draw();

  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& resource = game::GameDevice::GetInstance()->GetResource();
  auto& command_list = device.GetCurrentFrameResource()->GetCommandList();
  resource.GetPipeline().Get(PIPELINE_NAME)->SetCommandList(command_list);

  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();
  transform_cb_.RegisterHandle(
      device, directx::shader::ConstantBufferRegisterID::TRANSFORM);

  skill_icon_model_->Draw(command_list);
}

//���S����̕ύX
void SkillItemBox::ChangeDead() { is_dead_ = true; }

//���S����̎擾
bool SkillItemBox::GetIsDead() const { return is_dead_; }

//�ݒ肳�ꂽ�X�L���̎擾
std::shared_ptr<Skill> SkillItemBox::GetSkill() const { return skill_; }
}  // namespace skill
}  // namespace legend
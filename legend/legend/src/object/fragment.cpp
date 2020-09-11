#include "src/object/fragment.h"

#include "src/game/game_device.h"
#include "src/util/resource/resource_names.h"

namespace legend {
namespace object {
//�R���X�g���N�^
Fragment::Fragment() : Parent(L"Fragment") {}

//�f�X�g���N�^
Fragment::~Fragment() {}

//������
bool Fragment::Init(actor::IActorMediator* mediator,
                    const InitializeParameter& parameter) {
  if (!Parent::Init(mediator)) {
    return false;
  }

  this->transform_ =
      util::Transform(parameter.position, parameter.rotation, parameter.scale);

  bullet::BoundingBox::InitializeParameter params;
  params.position = this->transform_.GetPosition();
  params.rotation = this->transform_.GetRotation();
  params.scale = parameter.bounding_box_length;
  params.mass = 0.0f;
  params.restitution = 0.0f;
  params.friction = 0.0f;
  box_ = std::make_shared<bullet::BoundingBox>(this, params);
  mediator_->AddCollider(box_);
  box_->SetFlags(box_->GetFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);

  is_dead_ = false;
  dead_time_.Init(0.5f);

  auto& resource = game::GameDevice::GetInstance()->GetResource();
  model_ = resource.GetModel().Get(
      util::resource::resource_names::model::ERASER_FRAGMENT_01);
  return true;
}

//�X�V
bool Fragment::Update() {
  if (is_dead_) {
    if (dead_time_.Update()) {
      if (box_) mediator_->RemoveCollider(box_);
      mediator_->RemoveActor(this);
    }
  }
  return true;
}

//�`��
void Fragment::Draw() {
  if (is_dead_) return;
  Parent::Draw();
}

//���S����̐؂�ւ�
void Fragment::ChangeDead() { is_dead_ = true; }

//���S����̎擾
bool Fragment::GetIsDead() const { return is_dead_; }

}  // namespace object
}  // namespace legend

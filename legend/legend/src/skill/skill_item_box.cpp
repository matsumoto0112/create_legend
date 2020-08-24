#include "skill_item_box.h"

#include "src/game/game_device.h"
#include "src/player/player.h"
#include "src/util/resource/resource_names.h"

namespace legend {
namespace skill {
//コンストラクタ
SkillItemBox::SkillItemBox() : Parent(L"ItemBox") {}

//デストラクタ
SkillItemBox::~SkillItemBox() {}

//初期化
bool SkillItemBox::Init(actor::IActorMediator* mediator,
                        const InitializeParameter& parameter,
                        std::shared_ptr<Skill> skill) {
  if (!Parent::Init(mediator)) {
    return false;
  }

  this->transform_ = parameter.transform;

  //コライダーの設定
  bullet::BoundingBox::InitializeParameter params;
  params.position = transform_.GetPosition();
  params.rotation = transform_.GetRotation();
  params.scale = parameter.bounding_box_length;

  // TODO: ここら辺も初期化パラメータにいれる
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
      util::resource::resource_names::model::STATIONERY_01);

  return true;
}

//更新
bool SkillItemBox::Update() {
  if (is_dead_) {
    if (delete_time_.Update()) {
      if (box_) mediator_->RemoveCollider(box_);
      mediator_->RemoveActor(this);
    }
  }

  return true;
}
//描画
void SkillItemBox::Draw() {
  if (is_dead_) return;
  Parent::Draw();
}

//死亡判定の変更
void SkillItemBox::ChangeDead() { is_dead_ = true; }

//死亡判定の取得
bool SkillItemBox::GetIsDead() const { return is_dead_; }

//設定されたスキルの取得
std::shared_ptr<Skill> SkillItemBox::GetSkill() const { return skill_; }
}  // namespace skill
}  // namespace legend
#include "src/player/player.h"

#include "src/bullet/bullet_helper.h"
#include "src/directx/shader/shader_register_id.h"
#include "src/enemy/boss.h"
#include "src/enemy/enemy.h"
#include "src/game/game_device.h"
#include "src/object/desk.h"
#include "src/object/fragment.h"
#include "src/object/graffiti.h"
#include "src/skill/skill.h"
#include "src/skill/skill_item_box.h"
#include "src/util/resource/resource_names.h"

namespace legend {
namespace player {

//コンストラク
Player::Player() : Parent(L"Player") {}

//デストラクタ
Player::~Player() {}

//初期化
bool Player::Init(actor::IActorMediator* mediator,
                  const InitializeParameter& parameter) {
  if (!Parent::Init(mediator)) {
    return false;
  }

  this->transform_ = parameter.transform;

  //コライダーの設定
  bullet::BoundingBox::InitializeParameter params;
  params.position = transform_.GetPosition();
  params.rotation = transform_.GetRotation();
  params.scale = parameter.bouding_box_length;

  // TODO: ここら辺も初期化パラメータにいれる
  params.mass = 1.0f;
  params.friction = 0.8f;
  params.restitution = 1.0f;
  box_ = std::make_shared<bullet::BoundingBox>(this, params);
  box_->SetCollisionCallBack([&](bullet::Collider* other) { OnHit(other); });
  mediator_->AddCollider(box_);

  min_power_ = parameter.min_power;
  max_power_ = parameter.max_power;

  up_power_ = true;
  is_set_power_ = false;
  strength_ = 1.0f;
  max_strength_ = parameter.max_strength;
  min_strength_ = parameter.min_strength;

  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();

  auto& resource = game::GameDevice::GetInstance()->GetResource();
  model_ =
      resource.GetModel().Get(util::resource::resource_names::model::PLAYER);

  //スキルマネージャーの初期化
  skill_manager_.Init(mediator_);

  return true;
}

//更新
bool Player::Update() {
  update_time_ =
      game::GameDevice::GetInstance()->GetFPSCounter().GetDeltaSeconds<float>();

  //スキルのデバック用のGUI
  if (ImGui::Begin("Skill")) {
    if (ImGui::Button("Add Skill")) {
      std::shared_ptr<skill::SkillPencil> skill =
          std::make_shared<skill::SkillPencil>();
      skill->Init(mediator_, this);
      skill_manager_.AddSkill(skill);
    }
  }
  ImGui::End();
  //スキルマネージャーの更新
  skill_manager_.Update();

  // if (game::GameDevice::GetInstance()->GetInput().GetKeyboard()->GetKeyDown(
  //        input::key_code::A) &&
  //    0.05f < input_velocity_.Magnitude()) {
  //  box_->ApplyCentralImpulse(input_velocity_ * power_);
  //}

  if (skill_manager_.IsProductionNow()) {
    return true;
  }

  // if (change_amount_velocity_.Magnitude() - input_velocity_.Magnitude() >=
  //    0.5f) {
  //  is_move_ = true;
  //  mediator_->PlayerMoveStartEvent();
  //}

  if (GetMoveEnd()) {
    ResetParameter();
    mediator_->PlayerMoveEndEvent();
  }
  return true;
}

void Player::Draw() {
  //プレイヤーの描画
  actor::Actor::Draw();
  //スキルマネージャーの描画
  skill_manager_.Draw();
}

//座標の設定
void Player::SetPosition(math::Vector3 position) {
  transform_.SetPosition(position);
}

//回転の設定
void Player::SetRotation(math::Quaternion rotation) {
  transform_.SetRotation(rotation);
}

//スケールの設定
void Player::SetScale(math::Vector3 scale) { transform_.SetScale(scale); }

void Player::CheckImpulse() {
  auto framerate =
      12.5f *
      game::GameDevice::GetInstance()->GetFPSCounter().GetDeltaSeconds<float>();
  if (!is_move_) {
    input::InputManager& input = game::GameDevice::GetInstance()->GetInput();
    input_velocity_.x = -input.GetGamepad()->GetStickLeft().x;
    input_velocity_.z = -input.GetGamepad()->GetStickLeft().y;

    if (0.2f <= input_velocity_.Magnitude()) {
      is_input_ = true;
    }

    if ((change_amount_velocity_.Magnitude() < input_velocity_.Magnitude()) ||
        (change_amount_velocity_ - input_velocity_).Magnitude() <= framerate) {
      change_amount_velocity_ = input_velocity_;
    }
    // velocity_update_time_ += update_time_;
    // if (!(velocity_update_time_ < change_time_)) {
    //  velocity_ = input_velocity_;

    //  // float thita = math::util::Atan2(velocity_.z, velocity_.x);
    //  // SetRotation(math::Quaternion(0, thita, 0, 1));
    //  if (velocity_.Magnitude() >= 0.2f) is_input_ = true;

    //  change_amount_velocity_ = velocity_;
    //  velocity_update_time_ = 0;
    //}
  }

  auto iv_mag = input_velocity_.Magnitude();
  auto cav_mag = change_amount_velocity_.Magnitude();
  auto vector = (change_amount_velocity_ - input_velocity_);
  auto vec_mag = vector.Magnitude();

  if (is_input_ && (iv_mag < cav_mag) && (iv_mag < framerate) &&
      (framerate < vec_mag)) {
    is_move_ = true;

    auto vel = vector.Normalized() * power_ * impulse_;
    box_->ApplyCentralImpulse(vel);
    mediator_->PlayerMoveStartEvent();
  }
  SetImpulse();
}

//速度の設定
void Player::SetVelocity(math::Vector3 velocity) {
  if (!is_move_) return;

  box_->ApplyCentralImpulse(velocity);
  // velocity_ = velocity;
}

//パワーの設定
void Player::SetImpulse() {
  if (is_set_power_ || !is_input_) return;

  input::InputManager& input = game::GameDevice::GetInstance()->GetInput();
  if (input.GetGamepad()->GetStickLeft().Magnitude() >= 0.2f) {
    if (up_power_) {
      impulse_ += 0.1f * update_time_ * 5;
      if (impulse_ >= max_power_) {
        impulse_ = max_power_;
        up_power_ = false;
      }
    } else {
      impulse_ -= 0.1f * update_time_ * 5;
      if (impulse_ <= min_power_) {
        impulse_ = min_power_;
        up_power_ = true;
      }
    }
  } else {
    is_set_power_ = true;
  }
}

//移動に必要なパラメータを初期化
void Player::ResetParameter() {
  is_input_ = false;
  is_set_power_ = false;
  impulse_ = 0;
  up_power_ = true;
  change_amount_velocity_ = math::Vector3::kZeroVector;
  input_velocity_ = math::Vector3::kZeroVector;
  is_move_ = false;
  velocity_update_time_ = 0;
}

//座標の取得
math::Vector3 Player::GetPosition() const { return transform_.GetPosition(); }

//スケールの取得
math::Vector3 Player::GetScale() const { return transform_.GetScale(); }

//移動量の取得
math::Vector3 Player::GetVelocity() const { return box_->GetVelocity(); }

//回転の取得
math::Quaternion Player::GetRotation() const {
  return transform_.GetRotation();
}

//加える力の割合の取得
float Player::GetImpulse() const { return impulse_; }

//加える力の取得
float Player::GetPower() const { return power_; }

//移動を終えているかを取得
bool Player::GetMoveEnd() const {
  bool is_end = (is_move_ && GetVelocity() == math::Vector3::kZeroVector);
  return is_end;
}

//移動判定の取得
bool Player::GetIsMove() const { return is_move_; }

//強化パラメータの更新
void Player::UpdateStrength(const float& add_strength) {
  strength_ += add_strength;
  if (strength_ > max_strength_)
    strength_ = max_strength_;
  else if (strength_ < min_strength_)
    strength_ = min_strength_;
}

//強化度合いを取得
float Player::GetStrength() const { return strength_; }

bool Player::GetSkillSelect() {
  if (is_input_) return false;

  return skill_manager_.SelectSkill();
}

void Player::OnHit(bullet::Collider* other) {
  //敵に触れた
  {
    enemy::Enemy* e = dynamic_cast<enemy::Enemy*>(other->GetOwner());
    if (e) {
      const math::Vector3 player_position = transform_.GetPosition();
      const math::Vector3 enemy_position = e->GetTransform().GetPosition();
      const math::Vector3 direction =
          (enemy_position - player_position).Normalized();

      e->GetCollider()->ApplyCentralImpulse(direction * power_);
    }
  }
  //ボスに触れた
  {
    enemy::Boss* b = dynamic_cast<enemy::Boss*>(other->GetOwner());
    if (b) {
      const math::Vector3 player_position = transform_.GetPosition();
      const math::Vector3 boss_position = b->GetTransform().GetPosition();
      const math::Vector3 direction =
          (boss_position - player_position).Normalized();

      b->GetCollider()->ApplyCentralImpulse(direction * power_);
    }
  }
  //スキルアイテムボックスに触れた
  {
    skill::SkillItemBox* skill_item =
        dynamic_cast<skill::SkillItemBox*>(other->GetOwner());
    if (skill_item) {
      if (!skill_item->GetIsDead()) {
        skill_item->ChangeDead();
        std::shared_ptr<skill::Skill> skill = skill_item->GetSkill();
        skill->Init(mediator_, this);
        skill_manager_.AddSkill(skill);
      }
    }
  }
  {
    object::Fragment* fragment =
        dynamic_cast<object::Fragment*>(other->GetOwner());
    if (fragment) {
      if (!fragment->GetIsDead()) {
        fragment->ChangeDead();
        UpdateStrength(0.01f);
      }
    }
  }
}

}  // namespace player
}  // namespace legend
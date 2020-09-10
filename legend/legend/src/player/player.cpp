#include "src/player/player.h"

#include "src/bullet/bullet_helper.h"
#include "src/directx/shader/shader_register_id.h"
#include "src/draw/particle/particle_factory.h"
#include "src/enemy/boss.h"
#include "src/enemy/enemy.h"
#include "src/enemy/enemy_actor.h"
#include "src/game/game_device.h"
#include "src/object/desk.h"
#include "src/object/fragment.h"
#include "src/object/graffiti.h"
#include "src/skill/skill.h"
#include "src/skill/skill_item_box.h"
#include "src/system/mode.h"
#include "src/system/turn_system.h"
#include "src/util/resource/resource_names.h"

namespace legend {
namespace player {
namespace resource_name = util::resource::resource_names;

//コンストラク
Player::Player() : Parent(L"Player") {}

//デストラクタ
Player::~Player() { player_move_particle_->Delete(); }

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
  params.mass = parameter.mass;
  params.friction = parameter.friction;
  params.restitution = parameter.restitution;
  box_ = std::make_shared<bullet::BoundingBox>(this, params);
  box_->SetCollisionCallBack([&](bullet::Collider* other) { OnHit(other); });
  mediator_->AddCollider(box_);

  min_power_ = parameter.min_power;
  max_power_ = parameter.max_power;

  up_power_ = true;
  is_set_power_ = false;
  is_hitstop_ = false;
  strength_ = 1.0f;
  max_strength_ = parameter.max_strength;
  min_strength_ = parameter.min_strength;

  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();

  auto& resource = game::GameDevice::GetInstance()->GetResource();
  model_ = resource.GetModel().Get(resource_name::model::ERASER_01);

  //スキルマネージャーの初期化
  skill_manager_.Init(mediator_, this);

  is_hit_obstacle_ = false;
  power_se_interval_.Init(0.0f);
  move_start_ = false;
  fall_start_ = false;

  if (!move_direction_.Init()) {
    return false;
  }
  player_move_particle_ =
      draw::particle::particle_factory::CreatePlayerMoveParticle();
  player_move_particle_->SetEmitEnable(false);

  ResetParameter();

  //集中線の初期化
  if (!concentrated_line_.Init(
          resource.GetTexture().Get(resource_name::texture::TITLE_LOGO),
          directx::descriptor_heap::heap_parameter::LocalHeapID::ONE_PLAY)) {
    MY_LOG(L"集中線の画像の初期化に失敗しました。");
    return false;
  }
  concentrated_line_.SetRect(math::Rect(0, 0, 1, 1));
  concentrated_line_.SetZOrder(0.005f);
  concentrated_line_.SetPosition(math::Vector2::kZeroVector);

  return true;
}

//更新
bool Player::Update() {
  update_time_ =
      game::GameDevice::GetInstance()->GetFPSCounter().GetDeltaSeconds<float>();

  //スキルのデバック用のGUI
  if (ImGui::Begin("Skill")) {
    if (ImGui::Button("Add SkillPencil")) {
      if (skill_manager_.GetSkillList().size() < 5) {
        std::shared_ptr<skill::SkillPencil> skill =
            std::make_shared<skill::SkillPencil>();
        skill->Init(mediator_, this);
        skill_manager_.AddSkill(skill);
      }
    }
    if (ImGui::Button("Add SkillPasteStick")) {
      if (skill_manager_.GetSkillList().size() < 5) {
        std::shared_ptr<skill::SkillPasteStick> skill =
            std::make_shared<skill::SkillPasteStick>();
        skill->Init(mediator_, this);
        skill_manager_.AddSkill(skill);
      }
    }
    if (ImGui::Button("Add SkillCompass")) {
      if (skill_manager_.GetSkillList().size() < 5) {
        std::shared_ptr<skill::SkillCompass> skill =
            std::make_shared<skill::SkillCompass>();
        skill->Init(mediator_, this);
        skill_manager_.AddSkill(skill);
      }
    }
  }
  ImGui::End();

  skill_manager_.Update();
  // if (game::GameDevice::GetInstance()->GetInput().GetKeyboard()->GetKeyDown(
  //        input::key_code::A) &&
  //    0.05f < input_velocity_.Magnitude()) {
  //  box_->ApplyCentralImpulse(input_velocity_ * power_);
  //}

  // if (skill_manager_.IsProductionNow()) {
  //  return true;
  //}

  // if (change_amount_velocity_.Magnitude() - input_velocity_.Magnitude() >=
  //    0.5f) {
  //  is_move_ = true;
  //  mediator_->PlayerMoveStartEvent();
  //}

  auto UpdateMoveDirectionModel = [&]() {
    auto Rad = [](float deg) { return deg * math::util::DEG_2_RAD; };
    //カメラの角度と入力の方向に応じた回転をかけるための角度を計算する
    const float angle = -(mediator_->GetMainCameraThetaAngle() + Rad(90.0f)) +
                        math::util::Atan2(input_velocity_.x, input_velocity_.z);
    const math::Vector3 direction_offset =
        math::Quaternion::FromEular(0, angle, 0) * math::Vector3(0, 0, 7.5f);
    move_direction_.SetPosition(transform_.GetPosition() + direction_offset);
    const math::Quaternion direction_rotation =
        math::Quaternion::FromEular(0.0f, angle + Rad(180.0f), 0.0f);
    move_direction_.SetRotation(direction_rotation);
  };
  UpdateMoveDirectionModel();
  auto ParticleUpdate = [&]() {
    const math::Vector3 MOVE_PARTICLE_OFFSET = GetVelocity().Normalized() * -3;
    const math::Vector3 move_particle_position =
        transform_.GetPosition() + MOVE_PARTICLE_OFFSET;
    player_move_particle_->GetTransformRef().SetPosition(
        move_particle_position);
    const math::Vector3 velocity = GetVelocity();
    const math::Vector3 velocity_xz{velocity.x, 0.0f, velocity.z};
    const bool emit_enable = is_move_ ? velocity_xz.Magnitude() > 0.6f
                                      : math::util::Abs(velocity.y) > 1.0f;
    player_move_particle_->SetEmitEnable(emit_enable);

    if (math::util::Abs(velocity.y) > 0.5f) {
      player_move_particle_->SetTextureName(
          util::resource::resource_names::texture::PARTICLE_FALLLOCUS_1);
    } else {
      player_move_particle_->SetTextureName(
          util::resource::resource_names::texture::PARTICLE_SMOKE_3);
    }
  };

  {
    auto& audio = game::GameDevice::GetInstance()->GetAudioManager();
    if (GetVelocity().y <= -0.5f) {
      if (!fall_start_) {
        audio.Start(resource_name::audio::PLAYER_FALL_OUT, 1.0f);
        fall_start_ = true;
      }
    } else {
      const math::Vector2 velocity(GetVelocity().x, GetVelocity().z);
      if (velocity.Magnitude() > 0.1f) {
        if (!move_start_) {
          audio.Start(resource_name::audio::PLAYER_MOVING, 3.0f);
          move_start_ = true;
        }
      } else {
        move_start_ = false;
      }

      fall_start_ = false;
    }
  }

  ParticleUpdate();
  obstacle_hit_timer_.Update();
  if (GetMoveEnd()) {
    player_move_particle_->SetEmitEnable(false);
    ResetParameter();

    mediator_->PlayerMoveEndEvent();
  }
  return true;
}  // namespace player

void Player::Draw() {
  //プレイヤーの描画
  actor::Actor::Draw();

  const bool visible_move_direction = input_velocity_.Magnitude() > 0.0f;
  if (visible_move_direction) {
    move_direction_.Draw();
  }

  //スキルマネージャーの描画
  skill_manager_.Draw();

  if (!is_fullpower_) return;

  draw::SpriteRenderer& sprite_renderer =
      game::GameDevice::GetInstance()->GetSpriteRenderer();
  sprite_renderer.AddDrawItems(&concentrated_line_);
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
  auto& audio = game::GameDevice::GetInstance()->GetAudioManager();

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

    //メインカメラの回転角を取得する
    //角度はXY平面上で(1,0)の方角から0としているため、90°回転する必要がある
    float theta =
        mediator_->GetMainCameraThetaAngle() + math::util::DEG_2_RAD * 90.0f;
    vector = vector.Normalized();
    //移動する方向ベクトルにカメラの向きに応じた回転をかけることで、カメラの向いている方向に対した入力値に変換する
    vector = math::Matrix4x4::MultiplyCoord(
        vector, math::Matrix4x4::CreateRotationY(-theta));

    auto vel = vector.Normalized() * power_ * impulse_;
    box_->ApplyCentralImpulse(vel);
    player_move_particle_->ResetParticle();
    player_move_particle_->SetEmitEnable(true);
    input_velocity_ = math::Vector3::kZeroVector;
    mediator_->PlayerMoveStartEvent();
    audio.Start(resource_name::audio::PLAYER_SNAP, 0.8f);
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

  auto& audio = game::GameDevice::GetInstance()->GetAudioManager();
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
    if (power_se_interval_.Update()) {
      audio.Start(resource_name::audio::PLAYER_POWER_CHARGE, 1.0f);
      power_se_interval_.Init(1.0f);
    }
  } else {
    is_fullpower_ = (impulse_ >= 0.9f);
    is_hitstop_ = is_fullpower_;

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
  is_fullpower_ = false;
  power_se_interval_.Init(0.0f);
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

bool Player::GetPlayerDeathFlag() { return transform_.GetPosition().y <= -20; }

void Player::OnHit(bullet::Collider* other) {
  system::Mode turn_mode = mediator_->GetCurrentTurn();
  auto& audio = game::GameDevice::GetInstance()->GetAudioManager();

  if (turn_mode == system::Mode::PLAYER_MOVING) {
    //敵に触れた
    {
      enemy::EnemyActor* ea =
          dynamic_cast<enemy::EnemyActor*>(other->GetOwner());
      if (ea) {
        auto CalcApplyImpulse = [&]() {
          const math::Vector3 position = transform_.GetPosition();
          const math::Vector3 other_position = ea->GetTransform().GetPosition();
          const math::Vector3 velocity = GetVelocity();
          const math::Vector3 direction =
              ((other_position - position).Normalized() + velocity.Normalized())
                  .Normalized();

          const float magnitude = ((strength_ + velocity.Magnitude()) / 2.0f);
          const math::Vector3 base_power = direction * magnitude;

          const float stength_consideration_full_power =
              is_fullpower_ ? strength_ * fullpower_bonus_ : strength_;

          const float clamp_sub_strength =
              math::util::Clamp(strength_ - ea->GetStrength(), 0.0f, 1.0f);
          const float up_power = is_fullpower_
                                     ? clamp_sub_strength * fullpower_bonus_
                                     : clamp_sub_strength;
          auto trigonometric =
              (math::util::Sin(30.0f * math::util::DEG_2_RAD * up_power));
          auto addPower = math::Vector3::kUpVector * GetVelocity().Magnitude() *
                          trigonometric;

          if (is_hitstop_) {
            mediator_->SetStopTime(0.3f);
            is_hitstop_ = false;
          }

          return base_power + addPower;
        };

        other->ApplyCentralImpulse(CalcApplyImpulse());

        if (auto* b = dynamic_cast<enemy::Boss*>(other->GetOwner())) {
          auto v = GetVelocity().Magnitude();
          b->UpdateStrength(v * strength_ * -0.005f);
        }

        //ヒット時の速度の大きさでSE音を適用
        {
          std::wstring file;
          if (GetCollider()->GetVelocity().Magnitude() < 25.0f) {
            file = resource_name::audio::PLAYER_ENEMY_HIT_SMALL;
          } else {
            file = resource_name::audio::PLAYER_ENEMY_HIT_BIG;
            // mediator_->SetStopTime(0.25f);
          }
          audio.Start(file, 1.0f);
        }
        CreateFireParticle(GetCollider()->GetHitPositions().at(other));
      }
    }
    {
      object::Obstacle* obstacle =
          dynamic_cast<object::Obstacle*>(other->GetOwner());
      if (obstacle) {
        if (!is_hit_obstacle_) {
          obstacle_hit_timer_.Init(1.0f, [&]() { is_hit_obstacle_ = false; });
          is_hit_obstacle_ = true;
          audio.Start(resource_name::audio::PLAYER_OBSTACLE_HIT, 0.8f);
          CreateFireParticle(GetCollider()->GetHitPositions().at(other));
        }
      }
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
        skill_manager_.GetSkill(skill);
        audio.Start(resource_name::audio::PLAYER_GET_STATIONERY, 1.0f);
      }
    }
  }
  {
    object::Fragment* fragment =
        dynamic_cast<object::Fragment*>(other->GetOwner());
    if (fragment) {
      if (!fragment->GetIsDead()) {
        fragment->ChangeDead();
        UpdateStrength(0.03f);
        audio.Start(resource_name::audio::PLAYER_GET_FRAGMENT, 1.0f);
      }
    }
  }
}

void Player::SkillUpdate() {
  //スキルマネージャーの更新
  skill_manager_.UseSkill();
  skill_manager_.EndSkill();
  skill_manager_.RemoveSkill();
}

//ターン終了時の処理
bool Player::SkillUpdateTurnEnd() {
  if (mediator_->GetCurrentTurn() == system::Mode::PLAYER_SKILL_AFTER_MOVED) {
    skill_manager_.PlayerTurnEnd();
    skill_manager_.EndSkill();
    skill_manager_.RemoveSkill();
  } else if (mediator_->GetCurrentTurn() == system::Mode::ENEMY_MOVE_END) {
    skill_manager_.EnemyTurnEnd();
    skill_manager_.EndSkill();
    skill_manager_.RemoveSkill();
  }

  return skill_manager_.IsProductionNow();
}

void Player::CreateFireParticle(const util::Transform& transform) {
  auto fire = draw::particle::particle_factory::CreateFireParticle();
  fire->SetTransform(transform);
}

void Player::EquipmentUpdate() { skill_manager_.EquipmentProductionUpdate(); }

void Player::SkillUIChangeEnable(bool enable) {
  skill_manager_.ChangeEnable(enable);
}

}  // namespace player
}  // namespace legend
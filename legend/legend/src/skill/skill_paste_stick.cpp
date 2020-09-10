#include "src/skill/skill_paste_stick.h"

#include "src/game/game_device.h"
#include "src/player/player.h"
#include "src/util/resource/resource_names.h"

namespace legend {
namespace skill {
namespace resource_name = util::resource::resource_names;

//コンストラクタ
SkillPasteStick::SkillPasteStick() {}

//デストラクタ
SkillPasteStick::~SkillPasteStick() {}

//初期化
bool SkillPasteStick::Init(actor::IActorMediator* mediator,
                           player::Player* player) {
  if (!Parent::Init(mediator, player)) {
    return false;
  }
  //! 規定使用可能回数
  usable_count_ = 1;
  //! 残り使用可能回数
  remaining_usable_count_ = usable_count_;
  //! 再使用まで規定のターン数
  recast_turn_ = 1;
  //! 残り再使用までのターン数
  remaining_recast_turn_ = 0;
  //! スキルの発動タイミング
  activetion_timing_ = SkillActivationTiming::PLAYER_TURN_END;
  //! スキルの効果終了タイミング
  end_timing_ = SkillEffectEndTiming::ENEMY_TURN_END;
  //! 使用されているかのフラグ
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
  //モデルの初期化
  model_ = resource.GetModel().Get(resource_name::model::STATIONERY_02);

  //スキルアイコンのテクスチャ設定
  skill_icon_texture_ =
      resource.GetTexture().Get(resource_name::texture::UI_SKILL_ICON_2);

  //スキル使用アイコンのテクスチャ設定
  skill_icon_using_texture_ =
      resource.GetTexture().Get(resource_name::texture::UI_SKILL_ICON_2_USING);

  //スキル説明のテクスチャ設定
  skill_explanation_texture_ =
      resource.GetTexture().Get(resource_name::texture::UI_SKILL_EXPLANATION_2);

  //装備用テクスチャ設定
  equipment_texture_ =
      resource.GetTexture().Get(resource_name::texture::UI_PLAYERFORM_SKILL_2);

  return true;
}

//更新
bool SkillPasteStick::Update() {
  if (player_ == nullptr) {
    return false;
  }

  for (auto&& paste : pastes_) {
    paste->Update();
  }

  return true;
}

//描画
void SkillPasteStick::Draw() {
  for (auto&& paste : pastes_) {
    paste->Draw();
  }

  actor::Actor::Draw();
}

//使用
void SkillPasteStick::Use() { is_use_ = true; }

//発動
void SkillPasteStick::Action() {
  //演出状態への切り替わり時に座標を移動
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

//演出の更新
void SkillPasteStick::ProductionUpdate() {
  float update_time =
      game::GameDevice::GetInstance()->GetFPSCounter().GetDeltaSeconds<float>();
  // y軸に回転を加える
  math::Vector3 angle(0.0f, 360.0f, .0f);
  math::Quaternion rotation = math::Quaternion::FromEular(
      (angle * update_time) * math::util::DEG_2_RAD);
  transform_.SetRotation(transform_.GetRotation() * rotation);
  box_->SetTransform(transform_);

  // 1秒間で360度回転するので、1秒経過後に生成する
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

//終了
void SkillPasteStick::EndAction() {
  remaining_usable_count_--;
  for (auto&& paste : pastes_) {
    paste->Destroy(mediator_);
  }
  if (remaining_usable_count_ <= 0) mediator_->RemoveActor(this);
}
}  // namespace skill
}  // namespace legend
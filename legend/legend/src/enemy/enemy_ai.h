#ifndef LEGEND_ENEMY_ENEMY_AI_H_
#define LEGEND_ENEMY_ENEMY_AI_H_

#include <vector>
#include "src/bullet/bounding_box.h"
#include "src/enemy/enemy_type.h"
#include "src/game/game_device.h"

namespace legend {
namespace enemy {

enum EnemyAIType : i32 {
  None,
  Enemy_Rotate,
  Boss_Rotate_Stand,
  Boss_Rush_Move,
  Boss_Rotate_Move,
  Boss_Escape,
};

struct EnemyAI {
  //! 移動タイプ
  enemy_type::MoveType move_type_;
  //! 衝突タイプ
  enemy_type::HitType hit_type_;
  //! 効果タイプ
  enemy_type::EffectType effect_type_;

  EnemyAIType ai_type_;
  std::vector<EnemyAIType> ai_actions_;

 private:
  i32 action_index_ = 0;

 public:
  void Init() {
    move_type_ = enemy_type::MoveType::Detour;
    hit_type_ = enemy_type::HitType::Rush;
    effect_type_ = enemy_type::EffectType::None;

    ai_type_ = EnemyAIType::None;
    ai_actions_ = {ai_type_};
    action_index_ = 0;
  }

  void SetAction(std::vector<EnemyAIType> ai_actions) {
    ai_actions_ = ai_actions;
    ai_type_ = (ai_actions_.size() <= 0) ? EnemyAIType::None : ai_actions_[0];
    action_index_ = 0;
  }

  void Action(math::Vector3 velocity, bullet::BoundingBox* box) {
    auto value = enemy_ai_[ai_type_];
    value(velocity, box);
    action_index_ =
        (ai_actions_.size() <= (action_index_ + 1)) ? 0 : (action_index_ + 1);
    ai_type_ = ai_actions_[action_index_];

    // switch (ai_type_) {
    //  case EnemyAIType::Boss_Rotate_Stand:
    //    ai_type_ = EnemyAIType::Boss_Rush_Move;
    //    break;
    //  case EnemyAIType::Boss_Rush_Move:
    //    ai_type_ = EnemyAIType::Boss_Rotate_Stand;
    //    break;
    //  case EnemyAIType::Boss_Rotate_Move:
    //    ai_type_ = EnemyAIType::Boss_Rotate_Stand;
    //    break;
    //}
  }

 private:
  std::unordered_map<i32,
                     std::function<void(math::Vector3, bullet::BoundingBox*)>>
      enemy_ai_ = {
          {EnemyAIType::None,
           [&](math::Vector3 velocity, bullet::BoundingBox* box) {
             // 加速度の設定
             box->ApplyCentralImpulse(velocity);
           }},
          {EnemyAIType::Enemy_Rotate,
           [&](math::Vector3 velocity, bullet::BoundingBox* box) {
             // 加速度の設定
             box->ApplyCentralImpulse(velocity);
             // 回転の設定
             auto angle = math::Vector3::kUpVector * velocity.Magnitude();
             angle *= (game::GameDevice::GetInstance()->GetRandom().Range(
                 -0.3f, 0.3f));
             box->SetAngularVelocity(angle);
           }},
          {EnemyAIType::Boss_Rotate_Stand,
           [&](math::Vector3 velocity, bullet::BoundingBox* box) {
             //// 回転の設定
             // auto angle = math::Vector3::kUpVector * velocity.Magnitude();
             // box->SetAngularVelocity(angle);
           }},
          {EnemyAIType::Boss_Rush_Move,
           [&](math::Vector3 velocity, bullet::BoundingBox* box) {
             // 加速度の設定
             box->ApplyCentralImpulse(velocity);
           }},
          {EnemyAIType::Boss_Rotate_Move,
           [&](math::Vector3 velocity, bullet::BoundingBox* box) {
             // 加速度の設定
             box->ApplyCentralImpulse(velocity);
             // 回転の設定
             auto angle = math::Vector3::kUpVector * velocity.Magnitude();
             angle *= (game::GameDevice::GetInstance()->GetRandom().Range(
                 -0.75f, 0.75f));
             box->SetAngularVelocity(angle);
           }},
          {EnemyAIType::Boss_Escape,
           [&](math::Vector3 velocity, bullet::BoundingBox* box) {
             // 加速度の設定
             box->ApplyCentralImpulse(velocity);
           }},
  };  //!< 行動判定
};
}  // namespace enemy
}  // namespace legend
#endif  //! LEGEND_ENEMY_ENEMY_AI_H_
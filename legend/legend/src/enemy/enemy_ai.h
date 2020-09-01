#ifndef LEGEND_ENEMY_ENEMY_AI_H_
#define LEGEND_ENEMY_ENEMY_AI_H_

#include "src/bullet/bounding_box.h"
#include "src/enemy/enemy_type.h"
#include "src/game/game_device.h"

namespace legend {
namespace enemy {

enum EnemyAIType : i32 {
  None,
  Enemy_Rotate,
  Boss_Rotate_Stand,
  Boss_Rotate_Move,
};

struct EnemyAI {
  //! �ړ��^�C�v
  enemy_type::MoveType move_type_;
  //! �Փ˃^�C�v
  enemy_type::HitType hit_type_;
  //! ���ʃ^�C�v
  enemy_type::EffectType effect_type_;

  EnemyAIType ai_type_;

 public:
  void Init() {
    move_type_ = enemy_type::MoveType::Detour;
    hit_type_ = enemy_type::HitType::Rush;
    effect_type_ = enemy_type::EffectType::None;

	ai_type_ = EnemyAIType::None;
  }

  void Action(math::Vector3 velocity, bullet::BoundingBox* box) {
    auto value = enemy_ai_[ai_type_];
    value(velocity, box);
    switch (ai_type_) {
      case EnemyAIType::Boss_Rotate_Stand:
        ai_type_ = EnemyAIType::Boss_Rotate_Move;
        break;
      case EnemyAIType::Boss_Rotate_Move:
        ai_type_ = EnemyAIType::Boss_Rotate_Stand;
        break;
    }
  }

 private:
  std::unordered_map<i32,
                     std::function<void(math::Vector3, bullet::BoundingBox*)>>
      enemy_ai_ = {
          {EnemyAIType::None,
           [&](math::Vector3 velocity, bullet::BoundingBox* box) {
             // �����x�̐ݒ�
             box->ApplyCentralImpulse(velocity);
           }},
          {EnemyAIType::Enemy_Rotate,
           [&](math::Vector3 velocity, bullet::BoundingBox* box) {
             // �����x�̐ݒ�
             box->ApplyCentralImpulse(velocity);
             // ��]�̐ݒ�
             auto angle = math::Vector3::kUpVector * velocity.Magnitude();
             angle *= (game::GameDevice::GetInstance()->GetRandom().Range(
                 -0.3f, 0.3f));
             box->SetAngularVelocity(angle);
           }},
          {EnemyAIType::Boss_Rotate_Stand,
           [&](math::Vector3 velocity, bullet::BoundingBox* box) {
             //// ��]�̐ݒ�
             //auto angle = math::Vector3::kUpVector * velocity.Magnitude();
             //box->SetAngularVelocity(angle);
           }},
          {EnemyAIType::Boss_Rotate_Move,
           [&](math::Vector3 velocity, bullet::BoundingBox* box) {
             // �����x�̐ݒ�
             box->ApplyCentralImpulse(velocity);
             // ��]�̐ݒ�
             auto angle = math::Vector3::kUpVector * velocity.Magnitude();
             angle *= (game::GameDevice::GetInstance()->GetRandom().Range(
                 -0.75f, 0.75f));
             box->SetAngularVelocity(angle);
           }},
          //{EnemyAIType::Forword,
          // [&](math::Vector3& velocity, bullet::BoundingBox* box) {
          //   auto angle = math::Vector3::kUpVector * velocity.Magnitude();
          //   angle *= (game::GameDevice::GetInstance()->GetRandom().Range(
          //       -0.3f, 0.3f));
          //   box->SetAngularVelocity(angle);
          // }},
  };  //!< �s������
};
}  // namespace enemy
}  // namespace legend
#endif  //! LEGEND_ENEMY_ENEMY_AI_H_
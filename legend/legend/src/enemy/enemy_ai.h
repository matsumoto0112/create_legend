#ifndef LEGEND_ENEMY_ENEMY_AI_H_
#define LEGEND_ENEMY_ENEMY_AI_H_

#include <vector>
#include "src/bullet/bounding_box.h"
#include "src/enemy/enemy_type.h"
#include "src/game/game_device.h"

namespace legend {
namespace enemy {

// �GAI�^�C�v
enum EnemyAIType : i32 {
  None,
  Enemy_Rotate,
  Boss_Rotate_Stand,
  Boss_Rush_Move,
  Boss_Rotate_Move,
  Boss_Tutorial,
};

// �GAI
struct EnemyAI {
  //! �ړ��^�C�v
  enemy_type::MoveType move_type_;
  //! �Փ˃^�C�v
  enemy_type::HitType hit_type_;
  //! ���ʃ^�C�v
  enemy_type::EffectType effect_type_;

  //! �GAI�^�C�v
  EnemyAIType ai_type_;
  //! �s�����X�g
  std::vector<EnemyAIType> ai_actions_;

 private:
  //! ���݂̍s���ԍ�
  i32 action_index_ = 0;

 public:
  /**
   * @brief ������
   */
  void Init() {
    move_type_ = enemy_type::MoveType::Move_Detour;
    hit_type_ = enemy_type::HitType::Hit_Rush;
    effect_type_ = enemy_type::EffectType::Effect_None;

    ai_type_ = EnemyAIType::None;
    ai_actions_ = {ai_type_};
    action_index_ = 0;
  }

  /**
   * @brief �s���ݒ�
   */
  void SetAction(std::vector<EnemyAIType> ai_actions) {
    ai_actions_ = ai_actions;
    ai_type_ = (ai_actions_.size() <= 0) ? EnemyAIType::None : ai_actions_[0];
    action_index_ = 0;
  }

  /**
   * @brief �s���A�N�V����
   */
  void Action(math::Vector3 velocity, bullet::BoundingBox* box) {
    auto value = enemy_ai_[ai_type_];
    value(velocity, box);
    action_index_ =
        (ai_actions_.size() <= (action_index_ + 1)) ? 0 : (action_index_ + 1);
    ai_type_ = ai_actions_[action_index_];
  }

 private:
  //! �s�����X�g
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
             // auto angle = math::Vector3::kUpVector * velocity.Magnitude();
             // box->SetAngularVelocity(angle);
           }},
          {EnemyAIType::Boss_Rush_Move,
           [&](math::Vector3 velocity, bullet::BoundingBox* box) {
             // �����x�̐ݒ�
             box->ApplyCentralImpulse(velocity);
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
          {EnemyAIType::Boss_Tutorial,
           [&](math::Vector3 velocity, bullet::BoundingBox* box) {
             // �����x�̐ݒ�
             box->ApplyCentralImpulse(velocity);
           }},
  };  //!< �s������
};
}  // namespace enemy
}  // namespace legend
#endif  //! LEGEND_ENEMY_ENEMY_AI_H_
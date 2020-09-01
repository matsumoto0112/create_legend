#ifndef LEGEND_ACTOR_ACTOR_MEDIATOR_H
#define LEGEND_ACTOR_ACTOR_MEDIATOR_H

/**
 * @file actor_mediator.h
 * @brief
 */

#include "src/bullet/collider.h"
#include "src/system/mode.h"

namespace legend {
namespace player {
class Player;
}  // namespace player
namespace enemy {
class Enemy;
}  // namespace enemy
namespace object {
class Fragment;
}
}  // namespace legend

namespace legend {
namespace actor {
class IActorMediator {
 public:
  virtual ~IActorMediator() = default;
  /**
   * @brief �v���C���[�̈ړ��J�n���C�x���g
   */
  virtual void PlayerMoveStartEvent() = 0;
  /**
   * @brief �v���C���[�̈ړ��I�����C�x���g
   */
  virtual void PlayerMoveEndEvent() = 0;
  /**
   * @brief �v���C���[�̃X�L�������J�n���C�x���g
   */
  virtual void PlayerSkillActivate() = 0;
  /**
   * @brief �v���C���[�̃X�L�������I�����C�x���g
   */
  virtual void PlayerSkillDeactivate() = 0;
  /**
   * @brief �v���C���[�̃X�L���������o�I�����C�x���g
   */
  virtual void PlayerCompleteEquipment() = 0;
  /**
   * @brief �v���C���[���擾����
   */
  virtual player::Player* GetPlayer() const = 0;
  /**
   * @brief �G���V�[������擾����
   */
  virtual std::vector<enemy::Enemy*> GetEnemies() = 0;
  /**
   * @brief �R���C�_�[��ǉ�����
   */
  virtual void AddCollider(std::shared_ptr<bullet::Collider> collider) = 0;
  /**
   * @brief �R���C�_�[���폜����
   */
  virtual void RemoveCollider(std::shared_ptr<bullet::Collider> collider) = 0;
  /**
   * @brief �A�N�^�[���폜����
   * @details
   * �폜�͂��̃^�C�~���O�ł͂Ȃ��A�A�N�^�[�̍X�V�I�����ɍ폜����悤�ɂ���
   */
  virtual void RemoveActor(Actor* actor) = 0;
  /**
   * @brief ���C�L���X�g
   */
  virtual btCollisionWorld::AllHitsRayResultCallback RayCast(
      const math::Vector3& start, const math::Vector3& end) const = 0;
  /**
   * @brief ���C���J������Y����]�ʂ��擾����
   */
  virtual float GetMainCameraThetaAngle() const = 0;
  /**
   * @brief ���݂̃^�[���󋵂��擾����
   */
  virtual system::Mode GetCurrentTurn() const = 0;
  /**
   * @brief 1�O�̃^�[���󋵂��擾����
   */
  virtual system::Mode GetBeforeTurn() const = 0;
  /**
   * @brief �X�g�b�v���Ԃ�ǉ�����
   */
  virtual void AddStopTime(float time) = 0;
  /**
   * @brief �X�g�b�v���Ԃ�ݒ�
   */
  virtual void SetStopTime(float time) = 0;
  /**
   * @brief �������������J�X��o�^����
   */
  virtual void AddFragment(std::unique_ptr<object::Fragment> fragment) = 0;
};

}  // namespace actor
}  // namespace legend
#endif  //! LEGEND_ACTOR_ACTOR_MEDIATOR_H

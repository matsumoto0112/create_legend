#ifndef LEGEND_OBJECT_FRAGMENT_H
#define LEGEND_OBJECT_FRAGMENT_H

#include "src/actor/actor.h"
#include "src/bullet/bounding_box.h"
#include "src/util/timer.h"

/**
 * @file fragment.h
 * @brief �����J�X�N���X
 */

namespace legend {
namespace object {
/**
 * @brief �����J�X
 */
class Fragment : public actor::Actor {
  using Parent = actor::Actor;

 public:
  struct InitializeParameter {
    math::Vector3 position;
    math::Quaternion rotation;
    math::Vector3 scale;
    math::Vector3 bounding_box_length;
  };

 public:
  /**
   * @brief �R���X�g���N�^
   */
  Fragment();
  /**
   * @brief �f�X�g���N�^
   */
  virtual ~Fragment();
  /**
   * @brief ������
   */
  virtual bool Init(actor::IActorMediator* mediator,
                    const InitializeParameter& params);
  /**
   * @brief �X�V
   */
  virtual bool Update() override;
  /**
   * @brief �`��
   */
  void Draw() override;
  /**
   * @brief ���S����؂�ւ�
   */
  void ChangeDead();
  /**
   * @brief ���S����̎擾
   */
  bool GetIsDead() const;

 private:
  //! �R���C�_�[
  std::shared_ptr<bullet::BoundingBox> box_;
  //! ���S����
  bool is_dead_;
  //! �폜����
  util::CountDownTimer dead_time_;
};

}  // namespace object
}  // namespace legend
#endif  //! LEGEND_OBJECT_FRAGMENT_H

#ifndef LEGEND_OBJECT_OBSTACLE_H_
#define LEGEND_OBJECT_OBSTACLE_H_

#include "src/actor/actor.h"
#include "src/bullet/bounding_box.h"

/**
 * @file obstacle.h
 * @brief ��Q���N���X
 */

namespace legend {
namespace object {

/**
 * @brief ��Q��
 */
class Obstacle : public actor::Actor {
 public:
  struct InitializeParameter {
    u32 model_id;
    math::Vector3 position;
    math::Quaternion rotation;
    math::Vector3 bounding_box_length;
  };

  using Parent = actor::Actor;

 public:
  /**
   * @brief �R���X�g���N�^
   */
  Obstacle();
  /**
   * @brief�f�X�g���N�^
   */
  virtual ~Obstacle();
  /**
   * @brief ������
   */
  bool Init(actor::IActorMediator* mediator, const InitializeParameter& params);
  /**
   * @brief �X�V
   */
  virtual bool Update() override;

 private:
  //! �R���C�_�[
  std::shared_ptr<bullet::BoundingBox> box_;
};

}  // namespace object
}  // namespace legend
#endif  //! LEGEND_OBJECT_OBSTACLE_H_

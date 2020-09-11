#ifndef LEGEND_OBJECT_DESK_H_
#define LEGEND_OBJECT_DESK_H_

#include "src/actor/actor.h"
#include "src/bullet/bounding_box.h"

/**
 * @file desk.h
 * @brief ���N���X
 */

namespace legend {
namespace object {

/**
 * @brief ��
 */
class Desk : public actor::Actor {
  using Parent = actor::Actor;

 public:
  /**
   * @brief ���̏������p�����[�^
   */
  struct InitializeParameter {
    util::Transform transform;
    math::Vector3 bounding_box_length;
    math::Vector3 normal;
    float mass = 0.0f;
    float restitution = 0.5f;
    float friction = 1.5f;
  };

 public:
  /**
   * @brief �R���X�g���N�^
   */
  Desk();
  /**
   * @brief �f�X�g���N�^
   */
  virtual ~Desk();

  virtual bool Init(actor::IActorMediator* mediator,
                    const InitializeParameter& parameter);
  /**
   * @brief �X�V
   */
  bool Update() override;
  virtual void Destroy() override;
  /**
   * @brief ���W�̐ݒ�
   */
  void SetPosition(math::Vector3 position);
  /**
   * @brief ��]�̐ݒ�
   */
  void SetRotation(math::Quaternion rotation);
  /**
   * @brief �X�P�[���̐ݒ�
   */
  void SetScale(math::Vector3 scale);
  /**
   * @brief �@���x�N�g���̐ݒ�
   */
  void SetNormal(math::Vector3 normal);
  /**
   * @brief ���W�̎擾
   */
  math::Vector3 GetPosition();
  /**
   * @brief ��]�̎擾
   */
  math::Quaternion GetRotation();
  /**
   * @brief �X�P�[���̐ݒ�
   */
  math::Vector3 GetScale();
  /**
   * @brief �@���x�N�g���̎擾
   */
  math::Vector3 GetNormal();

 private:
  math::Vector3 normal_;
  std::shared_ptr<bullet::BoundingBox> box_;
};

}  // namespace object
}  // namespace legend

#endif  //! LEGEND_OBJECT_DESK_H_

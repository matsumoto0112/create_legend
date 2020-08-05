#ifndef LEGEND_OBJECT_DESK_H_
#define LEGEND_OBJECT_DESK_H_

#include "src/actor/actor.h"
#include "src/directx/buffer/constant_buffer.h"
#include "src/draw/model.h"
#include "src/physics/bounding_box.h"
#include "src/util/transform.h"

namespace legend {
namespace object {

class Desk : public actor::Actor<physics::BoundingBox> {
 public:
  /**
   * @brief ���̏������p�����[�^
   */
  struct InitializeParameter {
    util::Transform transform;
    math::Vector3 bounding_box_length;
    math::Vector3 normal;
  };

 public:
  /**
   * @brief �R���X�g���N�^
   */
  Desk();
  /**
   * @brief �f�X�g���N�^
   */
  ~Desk();

  virtual bool Init(const InitializeParameter& parameter);
  /**
   * @brief �X�V
   */
  bool Update() override;
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
};

}  // namespace object
}  // namespace legend

#endif  //! LEGEND_OBJECT_DESK_H_

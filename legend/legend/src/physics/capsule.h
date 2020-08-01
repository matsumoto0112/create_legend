#ifndef LEGEND_PHYSICS_CAPSULE_H_
#define LEGEND_PHYSICS_CAPSULE_H_

#include "src/physics/bounding_box.h"

namespace legend {
namespace physics {

/**
 * @class Capsule
 * @brief �J�v�Z���̃N���X
 */
class Capsule {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  Capsule();
  /**
   * @brief �R���X�g���N�^
   */
  Capsule(math::Vector3 end_position, float radius, float scale);
  /**
   * @brief �R���X�g���N�^
   */
  Capsule(math::Vector3 start_position, math::Vector3 end_position,
          float radius, float scale);
  /**
   * @brief �f�X�g���N�^
   */
  ~Capsule();
  /**
   * @brief ������
   */
  bool Initialize();
  /**
   * @brief �X�V
   */
  void Update();
  /**
   * @brief �`��
   */
  void Draw();
  /**
   * @brief �n�_�̐ݒ�
   */
  void SetStartPosition(math::Vector3 position);
  /**
   * @brief �I�_�̐ݒ�
   */
  void SetEndPosition(math::Vector3 position);
  /**
   * @brief ��]�ʂ̐ݒ�
   */
  void SetRotation(math::Quaternion rotate);
  /**
   * @brief �X�P�[���̐ݒ�
   */
  void SetScale(float scale);
  /**
   * @brief ���a�̐ݒ�
   */
  void SetRadius(float radius);
  /**
   * @brief �n�_�̎擾
   */
  math::Vector3 GetStartPosition();
  /**
   * @brief �I�_�̎擾
   */
  math::Vector3 GetEndPosition();
  /**
   * @brief ��]�ʂ̎擾
   */
  math::Quaternion GetRotation();
  /**
   * @brief�X�P�[���̎擾
   */
  float GetScale();
  /**
   * @brief ���a�̎擾
   */
  float GetRadius();
  /**
   * @brief �����̎擾
   */
  float GetLength();

 private:
  //! �n�_
  math::Vector3 start_position_;
  //! �I�_
  math::Vector3 end_position_;
  //! ��]��
  math::Quaternion rotation_;
  //! �X�P�[��
  float scale_;
  //! ���a
  float radius_;
};

}  // namespace physics
}  // namespace legend

#endif  //! LEGEND_PHYSICS_CAPSULE_H_

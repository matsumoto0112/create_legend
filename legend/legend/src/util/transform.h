#ifndef LEGEND_UTIL_TRANSFORM_H_
#define LEGEND_UTIL_TRANSFORM_H_

/**
 * @file transform.h
 * @brief �g�����X�t�H�[���Ǘ��N���X��`
 */

#include "src/math/matrix_4x4.h"
#include "src/math/quaternion.h"
#include "src/math/vector_3.h"

namespace legend {
namespace util {

/**
 * @class Transform
 * @brief �g�����X�t�H�[���Ǘ��N���X
 */
class Transform {
 public:
  /**
   * @brief �R���X�g���N�^
   * @param position ���W
   * @param rotation ��]
   * @param scale �X�P�[�����O
   */
  Transform(const math::Vector3& position = math::Vector3::kZeroVector,
            const math::Quaternion& rotation = math::Quaternion::kIdentity,
            const math::Vector3& scale = math::Vector3::kUnitVector);
  /**
   * @brief �f�X�g���N�^
   */
  ~Transform();

  /**
   * @brief ���W��ݒ肷��
   */
  inline void SetPosition(const math::Vector3& position) {
    this->position_ = position;
  }
  /**
   * @brief ���W���擾����
   */
  inline math::Vector3 GetPosition() const { return position_; }
  /**
   * @brief ��]��ݒ肷��
   */
  inline void SetRotation(const math::Quaternion& rotation) {
    this->rotation_ = rotation;
  }
  /**
   * @brief ��]���擾����
   */
  inline math::Quaternion GetRotation() const { return rotation_; }
  /**
   * @brief �X�P�[�����O��ݒ肷��
   */
  inline void SetScale(const math::Vector3& scale) { this->scale_ = scale; }
  /**
   * @brief �X�P�[�����O���擾����
   */
  inline math::Vector3 GetScale() const { return scale_; }

  /**
   * @brief ���[���h�s����쐬����
  */
  math::Matrix4x4 CreateWorldMatrix() const;

 private:
  //! ���W
  math::Vector3 position_;
  //! ��]
  math::Quaternion rotation_;
  //! �X�P�[�����O
  math::Vector3 scale_;
};

}  // namespace util
}  // namespace legend
#endif  //! LEGEND_UTIL_TRANSFORM_H_

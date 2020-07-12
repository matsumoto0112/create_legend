#ifndef LEGEND_PHYSICS_BOUNDING_BOX_H_
#define LEGEND_PHYSICS_BOUNDING_BOX_H_

#include "src/math/vector_3.h"

namespace legend {
namespace physics {

/**
 * @class BoundingBox
 * @brief �����̂̃N���X
 */

class BoundingBox {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  BoundingBox();
  /*
   * @brief �����x�N�g�����擾
   * @param ���ԍ�
   */
  math::Vector3 GetDirection(i32 direction_num);
  /**
   * @brief �������擾
   * @param ���ԍ�
   */
  float GetLength(i32 length_num);
  /**
   * @brief ���݂̈ʒu���擾
   */
  math::Vector3 GetPosition();
  /**
   * @brief �e�����x�N�g���̐ݒ�
   * @param X����
   * @param Y����
   * @param Z����
   */
  void SetDirection(math::Vector3 direction_x, math::Vector3 direction_y,
                    math::Vector3 direction_z);
  /**
   * @brief �e�����̒����̐ݒ�
   * @param X�����̒���
   * @param Y�����̒���
   * @param Z�����̒���
   */
  void SetLength(float length_x, float length_y, float length_z);

 private:
  //!���S���W
  math::Vector3 position_;
  //!�����x�N�g��
  std::vector<math::Vector3> directions_;
  //!�e�������̒���
  std::vector<float> lengthes_;
};

}  // namespace physics
}  // namespace legend

#endif  //! LEGEND_PHYSICS_BOUNDING_BOX_H_

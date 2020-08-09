#ifndef LEGEND_PRIMITIVE_BOX_H_
#define LEGEND_PRIMITIVE_BOX_H_

/**
 * @file box.h
 * @brief Box�^�̃v���~�e�B�u�`��N���X��`
 */

#include "src/primitive/primitive.h"

namespace legend {
namespace primitive {

/**
 * @brief Box�^�̃v���~�e�B�u�`��N���X
 */
class Box : public PrimitiveBase {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  Box();
  /**
   * @brief �f�X�g���N�^
   */
  virtual ~Box();
  /**
   * @brief ������
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init() override;
};

}  // namespace primitive
}  // namespace legend

#endif  //! LEGEND_PRIMITIVE_BOX_H_

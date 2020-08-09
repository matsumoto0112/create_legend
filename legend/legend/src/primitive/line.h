#ifndef LEGEND_PRIMITIVE_LINE_H_
#define LEGEND_PRIMITIVE_LINE_H_

/**
 * @file line.h
 * @brief Line�^�̃v���~�e�B�u�`��N���X��`
 */

#include "src/primitive/primitive.h"

namespace legend {
namespace primitive {

/**
 * @brief Line�^�̃v���~�e�B�u�`��N���X
 */
class Line : public PrimitiveBase {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  Line();
  /**
   * @brief �f�X�g���N�^
   */
  virtual ~Line();
  /**
   * @brief ������
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init() override;

 private:
};

}  // namespace primitive
}  // namespace legend
#endif  //! LEGEND_PRIMITIVE_LINE_H_

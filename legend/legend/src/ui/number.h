#ifndef LEGEND_UI_NUMBER_H
#define LEGEND_UI_NUMBER_H

/**
 * @file number.h
 * @brief 1���̐��l��`�悷��UI�摜�N���X��`
 */

#include "src/ui/image.h"

namespace legend {
namespace ui {

/**
 * @brief 1���̐��l��`�悷��UI�摜�N���X
 */
class Number : public Image {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  Number();
  /**
   * @brief �f�X�g���N�^
   */
  virtual ~Number();
  /**
   * @brief ������
   * @param name �摜��
   * @param heap_id �g�p����q�[�vID
   */
  virtual bool Init(
      const std::wstring& name,
      directx::descriptor_heap::heap_parameter::LocalHeapID heap_id =
          directx::descriptor_heap::heap_parameter::LocalHeapID::ONE_PLAY)
      override;
  /**
   * @brief ���l�̐ݒ�
   * @param 0~9�܂ł̐��l
   */
  void SetNumber(int num);
};

}  // namespace ui
}  // namespace legend
#endif  //! LEGEND_UI_NUMBER_H

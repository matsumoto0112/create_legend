#ifndef LEGEND_UI_UI_BOARD_H
#define LEGEND_UI_UI_BOARD_H

/**
 * @file ui_board.h
 * @brief UI�\���p�{�[�h��`
 */

#include "src/ui/ui_component.h"

namespace legend {
namespace ui {

/**
 * @brief UI�\���p�{�[�h
 * @details
 * ���̃I�u�W�F�N�g��UI�R���|�[�l���g��ǉ����Ă����A��ł܂Ƃ߂ĕ`�悷��
 */
class UIBoard {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  UIBoard();
  /**
   * @brief �f�X�g���N�^
   */
  virtual ~UIBoard();
  /**
   * @brief �R���|�[�l���g�����Z�b�g����
   */
  void Reset();
  /**
   * @brief �R���|�[�l���g��ǉ����A���̃|�C���^��Ԃ�
   */
  UIComponent* AddComponent(std::unique_ptr<UIComponent> component);
  /**
   * @�R���|�[�l���g�̕`��
   */
  void Draw();

 private:
  //! �R���|�[�l���g�z��
  std::vector<std::unique_ptr<UIComponent>> ui_components_;
};

}  // namespace ui
}  // namespace legend
#endif  //! LEGEND_UI_UI_BOARD_H

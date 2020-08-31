#ifndef LEGEND_UI_PLAYER_UI_H_
#define LEGEND_UI_PLAYER_UI_H_

#include "src/draw/sprite_renderer.h"
#include "src/draw/texture_string.h"
#include "src/skill/skill.h"

namespace legend {
namespace ui {
class PlayerUI {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  PlayerUI();
  /**
   * @brief �f�X�g���N�^
   */
  virtual ~PlayerUI();
  /**
   * @brief ������
   */
  void Init();
  /**
   * @brief �X�V
   */
  void Update();
  /**
   * @brief �`��
   */
  void Draw();
  /**
   * @brief �摜�ǉ�
   */
  void AddEquipmentUI(const skill::Skill* skill);
  /**
   * @brief �摜�폜
   */
  void RemoveEquipmentUI(i32 remove_num);

 private:
  //! �v���C���[�摜
  draw::Sprite2D player_sprite_;
  //! �����X�L���摜
  std::vector<draw::Sprite2D> equipment_sprites_;
};
}  // namespace ui
}  // namespace legend

#endif  //! LEGEND_UI_PLAYER_UI_H_

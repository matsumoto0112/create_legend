#ifndef LEGEND_TURN_CHANGE_TURN_CHANGE_H_
#define LEGEND_TURN_CHANGE_TURN_CHANGE_H_
#include "src/draw/sprite_renderer.h"
#include "src/math/vector_2.h"
#include "src/math/math_util.h"
#include "src/system/mode.h"

namespace legend {
namespace system {
class TurnSystem;
}  // namespace system
namespace turn_change {

class TurnChange {
 public:
     /**
      * @brief �R���X�g���N�^
      */
  TurnChange();
  /**
   * @brief �f�X�g���N�^
   */
  ~TurnChange();
  /**
   * @brief ������
   */
  bool Init(system::TurnSystem* turn_system);
  /**
   * @brief �^�[���؂�ւ����o�̊J�n
   */
  bool ChangeStart(system::Mode next_mode);
  /**
   * @brief �^�[���؂�ւ����o�̍X�V
   */
  bool TurnChangeUpdate();
  /**
   * @brief �{�X�o���Q�[�W�̍X�V
   */
  bool BossGenerateUIUpdate();
  /**
   * @brief �X�V����
   */
  bool Update();
  /**
   * @brief �`�揈��
   */
  void Draw();

private:
    math::Vector2 LerpVector2(math::Vector2 v1, math::Vector2 v2, float t);

 private:
  //! �\�������Ԃ��ǂ���
  bool is_view_;
  //! �^�C�}�[
  float timer_;
  //! �^�[���؂�ւ����o�̊J�n�܂ł̕b��
  const float start_time_ = 0.15f;
  //! �^�[���؂�ւ����o���̕b��
  const float staging_time_ = 0.75f;
  //! �^�[���؂�ւ����o�̌�̕b��
  const float before_time_ = 0.4f;
  //! �E�B���h�E�T�C�Y
  math::Vector2 screen_size_;
  //! �^�[���V�X�e���̃|�C���^
  system::TurnSystem* turn_system_;
  //! �ύX�O�̃^�[���摜
  draw::Sprite2D before_turn_sprite_;
  //! �ύX��̃^�[���摜
  draw::Sprite2D next_turn_sprite_;
  //! �ύX��̃��[�h
  system::Mode next_mode_;
  
  //! �{�X�̏o������^�[��
  i32 boss_generate_turn_;
  //! �{�X�o���܂ł�UI�̈ʒu
  math::Vector2 boss_generate_ui_center_position_;
  //! �ړ��O�̃v���C���[�A�C�R���̈ʒu
  math::Vector2 before_player_icon_position_;
  //! �v���C���[�̃A�C�R���摜
  draw::Sprite2D player_icon_;
  //! �{�X�̃A�C�R���摜
  draw::Sprite2D boss_icon_;
  //! �{�X�o���^�[���܂ł̔w�i�摜
  draw::Sprite2D boss_generate_ui_bg_;
};

}  // namespace turn_change
}  // namespace legend
#endif  //! LEGEND_TURN_CHANGE_TURN_CHANGE_H_

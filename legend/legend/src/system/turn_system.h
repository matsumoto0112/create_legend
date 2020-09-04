#ifndef LEGEND_SYSTEM_TURN_SYSTEM_H_
#define LEGEND_SYSTEM_TURN_SYSTEM_H_

/**
 * @file turn_system.h
 * @brief �^�[���ɂ��Q�[���i�s�Ǘ��N���X��`
 */

#include <set>

#include "src/actor/actor_manager.h"
#include "src/camera/follow_camera.h"
#include "src/camera/perspective_camera.h"
#include "src/directx/buffer/index_buffer.h"
#include "src/directx/buffer/vertex_buffer.h"
#include "src/scenes/fade_in_out.h"
#include "src/system/game_data.h"
#include "src/system/mode.h"
#include "src/turn_change/turn_change.h"
#include "src/ui/gauge.h"
#include "src/ui/image.h"
#include "src/ui/number.h"
#include "src/ui/ui_board.h"
#include "src/util/timer.h"

namespace legend {
namespace system {
namespace camera_mode {
enum Enum { Main, Sub1, Sub2, COUNT };
}  // namespace camera_mode
namespace ui_format {
/**
 * @brief UI�̃t�@�C���Ɋi�[���Ă���p�����[�^
 */
enum Enum { NAME, X, Y, Z, ID, MAX };
}  // namespace ui_format
/**
 * @brief UI�t�@�C���Ɋi�[���Ă���Q�[�W�̏�
 */
namespace gauge_id {
enum Enum {
  PLAYER_CHARGE_POWER,
  PLAYER_STRENGTHENED_STATE_0,
  PLAYER_STRENGTHENED_STATE_1,
  PLAYER_STRENGTHENED_STATE_2,
  MAX
};
}  // namespace gauge_id
/**
 * @brief UI�t�@�C���Ɋi�[���Ă��鐔�lUI�̏�
 */
namespace number_id {
enum Enum { DIGIT_3, DIGIT_2, DIGIT_1, MAX };
}  // namespace number_id

/**
 * @class TurnSystem
 * @brief �^�[�����Ǘ��N���X
 */
class TurnSystem {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  TurnSystem();
  /**
   * @brief �f�X�g���N�^
   */
  virtual ~TurnSystem();
  /**
   * @brief ������
   * @param stage_name �X�e�[�W��
   */
  bool Init(const std::string& stage_name);
  /**
   * @brief �X�V����
   */
  bool Update();
  /**
   * @brief �`�揈��
   */
  void Draw();
  /**
   * @brief �f�o�b�O�`��
   */
  void DebugDraw();
  /**
   * @brief �Q�[�����I���������ǂ���
   */
  bool IsGameEnd() const;
  /**
   * @brief �Q�[�����ʂ��擾����
   */
  system::GameDataStorage::ResultData GetResult() const;
  /**
   * @brief �^�[���̃��[�h��ݒ肷��
   */
  void SetTurnMode(Mode mode);
  Mode GetCurrentMode();
  /**
   * @brief 1�O�̃^�[����Ԃ��擾
   */
  Mode GetBeforeMode();
  /**
   * @brief �v���C���[�^�[���ɂȂ鉉�o
   */
  bool ToPlayerTurn();
  /**
   * @brief �G�̃^�[���ɂȂ鉉�o
   */
  bool ToEnemyTurn();

 private:
  /**
   * @brief �^�[�����̑���
   */
  void AddCurrentTurn();
  /**
   * @brief ���݂̃^�[�������擾
   */
  i32 GetCurrentTurn();
  /**
   * @brief �\���p�̃^�[������\��
   */
  i32 GetViewTurn();
  /**
   * @brief �v���C���[�̈ړ��O��������
   */
  bool PlayerMoveReady();
  /**
   * @brief �v���C���[�̈ړ�����
   */
  bool PlayerMoving();

  bool WaitEnemyMoveStart();

  /**
   * @brief �v���C���[�̈ړ��I����̃X�L����������
   */
  bool PlayerSkillAfterMoved();
  /**
   * @brief �X�L���ǉ����o����
   */
  bool AddSkill();

  /**
   * @brief �G�̈ړ�����
   */
  bool EnemyMove();
  /**
   * @brief �G�̈ړ��I��������
   */
  bool EnemyMoveEnd();
  /**
   * @brief �G�̈ړ��I��������
   */
  bool EnemyMoveProducing();
  /**
   * @brief �{�X�̈ړ��I��������
   */
  bool BossMoveProducing();
  /**
   * @brief �G�̐�������
   */
  bool EnemyGenerate();

 private:
  //! ���݃^�[����
  i32 current_turn_;
  i32 view_turn_;
  //! ���݂̃v���C���
  Mode current_mode_;
  //! 1�O�̃v���C���
  Mode before_mode_;

  //! �A�N�^�[�Ǘ��N���X
  actor::ActorManager actor_manager_;
  //! �^�[���ύX���o�p�N���X
  turn_change::TurnChange turn_change_;

  //! UI�\���{�[�h
  ui::UIBoard ui_board_;
  //! UI�Q�[�W�z��
  std::vector<ui::Gauge*> gauges_;
  //! ���l�摜�z��
  std::vector<ui::Number*> numbers_;
  std::vector<ui::UIComponent*> no_render_if_enemy_turn_uis_;

  scenes::FadeInOut fade_;
  bool is_scene_all_end_;
  bool is_scene_end_fade_start_;

 private:
  // UI�̃��A���^�C���C���p�p�����[�^
  std::vector<legend::ui::UIComponent*> components_;
  std::vector<std::vector<std::string>> input_lines_;

  directx::buffer::VertexBuffer vertex_buffer_;
  directx::buffer::IndexBuffer index_buffer_;
};

}  // namespace system
}  // namespace legend

#endif  //! LEGEND_SYSTEM_TURN_SYSTEM_H_

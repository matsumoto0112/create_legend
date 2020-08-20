#ifndef LEGEND_SYSTEM_TURN_SYSTEM_H_
#define LEGEND_SYSTEM_TURN_SYSTEM_H_

/**
 * @file turn_system.h
 * @brief �^�[���ɂ��Q�[���i�s�Ǘ��N���X��`
 */

#include "src/actor/actor_mediator.h"
#include "src/bullet/physics_field.h"
#include "src/camera/follow_camera.h"
#include "src/camera/perspective_camera.h"
#include "src/enemy/enemy_manager.h"
#include "src/object/desk.h"
#include "src/object/fragment.h"
#include "src/object/graffiti.h"
#include "src/player/player.h"
#include "src/search/search_manager.h"
#include "src/stage_generate/stage_generator.h"
#include "src/system/mode.h"
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
class TurnSystem : public actor::IActorMediator {
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
   * @brief �v���C���[�̈ړ��J�n���C�x���g
   */
  virtual void PlayerMoveStartEvent() override;
  /**
   * @brief �v���C���[�̈ړ��I�����C�x���g
   */
  virtual void PlayerMoveEndEvent() override;
  /**
   * @brief �v���C���[�̃X�L�������J�n���C�x���g
   */
  virtual void PlayerSkillActivate() override;
  /**
   * @brief �v���C���[�̃X�L�������I�����C�x���g
   */
  virtual void PlayerSkillDeactivate() override;
  /**
   * @brief �v���C���[���擾����
   */
  virtual player::Player* GetPlayer() override;
  /**
   * @brief �G���V�[������擾����
   */
  virtual std::vector<enemy::Enemy*> GetEnemies() override;
  virtual void AddCollider(std::shared_ptr<bullet::Collider> collider) override;

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
   * @brief �v���C���[�̈ړ��O��������
   */
  bool PlayerMoveReady();
  /**
   * @brief �v���C���[�̈ړ�����
   */
  bool PlayerMoving();
  /**
   * @brief �v���C���[�̈ړ��I����̃X�L����������
   */
  bool PlayerSkillAfterModed();
  /**
   * @brief �G�̈ړ�����
   */
  bool EnemyMove();
  /**
   * @brief �G�̈ړ��I��������
   */
  bool EnemyMoveEnd();

  /**
   * @brief �J�����̏�����
   */
  bool InitCameras();
  /**
   * @brief �������̍폜
   */
  void RemoveGraffiti();
  /**
   * @brief �������X�V����
   */
  void UpdateGraffiti();
  /**
   * @brief �����J�X�̍폜
   */
  void RemoveFragment();

 private:
  //! ���݃^�[����
  i32 current_turn_;
  //! ���݂̃v���C���
  Mode current_mode_;
  ////! �����t�B�[���h
  // PhysicsField physics_field_;
  //! �X�e�[�W����
  stage_generate::StageGenerator stage_generator_;
  ////! ���C���J����
  // camera::FollowCamera main_camera_;
  //! �v���C���[
  player::Player player_;
  //! ��
  std::vector<object::Desk> desks_;
  //! ��Q��
  std::vector<object::Obstacle> obstacles_;
  //! ������
  std::vector<object::Graffiti> graffities_;
  //! �����J�X
  std::vector<object::Fragment> fragments_;
  //! �G�Ǘ�
  enemy::EnemyManager enemy_manager_;
  //! ����Ǘ�
  search::SearchManager search_manager_;
  //! �^�C�}�[
  util::CountDownTimer countdown_timer_;

  //! �g�p����J����
  std::array<std::unique_ptr<camera::Camera>, camera_mode::COUNT> cameras_;
  //! ���ݎg�p���Ă���J����
  camera_mode::Enum current_camera_;

  //! UI�\���{�[�h
  ui::UIBoard ui_board_;
  //! UI�Q�[�W�z��
  std::vector<ui::Gauge*> gauges_;
  //! ���l�摜�z��
  std::vector<ui::Number*> numbers_;

  bullet::PhysicsField physics_field_;

 private:
  // UI�̃��A���^�C���C���p�p�����[�^
  std::vector<legend::ui::UIComponent*> components_;
  std::vector<std::vector<std::string>> input_lines_;
};

}  // namespace system
}  // namespace legend

#endif  //! LEGEND_SYSTEM_TURN_SYSTEM_H_

#ifndef LEGEND_SYSTEM_TURN_SYSTEM_H_
#define LEGEND_SYSTEM_TURN_SYSTEM_H_

/**
 * @file turn_system.h
 * @brief �^�[���ɂ��Q�[���i�s�Ǘ��N���X��`
 */

#include "src/actor/actor_mediator.h"
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
#include "src/ui/image.h"
#include "src/ui/ui_board.h"
#include "src/util/timer.h"

namespace legend {
namespace system {
namespace camera_mode {
enum Enum { Main, Sub1, Sub2, COUNT };
}  // namespace camera_mode

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

 private:
  //! ���݃^�[����
  i32 current_turn_;
  //! ���݂̃v���C���
  Mode current_mode_;
  //! �����t�B�[���h
  PhysicsField physics_field_;
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
};

}  // namespace system
}  // namespace legend

#endif  //! LEGEND_SYSTEM_TURN_SYSTEM_H_

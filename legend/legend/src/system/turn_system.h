#ifndef LEGEND_SYSTEM_TURN_SYSTEM_H_
#define LEGEND_SYSTEM_TURN_SYSTEM_H_

/**
 * @file turn_system.h
 * @brief �^�[���ɂ��Q�[���i�s�Ǘ��N���X��`
 */

#include <set>

#include "src/actor/actor_mediator.h"
#include "src/actor/actor_render_command_list.h"
#include "src/bullet/physics_field.h"
#include "src/camera/follow_camera.h"
#include "src/camera/lookat_target_camera.h"
#include "src/camera/perspective_camera.h"
#include "src/directx/buffer/index_buffer.h"
#include "src/directx/buffer/vertex_buffer.h"
#include "src/enemy/enemy_manager.h"
#include "src/object/desk.h"
#include "src/object/fragment.h"
#include "src/object/graffiti.h"
#include "src/player/player.h"
#include "src/scenes/fade_in_out.h"
#include "src/search/search_manager.h"
#include "src/skill/skill_item_box.h"
#include "src/stage_generate/stage_generator.h"
#include "src/system/game_data.h"
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
   * @brief �Q�[�����I���������ǂ���
   */
  bool IsGameEnd() const;
  /**
   * @brief �Q�[�����ʂ��擾����
   */
  system::GameDataStorage::GameData GetResult() const;
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
  virtual void RemoveCollider(
      std::shared_ptr<bullet::Collider> collider) override;
  virtual btCollisionWorld::AllHitsRayResultCallback RayCast(
      const math::Vector3& start, const math::Vector3& end) const override;
  virtual void RemoveActor(actor::Actor* actor) override;
  virtual float GetMainCameraThetaAngle() const override;
  /**
   * @brief ���݂̃^�[���󋵂��擾����
   */
  Mode GetCurrentTurn() const override { return current_mode_; }

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
   * @brief �����J�X�̒ǉ�
   */
  void AddFragment(std::unique_ptr<object::Fragment> fragment);
  /**
   * @brief �J�����̍X�V
   */
  void UpdateCamera();

 private:
  bullet::PhysicsField physics_field_;
  //! ���݃^�[����
  i32 current_turn_;
  //! ���݂̃v���C���
  Mode current_mode_;
  //! �X�e�[�W����
  stage_generate::StageGenerator stage_generator_;
  ////! ���C���J����
  // camera::FollowCamera main_camera_;
  //! �v���C���[
  std::unique_ptr<player::Player> player_;

  std::vector<std::unique_ptr<actor::Actor>> static_objects_;
  //! ������
  std::vector<std::unique_ptr<object::Graffiti>> graffities_;
  //! �����J�X
  std::vector<std::unique_ptr<object::Fragment>> fragments_;
  //! �G�Ǘ�
  enemy::EnemyManager enemy_manager_;
  //! ����Ǘ�
  search::SearchManager search_manager_;
  //! �^�C�}�[
  util::CountDownTimer countdown_timer_;
  //! �X�L���A�C�e���{�b�N�X
  std::vector<std::unique_ptr<skill::SkillItemBox>> item_boxes_;

  //! �g�p����J����
  std::array<std::unique_ptr<camera::Camera>, camera_mode::COUNT> cameras_;
  //! �v���C���[��ǔ����A����ŉ�]�ł���J�����ւ̃|�C���^
  camera::LookAtTargetCamera* player_follow_lookat_camera_;
  //! ���ݎg�p���Ă���J����
  camera_mode::Enum current_camera_;

  //! UI�\���{�[�h
  ui::UIBoard ui_board_;
  //! UI�Q�[�W�z��
  std::vector<ui::Gauge*> gauges_;
  //! ���l�摜�z��
  std::vector<ui::Number*> numbers_;

  std::set<object::Graffiti*> remove_graffiti_list_;
  std::set<object::Fragment*> remove_fragment_list_;
  std::set<skill::SkillItemBox*> remove_item_box_list_;
  scenes::FadeInOut fade_;
  bool is_scene_all_end_;
  bool is_scene_end_fade_start_;

  actor::ActorRenderCommandList actor_render_command_list_;

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

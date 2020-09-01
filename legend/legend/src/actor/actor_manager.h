#ifndef LEGEND_ACTOR_ACTOR_MANAGER_H_
#define LEGEND_ACTOR_ACTOR_MANAGER_H_
#include <set>

#include "src/actor/actor_mediator.h"
#include "src/actor/actor_render_command_list.h"
#include "src/bullet/physics_field.h"
#include "src/camera/camera_manager.h"
#include "src/enemy/enemy_manager.h"
#include "src/object/desk.h"
#include "src/object/fragment.h"
#include "src/object/graffiti.h"
#include "src/player/player.h"
#include "src/search/search_manager.h"
#include "src/skill/skill_item_box.h"
#include "src/stage_generate/stage_generator.h"
#include "src/system/mode.h"

namespace legend {
namespace system {
class TurnSystem;
}  // namespace system
namespace actor {

class ActorManager : public actor::IActorMediator {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  ActorManager();
  /**
   * @brief �f�X�g���N�^
   */
  ~ActorManager();
  /**
   * @brief ������
   * @param stage_name �X�e�[�W��
   */
  bool Init(const std::string& stage_name, system::TurnSystem* turn_system_);
  /**
   * @brief �X�V����
   */
  bool Update();
  /**
   * @brief �`�揈��
   */
  void DrawDifferedRenderingObject(directx::device::CommandList& command_list);
  void DrawAlphaObject(directx::device::CommandList& command_list);
  void Draw2D(directx::device::CommandList& command_list);
  void DrawEnd();

  /**
   * @brief �f�o�b�O�`��
   */
  void DebugDraw();
  /**
   * @brief �e�A�N�^�[�̐���
   */
  bool GenerateActors(i32 currnt_turn);
  /**
   * @brief �Q�[�����N���A������
   */
  bool IsGameClear() const;
  /**
   * @brief �G�̐����擾
   */
  i32 GetEnemiesSize();
  /**
   * @brief �G���~�܂��Ă��邩�ǂ���
   */
  bool IsAllEnemeyStop();
  enemy::EnemyManager* GetEnemyManager();
  void EnemyManagerUpdate();
  bool IsBossGenerated();

 public:
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
  virtual player::Player* GetPlayer() const override;
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
  system::Mode GetCurrentTurn() const override;
  /**
   * @brief ��~���Ԃ�ǉ�����
   */
  virtual void AddStopTime(float time) override;
  /**
   * @brief ��~���Ԃ��w�肷��
   */
  virtual void SetStopTime(float time) override;

  virtual void AddFragment(std::unique_ptr<object::Fragment> fragment) override;

  camera::CameraManager& GetCameraManager() { return camera_manager_; }

 private:
  /**
   * @brief �����J�X�̒ǉ�
   */
  // void AddFragment(std::unique_ptr<object::Fragment> fragment);

 private:
  bullet::PhysicsField physics_field_;
  //! �X�e�[�W����
  stage_generate::StageGenerator stage_generator_;
  //! �v���C���[
  std::unique_ptr<player::Player> player_;
  //! �Î~�I�u�W�F�N�g
  std::vector<std::unique_ptr<Actor>> static_actors_;

  std::vector<std::unique_ptr<Actor>> alpha_actors_;
  //! �G�Ǘ�
  enemy::EnemyManager enemy_manager_;
  //! ����Ǘ�
  search::SearchManager search_manager_;

  std::set<Actor*> remove_alpha_actors_;
  std::set<Actor*> remove_static_actors_;

  actor::ActorRenderCommandList actor_render_command_list_;

  //�^�[���V�X�e���̃|�C���^
  system::TurnSystem* turn_system_;

  //! �^�C�}�[
  util::CountDownTimer countdown_timer_;
  camera::CameraManager camera_manager_;

  //��~�����鎞��
  float hit_stop_time_;

  //�{�X���������ꂽ�ォ�ǂ���
  bool is_boss_generated_;
};

}  // namespace actor
}  // namespace legend

#endif  //! LEGEND_ACTOR_ACTOR_MANAGER_H_

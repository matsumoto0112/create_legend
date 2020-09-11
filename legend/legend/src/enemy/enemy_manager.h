#ifndef LEGEND_ENEMY_ENEMY_MANAGER_H_
#define LEGEND_ENEMY_ENEMY_MANAGER_H_

#include <vector>

#include "src/actor/actor_mediator.h"
#include "src/actor/differed_rendering_render_command_list.h"
#include "src/enemy/boss.h"
#include "src/enemy/enemy.h"
#include "src/enemy/enemy_actor.h"
#include "src/player/player.h"
#include "src/search/search_manager.h"

namespace legend {
namespace enemy {
/**
 * @class EnemyManager
 * @brief �G�l�~�[�Ǘ��N���X
 */
class EnemyManager {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  EnemyManager();
  /**
   * @brief �f�X�N�g���N�^
   */
  ~EnemyManager();
  /**
   * @brief ������
   * @param mediator �����
   */
  bool Initilaize(actor::IActorMediator* mediator);
  /**
   * @brief �X�V
   * @param search_manaegr �T���Ǘ�
   * @return �X�V������TRUE
   */
  bool Update(search::SearchManager* search_manaegr);
  /**
   * @brief �`��
   * @param render_command_list �`��R�}���h���X�g
   */
  void Draw(actor::DifferedRenderingRenderCommandList* render_command_list);
  /**
   * @brief �G�̍s������
   * @param search_manaegr �T���Ǘ�
   */
  void EnemyAction(search::SearchManager* search_manaegr);
  /**
   * @brief �G��ǉ�
   * @param paramater �������p�����[�^
   */
  void AddEnemy(const EnemyActor::InitializeParameter& paramater);
  /**
   * @brief �{�X��ǉ�
   * @param paramater �������p�����[�^
   */
  void AddBoss(const EnemyActor::InitializeParameter& paramater);
  /**
   * @brief �{�X��ǉ�
   * @return ����I��
   */
  bool AbsorpEnemies();
  /**
   * @brief �폜����
   */
  void DestroyUpdate();
  /**
   * @brief �G���폜
   * @param index �폜�ԍ�
   */
  void Destroy(i32 index);
  /**
   * @brief �{�X���폜
   */
  void DestroyBoss();
  /**
   * @brief �|�W�V�����ݒ�
   * @param enemy �G
   */
  void SetPosition(Enemy* enemy);
  /**
   * @brief �Ō�̓G���擾
   * @return �z��Ō���̓G�擾
   */
  Enemy* GetLastEnemy() const;
  /**
   * @brief �G�̐����擾
   * @return �G�z��擾
   */
  i32 GetEnemiesSize() const;
  /**
   * @brief �G�̍s����������
   */
  void ResetEnemyMove();
  /**
   * @brief �e�G�̑��x���擾
   * @return �G�̑��x�z��
   */
  std::vector<math::Vector3> GetVelocities();
  /**
   * @brief �Ō�̓G�̈ړ��I������̎擾
   * @return �z��Ō���̓G����~���Ă��邩����
   */
  bool LastEnemyMoveEnd() const;
  /**
   * @brief �{�X�̎擾
   * @return �{�X�̎擾
   */
  enemy::Boss* GetBoss() const { return boss_.get(); };
  /**
   * @brief �N���A����
   * @return �N���A����
   */
  bool IsGameClear() const { return is_game_clear_; };
  /**
   * @brief �v���C���[�ݒ�
   * @param player_obb �v���C���[���Ƃ���
   */
  void SetPlayer(bullet::Collider* player_obb);
  /**
   * @brief �G�̃|�C���^���X�g�擾
   * @return �G�̃|�C���^���X�g�擾
   */
  std::vector<Enemy*> GetEnemyPointers() const;

 private:
  //! �G���X�g
  std::vector<std::unique_ptr<Enemy>> enemys_;
  //! �{�X
  std::unique_ptr<Boss> boss_;
  //! �s���I�ԍ�
  i32 action_enemy_index_ = -1;
  //! �G�̍ő吶����
  i32 enemy_max_count_ = 50;
  //! �N���A����
  bool is_game_clear_ = false;
  //! �ړ����x�ő�l
  float move_speed_max_ = 60.0f;
  //! �ړ����x�Œ�l
  float move_speed_min_ = 20.0f;
  //! �ғ��ړ�����
  float move_timer_ = 0.0f;
  //! �ړ�����
  const float move_time_ = 0.5f;

  //! �e�G�̑��x�i�[���X�g
  std::vector<math::Vector3> velocities_;
  //! �v���C���[�̏Փ˔���
  bullet::Collider* player_collider_;

  //! �����
  actor::IActorMediator* actor_mediator_;
};
}  // namespace enemy
}  // namespace legend

#endif LEGEND_ENEMY_ENEMY_MANAGER_H_
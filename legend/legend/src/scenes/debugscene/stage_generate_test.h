#ifndef LEGEND_SCENES_DEBUGSCENE_STAGE_GENERATE_TEST_H_
#define LEGEND_SCENES_DEBUGSCENE_STAGE_GENERATE_TEST_H_

/**
 * @brief �X�e�[�W�����e�X�g
 */

#include "src/camera/perspective_camera.h"
#include "src/object/desk.h"
#include "src/player/player.h"
#include "src/scenes/scene.h"
#include "src/stage_generate/stage_generator.h"
#include "src/enemy/enemy_manager.h"

namespace legend {
namespace scenes {
namespace debugscene {
/**
 * @class SoundTest
 * @brief �X�e�[�W�����e�X�g�V�[��
 */
class StageGenerateTest : public Scene {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  StageGenerateTest(ISceneChange* scene_change);
  /**
   * @brief �f�X�g���N�^
   */
  ~StageGenerateTest();
  /**
   * @brief ������
   */
  bool Initialize() override;
  /**
   * @brief �X�V
   */
  bool Update() override;
  /**
   * @brief �`��
   */
  void Draw() override;

  /**
   * @brief �I������
   */
  void Finalize() override;

 private:
  //! �X�e�[�W�����N���X
  std::unique_ptr<stage_generate::StageGenerator> stage_generator_;
  //! �ǂݍ��񂾃X�e�[�W�f�[�^�i�[�p
  std::vector<std::string> indexs_;
  //! ���C���J����
  camera::PerspectiveCamera camera_;
  //! �v���C���[
  player::Player player_;
  //! ��
  object::Desk desk_;
  //! �G�Ǘ��V�X�e��
  enemy::EnemyManager enemy_manager_;
};

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_DEBUGSCENE_STAGE_GENERATE_TEST_H_

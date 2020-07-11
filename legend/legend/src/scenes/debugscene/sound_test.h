#ifndef LEGEND_SCENES_DEBUGSCENE_SOUND_TEST_H_
#define LEGEND_SCENES_DEBUGSCENE_SOUND_TEST_H_

/**
 * @brief �T�E���h�e�X�g�V�[��
 */

#include "src/scenes/scene.h"

namespace legend {
namespace scenes {
namespace debugscene {
/**
 * @class SoundTest
 * @brief ���f���`��e�X�g�V�[��
 */
class SoundTest : public Scene {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  SoundTest(ISceneChange* scene_change);
  /**
   * @brief �f�X�g���N�^
   */
  ~SoundTest();
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
};

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_DEBUGSCENE_SOUND_TEST_H_

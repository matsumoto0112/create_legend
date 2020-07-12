#ifndef LEGEND_SCENES_DEBUGSCENE_PHYSICS_TEST_H_
#define LEGEND_SCENES_DEBUGSCENE_PHYSICS_TEST_H_

#include "src/scenes/scene.h"

namespace legend {
namespace scenes {
namespace debugscene {

/**
 * @class PhysicsTest
 * @brief �����֌W�̃e�X�g�V�[��
 */
class PhysicsTest : public Scene {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  PhysicsTest(ISceneChange* scene_change);
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
};

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_DEBUGSCENE_PHYSICS_TEST_H_

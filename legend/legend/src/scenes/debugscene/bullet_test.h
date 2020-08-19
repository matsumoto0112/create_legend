#ifndef LEGEND_SCENES_DEBUGSCENE_BULLET_TEST_H_
#define LEGEND_SCENES_DEBUGSCENE_BULLET_TEST_H_

/**
 * @file bullet_test.h
 * @brief bullet�e�X�g�V�[��
 */

#include "src/bullet/bounding_box.h"
#include "src/bullet/physics_field.h"
#include "src/scenes/scene.h"

namespace legend {
namespace scenes {
namespace debugscene {

class BulletTest : public Scene {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  BulletTest(ISceneChange* scene_change);
  /**
   * @brief �f�X�g���N�^
   */
  ~BulletTest();
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
  bullet::PhysicsField physics_field_;
};
}  // namespace debugscene
}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_DEBUGSCENE_BULLET_TEST_H_

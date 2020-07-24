#ifndef LEGEND_SCENES_DEBUGSCENE_PHYSICS_TEST_H_
#define LEGEND_SCENES_DEBUGSCENE_PHYSICS_TEST_H_

#include "src/directx/buffer/render_target_texture.h"
#include "src/directx/shader/graphics_pipeline_state.h"
#include "src/directx/shader/root_signature.h"
#include "src/physics/bounding_box.h"
#include "src/physics/plane.h"
#include "src/physics/ray.h"
#include "src/physics/sphere.h"
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
   * @brief �f�X�g���N�^
   */
  ~PhysicsTest();
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
   * @brief �I��
   */
  void Finalize() override;

 private:
  //! ������
  std::vector<physics::BoundingBox> obbs_;
  //! ������
  i32 obb_num_ = 1;
  //! ����
  physics::Plane plane_;
  //! ��
  physics::Sphere sphere_;
  //! ���C
  physics::Ray ray_;
};

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_DEBUGSCENE_PHYSICS_TEST_H_

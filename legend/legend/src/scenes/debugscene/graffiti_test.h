#ifndef LEGEND_SCENES_DEBUGSCENE_GRAFFITI_TEST_H_
#define LEGEND_SCENES_DEBUGSCENE_GRAFFITI_TEST_H_

/**
 * @file graffiti_test.h
 */

#include "src/camera/perspective_camera.h"
#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/buffer/constant_buffer_structure.h"
#include "src/scenes/scene.h"
#include "src/util/transform.h"

namespace legend {
namespace scenes {
namespace debugscene {
/**
 * @class GraffitiTest
 * @brief �������e�X�g
 */
class GraffitiTest : public Scene {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  GraffitiTest(ISceneChange* scene_change);
  /**
   * @brief �f�X�g���N�^
   */
  ~GraffitiTest();
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
  camera::PerspectiveCamera camera_;

  util::Transform transform_;
  directx::buffer::ConstantBuffer<
      directx::buffer::constant_buffer_structure::Transform>
      transform_cb_;

  struct GraffitiStruct {
    float alpha;
  };
  directx::buffer::ConstantBuffer<GraffitiStruct> graffiti_struct_cb_;
};
}  // namespace debugscene
}  // namespace scenes
}  // namespace legend
#endif  //! LEGEND_SCENES_DEBUGSCENE_GRAFFITI_TEST_H_

#ifndef LEGEND_SCENES_DEBUGSCENE_PHYSICS_TEST_H_
#define LEGEND_SCENES_DEBUGSCENE_PHYSICS_TEST_H_

#include "src/directx/buffer/render_target_texture.h"
#include "src/directx/shader/graphics_pipeline_state.h"
#include "src/directx/shader/root_signature.h"
#include "src/physics/bounding_box.h"
#include "src/physics/plane.h"
#include "src/scenes/debugscene/model_view.h"
#include "src/scenes/scene.h"

namespace legend {
namespace scenes {
namespace debugscene {

/**
 * @class PhysicsTest
 * @brief 物理関係のテストシーン
 */
class PhysicsTest : public Scene {
 public:
  /**
   * @brief コンストラクタ
   */
  PhysicsTest(ISceneChange* scene_change);
  /**
   * @brief デストラクタ
   */
  ~PhysicsTest();
  /**
   * @brief 初期化
   */
  bool Initialize() override;
  /**
   * @brief 更新
   */
  bool Update() override;
  /**
   * @brief 描画
   */
  void Draw() override;
  /**
   * @brief 終了
   */
  void Finalize() override;

 private:
  //! 直方体
  std::vector<physics::BoundingBox> obbs_;
  //! 生成数
  i32 obb_num_ = 2;
  //! 平面
  physics::Plane plane_;

  directx::buffer::ConstantBuffer<WorldContext> world_constant_buffer_;
  std::shared_ptr<directx::shader::RootSignature> root_signature_;
  directx::shader::GraphicsPipelineState pipeline_state_;
};

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_DEBUGSCENE_PHYSICS_TEST_H_

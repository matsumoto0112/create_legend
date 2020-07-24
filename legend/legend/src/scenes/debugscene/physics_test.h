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
  i32 obb_num_ = 1;
  //! 平面
  physics::Plane plane_;
  //! 球
  physics::Sphere sphere_;
  //! レイ
  physics::Ray ray_;
};

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_DEBUGSCENE_PHYSICS_TEST_H_

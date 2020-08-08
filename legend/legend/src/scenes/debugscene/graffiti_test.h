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
 * @brief 落書きテスト
 */
class GraffitiTest : public Scene {
 public:
  /**
   * @brief コンストラクタ
   */
  GraffitiTest(ISceneChange* scene_change);
  /**
   * @brief デストラクタ
   */
  ~GraffitiTest();
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
   * @brief 終了処理
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

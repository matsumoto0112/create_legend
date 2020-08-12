#ifndef LEGEND_SCENES_DEBUGSCENE_GPU_PARTICLE_TEST_H_
#define LEGEND_SCENES_DEBUGSCENE_GPU_PARTICLE_TEST_H_

/**
 * @file gpu_particle_test.h
 */

#include "src/camera/perspective_camera.h"
#include "src/scenes/scene.h"

namespace legend {
namespace scenes {
namespace debugscene {

class GPUParticleTest : public Scene {
 public:
  GPUParticleTest(ISceneChange* scene_change);
  ~GPUParticleTest();
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
};

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend
#endif  //! LEGEND_SCENES_DEBUGSCENE_GPU_PARTICLE_TEST_H_

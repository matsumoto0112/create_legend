#ifndef LEGEND_SCENES_DEBUGSCENE_GPU_PARTICLE_TEST_H_
#define LEGEND_SCENES_DEBUGSCENE_GPU_PARTICLE_TEST_H_

/**
 * @file gpu_particle_test.h
 */

#include "src/camera/perspective_camera.h"
#include "src/draw/particle/smoke_particle.h"
#include "src/scenes/scene.h"

namespace legend {
namespace scenes {
namespace debugscene {

class GPUParticleTest : public Scene {
 public:
  GPUParticleTest(ISceneChange* scene_change);
  ~GPUParticleTest();
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

  draw::particle::SmokeParticle smoke_particle_;
};

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend
#endif  //! LEGEND_SCENES_DEBUGSCENE_GPU_PARTICLE_TEST_H_

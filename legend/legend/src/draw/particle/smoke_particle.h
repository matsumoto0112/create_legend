#ifndef LEGEND_DRAW_PARTICLE_SMOKE_PARTICLE_H_
#define LEGEND_DRAW_PARTICLE_SMOKE_PARTICLE_H_

/**
 * @file smoke_particle.h
 * @brief ���p�[�e�B�N���e�X�g
 */

#include "assets/shaders/particle/gpu_particle_test.h"
#include "src/draw/particle/particle_emitter.h"

namespace legend {
namespace draw {
namespace particle {
class SmokeParticle : public ParticleEmitter {
 public:
  SmokeParticle();
  ~SmokeParticle();

  bool Init(directx::device::CommandList& copy_command_list);
  void Update(ParticleCommandList& command_list) override;
  void Render(directx::device::CommandList& graphics_command_list) override;

 private:
  static constexpr u32 PARTICLE_NUM =
      THREAD_X * THREAD_Y * DISPATCH_X * DISPATCH_Y;
};

}  // namespace particle
}  // namespace draw
}  // namespace legend
#endif  //! LEGEND_DRAW_PARTICLE_SMOKE_PARTICLE_H_

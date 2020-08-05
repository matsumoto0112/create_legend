#ifndef LEGEND_DRAW_PARTICLE_SMOKE_PARTICLE_H_
#define LEGEND_DRAW_PARTICLE_SMOKE_PARTICLE_H_

/**
 * @file smoke_particle.h
 */

#include "src/draw/particle/particle_emitter.h"

namespace legend {
namespace draw {
namespace particle {
class SmokeParticle : public ParticleEmitter {
 public:
  SmokeParticle();
  ~SmokeParticle();
  virtual bool Init(
      std::shared_ptr<directx::buffer::Texture2D> texture) override;
  virtual bool Update(const math::Matrix4x4& billboard_matrix) override;
};

}  // namespace particle
}  // namespace draw
}  // namespace legend
#endif  //! LEGEND_DRAW_PARTICLE_SMOKE_PARTICLE_H_

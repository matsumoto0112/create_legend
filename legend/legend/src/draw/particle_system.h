#ifndef LEGEND_DRAW_PARTICLE_SYSTEM_H_
#define LEGEND_DRAW_PARTICLE_SYSTEM_H_

/**
 * @file particle_system.h
 */

#include "src/util/resource/resource_id.h"
#include "src/util/transform.h"

namespace legend {
namespace draw {
class ParticleSystem {
 public:
  ParticleSystem();
  ~ParticleSystem();
  bool Init(util::resource::id::Texture texture);
  bool Update(math::Vector3 v);
  void Render();

 private:
  util::Transform transform;
};

}  // namespace draw
}  // namespace legend
#endif  //! LEGEND_DRAW_PARTICLE_SYSTEM_H_

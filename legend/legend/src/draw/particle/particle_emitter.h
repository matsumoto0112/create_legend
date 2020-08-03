#ifndef LEGEND_DRAW_PARTICLE_PARTICLE_EMITTER_H_
#define LEGEND_DRAW_PARTICLE_PARTICLE_EMITTER_H_

/**
 * @file particle_emitter.h
 */

#include <deque>

#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/buffer/index_buffer.h"
#include "src/directx/buffer/texture_2d.h"
#include "src/directx/constant_buffer_structure.h"
#include "src/util/transform.h"

namespace legend {
namespace draw {
namespace particle {

class ParticleEmitter {
 public:
  ParticleEmitter();
  virtual ~ParticleEmitter();
  virtual bool Init(std::shared_ptr<directx::buffer::Texture2D> texture);
  virtual bool Update(const math::Matrix4x4& billboard_matrix) = 0;
  virtual void Render(directx::buffer::IndexBuffer& draw_index_buffer);

 protected:
  util::Transform emit_transform_;
  std::deque<util::Transform> particle_transforms_;
  std::vector<legend::directx::buffer::ConstantBuffer<
      legend::directx::constant_buffer_structure::Transform>>
      constant_buffers_;

  std::shared_ptr<directx::buffer::Texture2D> texture_;
};

}  // namespace particle
}  // namespace draw
}  // namespace legend
#endif  //! LEGEND_DRAW_PARTICLE_PARTICLE_EMITTER_H_

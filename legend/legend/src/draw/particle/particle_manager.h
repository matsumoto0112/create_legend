#ifndef LEGEND_DRAW_PARTICLE_PARTICLE_MANAGER_H_
#define LEGEND_DRAW_PARTICLE_PARTICLE_MANAGER_H_

/**
 * @file particle_manager.h
 */

#include "src/directx/buffer/index_buffer.h"
#include "src/directx/buffer/vertex_buffer.h"
#include "src/directx/shader/graphics_pipeline_state.h"
#include "src/draw/particle/particle_emitter.h"

namespace legend {
namespace draw {
namespace particle {
class ParticleManager {
 public:
  ParticleManager();
  ~ParticleManager();
  bool Init();
  void SetDrawList(ParticleEmitter* emitter);
  void DrawParticles();

 private:
  std::vector<ParticleEmitter*> emitters_;
  legend::directx::buffer::VertexBuffer vertex_buffer_;
  legend::directx::buffer::IndexBuffer index_buffer_;
  legend::directx::shader::GraphicsPipelineState pipeline_state_;
};

}  // namespace particle
}  // namespace draw
}  // namespace legend
#endif  //! LEGEND_DRAW_PARTICLE_PARTICLE_MANAGER_H_

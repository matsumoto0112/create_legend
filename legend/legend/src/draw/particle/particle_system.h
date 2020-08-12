#ifndef LEGEND_DRAW_PARTICLE_PARTICLE_SYSTEM_H_
#define LEGEND_DRAW_PARTICLE_PARTICLE_SYSTEM_H_

/**
 * @file particle_system.h
 */

#include "assets/shaders/particle/gpu_particle_test.h"
#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/buffer/constant_buffer_structure.h"
#include "src/directx/frame_resource.h"
#include "src/util/color_4.h"

namespace legend {
namespace draw {
namespace particle {
class ParticleSystem {
 public:
  ParticleSystem();
  ~ParticleSystem();

  bool Init();
  void Update(directx::device::CommandList& compute_command_list);
  void Render(directx::device::CommandList& graphics_command_list);

  static constexpr u32 PARTICLE_NUM =
      THREAD_X * THREAD_Y * DISPATCH_X * DISPATCH_Y;

 private:
  ComPtr<ID3D12PipelineState> graphics_pipeline_state_;
  ComPtr<ID3D12PipelineState> compute_pipeline_state_;

  directx::buffer::ConstantBuffer<
      directx::buffer::constant_buffer_structure::Transform>
      transform_cb_;
  directx::buffer::ConstantBuffer<
      directx::buffer::constant_buffer_structure::WorldContext>
      world_cb_;

  ComPtr<ID3D12Resource> particle_uav_;
  ComPtr<ID3D12Resource> particle_uav_upload_;
  directx::descriptor_heap::DescriptorHandle handle_;
  D3D12_VERTEX_BUFFER_VIEW vertex_buffer_view_;
};

}  // namespace particle
}  // namespace draw
}  // namespace legend
#endif  //! LEGEND_DRAW_PARTICLE_PARTICLE_SYSTEM_H_

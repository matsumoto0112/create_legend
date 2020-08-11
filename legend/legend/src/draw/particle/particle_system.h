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
  void Execute();

  static constexpr u32 PARTICLE_NUM =
      THREAD_X * THREAD_Y * DISPATCH_X * DISPATCH_Y;

  struct IndirectCommand {
    D3D12_DRAW_ARGUMENTS draw_argument;
  };

 private:
  ComPtr<ID3D12CommandQueue> compute_command_queue_;
  ComPtr<ID3D12PipelineState> graphics_pipeline_state_;
  ComPtr<ID3D12PipelineState> compute_pipeline_state_;

  directx::FrameResource frame_resources_[3];
  directx::FrameResource* current_resource_;
  // directx::device::CommandList compute_command_list_;
  ComPtr<ID3D12Fence> fence_;
  u64 fence_value_;
  ComPtr<ID3D12CommandSignature> command_signature_;

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
  IndirectCommand command_;
  ComPtr<ID3D12Resource> command_resource_;
  ComPtr<ID3D12DescriptorHeap> heaps_;
};

}  // namespace particle
}  // namespace draw
}  // namespace legend
#endif  //! LEGEND_DRAW_PARTICLE_PARTICLE_SYSTEM_H_

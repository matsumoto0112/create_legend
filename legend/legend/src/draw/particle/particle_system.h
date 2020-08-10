#ifndef LEGEND_DRAW_PARTICLE_PARTICLE_SYSTEM_H_
#define LEGEND_DRAW_PARTICLE_PARTICLE_SYSTEM_H_

/**
 * @file particle_system.h
 */

#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/buffer/constant_buffer_structure.h"

namespace legend {
namespace draw {
namespace particle {
class ParticleSystem {
 public:
  ParticleSystem();
  ~ParticleSystem();

  bool Init();
  void Execute();

  struct Particle {
    math::Vector3 position;
  };
  static constexpr u32 THREAD_X = 1;
  static constexpr u32 THREAD_Y = 1;
  static constexpr u32 DISPATCH_X = 1;
  static constexpr u32 DISPATCH_Y = 1;
  static constexpr u32 PARTICLE_NUM =
      THREAD_X * THREAD_Y * DISPATCH_X * DISPATCH_Y;

 private:
  ComPtr<ID3D12CommandQueue> compute_command_queue_;
  ComPtr<ID3D12CommandAllocator> compute_command_alocator_;
  ComPtr<ID3D12PipelineState> graphics_pipeline_state_;
  ComPtr<ID3D12PipelineState> compute_pipeline_state_;
  ComPtr<ID3D12GraphicsCommandList> compute_command_list_;

  directx::buffer::ConstantBuffer<
      directx::buffer::constant_buffer_structure::Transform>
      transform_cb_;
  directx::buffer::ConstantBuffer<
      directx::buffer::constant_buffer_structure::WorldContext>
      world_cb_;

  ComPtr<ID3D12Resource> particle_uav_;
  ComPtr<ID3D12Resource> particle_uav_upload_;
  directx::descriptor_heap::DescriptorHandle handle_;
};

}  // namespace particle
}  // namespace draw
}  // namespace legend
#endif  //! LEGEND_DRAW_PARTICLE_PARTICLE_SYSTEM_H_

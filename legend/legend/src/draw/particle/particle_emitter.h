#ifndef LEGEND_DRAW_PARTICLE_PARTICLE_EMITTER_H_
#define LEGEND_DRAW_PARTICLE_PARTICLE_EMITTER_H_

/**
 * @file particle_emitter.h
 */

#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/buffer/constant_buffer_structure.h"
#include "src/directx/device/command_list.h"
#include "src/draw/particle/particle_manager.h"

namespace legend {
namespace draw {
namespace particle {
class ParticleEmitter {
 protected:
  using TransformStruct = directx::buffer::constant_buffer_structure::Transform;
  using TransformConstantBuffer =
      directx::buffer::ConstantBuffer<TransformStruct>;

 public:
  ParticleEmitter(u32 particle_max_size, u32 particle_structure_size,
                  u32 dispatch_x, u32 dispatch_y, const std::wstring& name);
  ~ParticleEmitter();

  virtual bool Init(directx::device::CommandList& copy_command_list,
                    const void* data);
  virtual bool Update(directx::device::CommandList& compute_command_list);
  virtual bool Render(directx::device::CommandList& graphics_command_list);

 protected:
  ComPtr<ID3D12PipelineState> graphics_pipeline_state_;
  ComPtr<ID3D12PipelineState> compute_pipeline_state_;

  const std::wstring name_;
  const u32 particle_max_num_;
  const u32 particle_structure_size_;
  const u32 dispatch_x_;
  const u32 dispatch_y_;

  TransformConstantBuffer transform_cb_;

  ComPtr<ID3D12Resource> particle_uav_;
  ComPtr<ID3D12Resource> particle_uav_upload_;
  directx::descriptor_heap::DescriptorHandle handle_;
  D3D12_VERTEX_BUFFER_VIEW vertex_buffer_view_;
};

}  // namespace particle
}  // namespace draw
}  // namespace legend
#endif  //! LEGEND_DRAW_PARTICLE_PARTICLE_EMITTER_H_

#ifndef LEGEND_DRAW_PARTICLE_PARTICLE_MANAGER_H_
#define LEGEND_DRAW_PARTICLE_PARTICLE_MANAGER_H_

/**
 * @file particle_manager.h
 */

#include "src/directx/device/directx_accessor.h"
#include "src/directx/device/directx_device.h"
#include "src/directx/frame_resource.h"

namespace legend {
namespace draw {
namespace particle {
class ParticleManager {
 public:
  ParticleManager();
  ~ParticleManager();
  bool Init(directx::device::IDirectXAccessor& accessor, u32 frame_count);
  void BeginFrame(directx::device::DirectXDevice& device);
  void Execute();
  directx::device::CommandList& GetCommandList() {
    return current_frame_resource_->GetCommandList();
  }

 private:
  std::vector<directx::FrameResource> frame_resources_;
  directx::FrameResource* current_frame_resource_;
  ComPtr<ID3D12CommandQueue> command_queue_;
  ComPtr<ID3D12Fence> fence_;
  u64 fence_value_;
};

}  // namespace particle
}  // namespace draw
}  // namespace legend
#endif  //! LEGEND_DRAW_PARTICLE_PARTICLE_MANAGER_H_

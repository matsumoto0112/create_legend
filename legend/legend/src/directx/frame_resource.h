#ifndef LEGEND_DIRECTX_FRAME_RESOURCE_H_
#define LEGEND_DIRECTX_FRAME_RESOURCE_H_

/**
 * @file frame_resource.h
 */

#include "src/directx/device/command_list.h"

namespace legend {
namespace directx {

class FrameResource {
 public:
  std::vector<ID3D12CommandList*> batch_submit_;

  std::vector<directx::device::CommandList> command_lists_;

  u64 fence_value_;

  FrameResource() {}
  ~FrameResource() {}
  bool Init(ID3D12Device* device);
  bool AddCommandList(ID3D12Device* device);
  bool Ready();
  void Destroy();
};
}  // namespace directx
}  // namespace legend
#endif  //! LEGEND_DIRECTX_FRAME_RESOURCE_H_

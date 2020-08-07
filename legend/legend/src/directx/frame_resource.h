#ifndef LEGEND_DIRECTX_FRAME_RESOURCE_H_
#define LEGEND_DIRECTX_FRAME_RESOURCE_H_

/**
 * @file frame_resource.h
 */

#include "src/directx/device/command_list.h"
#include "src/directx/device/directx_accessor.h"

namespace legend {
namespace directx {

class FrameResource {
 public:
  FrameResource() {}
  ~FrameResource() {}
  bool Init(device::IDirectXAccessor& accessor);
  bool Ready();
  void Destroy();
  device::CommandList& GetCommandList() { return command_lists_; }

 public:
  u64 fence_value_;

 private:
  device::CommandList command_lists_;
};
}  // namespace directx
}  // namespace legend
#endif  //! LEGEND_DIRECTX_FRAME_RESOURCE_H_

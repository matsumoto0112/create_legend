#ifndef LEGEND_DIRECTX_DEVICE_DIRECTX_DEVICE_H_
#define LEGEND_DIRECTX_DEVICE_DIRECTX_DEVICE_H_

/**
 * @file directx_device.h
 */

#include "src/directx/descriptor_heap/heap_manager.h"
#include "src/directx/device/command_list.h"
#include "src/directx/device/directx_accessor.h"
#include "src/directx/device/dxgi_adapter.h"
#include "src/directx/device/swap_chain.h"
#include "src/directx/frame_resource.h"
#include "src/directx/render_target/depth_stencil.h"

namespace legend {
namespace directx {
namespace device {

class DirectXDevice : public IDirectXAccessor {
 public:
  DirectXDevice();
  ~DirectXDevice();

  bool Init(u32 width, u32 height, HWND hwnd);
  bool Prepare();
  bool Present();
  void Destroy();

  static constexpr u32 FRAME_COUNT = 3;
  static constexpr bool USE_WARP_DEVICE = false;

  enum CommandListID {
    PRE_COMMAND_LIST_ID,
    MID_COMMAND_LIST_ID,
    POST_COMMAND_LIST_ID,
    COUNT,
  };

 public:
  ID3D12Device* GetDevice() const override { return device_.Get(); }
  descriptor_heap::DescriptorHandle GetRTVHandle() override;
  descriptor_heap::DescriptorHandle GetDSVHandle() override;

 //private:
  directx::device::DXGIAdapter adapter_;

  ComPtr<ID3D12Device> device_;
  ComPtr<ID3D12CommandQueue> command_queue_;
  SwapChain swap_chain_;

  u32 frame_index_;

  HANDLE fence_event_;
  ComPtr<ID3D12Fence> fence_;
  UINT64 fence_value_;

  render_target::DepthStencil depth_stencil_;

  FrameResource resources_[FRAME_COUNT];
  FrameResource* current_resource_;

  directx::descriptor_heap::HeapManager heap_manager_;
};

}  // namespace device
}  // namespace directx
}  // namespace legend
#endif  //! LEGEND_DIRECTX_DEVICE_DIRECTX_DEVICE_H_

#include "src/directx/render_target/render_resource_manager.h"

#include "src/util/stl_extend.h"

namespace legend {
namespace directx {
namespace render_target {

//コンストラクタ
RenderResourceManager::RenderResourceManager() {}

//デストラクタ
RenderResourceManager::~RenderResourceManager() {}

bool RenderResourceManager::Init(device::IDirectXAccessor& accessor,
                                 device::DXGIAdapter& adapter, u32 frame_count,
                                 u32 width, u32 height, DXGI_FORMAT format,
                                 HWND hwnd, ID3D12CommandQueue* command_queue) {
  this->frame_count_ = frame_count;
  if (!swap_chain_.Init(accessor, adapter, frame_count, width, height, format,
                        hwnd, command_queue)) {
    return false;
  }

  render_targets_.emplace(
      RenderTargetID::BACK_BUFFER,
      MultiFrameMultiRenderTargetTexture(frame_count, swap_chain_));
  auto& rtt = render_targets_.at(RenderTargetID::BACK_BUFFER);
  rtt.mrt.resize(frame_count);

  for (u32 i = 0; i < frame_count; i++) {
    std::wstringstream wss;
    wss << L"BackBuffer " << i;
    if (!rtt.mrt[i].InitFromBuffer(accessor, swap_chain_.GetBuffer(i),
                                   util::Color4(1.0f, 0.0f, 0.0f, 1.0f),
                                   wss.str())) {
      return false;
    }
  }

  this->depth_stencil_targets_.clear();
  this->current_render_target_id_ = RenderTargetID::BACK_BUFFER;
  this->current_depth_stencil_target_id_ = DepthStencilTargetID::NONE;
  return true;
}

void RenderResourceManager::ClearCurrentRenderTarget(
    device::CommandList& command_list) const {
  render_targets_.at(current_render_target_id_)
      .Get()
      .ClearRenderTarget(command_list);
}

void RenderResourceManager::ClearCurrentDepthStencil(
    device::CommandList& command_list) const {
  depth_stencil_targets_.at(current_depth_stencil_target_id_)
      .Get()
      .ClearDepthStencil(command_list);
}

void RenderResourceManager::SetRenderTargets(
    device::CommandList& command_list) {
  MultiRenderTargetTexture& target =
      render_targets_.at(current_render_target_id_).Get();
  target.PrepareToUseRenderTargets(command_list);

  const std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> rtv_handle =
      target.GetRTVHandles();

  const u32 size = static_cast<u32>(rtv_handle.size());
  if (current_depth_stencil_target_id_ == DepthStencilTargetID::NONE) {
    command_list.GetCommandList()->OMSetRenderTargets(size, rtv_handle.data(),
                                                      FALSE, nullptr);
  } else {
    const D3D12_CPU_DESCRIPTOR_HANDLE dsv_handle =
        depth_stencil_targets_.at(current_depth_stencil_target_id_)
            .Get()
            .GetHandle()
            .cpu_handle_;
    command_list.GetCommandList()->OMSetRenderTargets(size, rtv_handle.data(),
                                                      FALSE, &dsv_handle);
  }

  target.SetScissorRect(command_list);
  target.SetViewport(command_list);
}

u32 RenderResourceManager::GetCurrentBackBufferIndex() const {
  return swap_chain_.GetCurrentBackBufferIndex();
}

void RenderResourceManager::DrawEnd(device::CommandList& command_list) {
  render_targets_.at(RenderTargetID::BACK_BUFFER)
      .Get()
      .Transition(command_list,
                  D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT);
}

bool RenderResourceManager::Present() { return swap_chain_.Present(); }

bool RenderResourceManager::AddDepthStencil(
    DepthStencilTargetID id, device::IDirectXAccessor& accessor,
    const DepthStencil::DepthStencilDesc& desc) {
  depth_stencil_targets_.emplace(
      id, MultiFrameDepthStencil(frame_count_, swap_chain_));
  auto& ds = depth_stencil_targets_.at(id);
  for (u32 i = 0; i < frame_count_; i++) {
    if (!ds.ds[i].Init(accessor, desc)) {
      return false;
    }
  }
  return true;
}

}  // namespace render_target
}  // namespace directx
}  // namespace legend

#include "src/directx/render_target/render_resource_manager.h"

#include "src/util/stl_extend.h"

namespace legend {
namespace directx {
namespace render_target {

//コンストラクタ
RenderResourceManager::RenderResourceManager() {}

//デストラクタ
RenderResourceManager::~RenderResourceManager() {}

//初期化
bool RenderResourceManager::Init(IDirectXAccessor& accessor,
                                 device::DXGIAdapter& adapter,
                                 window::Window& target_window, u32 frame_count,
                                 DXGI_FORMAT format,
                                 ID3D12CommandQueue* command_queue) {
  if (!swap_chain_.Init(accessor, adapter, target_window, command_queue,
                        format)) {
    return false;
  }

  this->current_render_target_id_ = RenderTargetID::BACK_BUFFER;
  this->current_depth_stencil_target_id_ = UINT_LEAST32_MAX;
  return true;
}

//レンダーターゲットの作成
bool RenderResourceManager::CreateRenderTarget(IDirectXAccessor& accessor,
                                               RenderTargetID unique_id,
                                               DXGI_FORMAT format, u32 width,
                                               u32 height,
                                               const util::Color4& clear_color,
                                               const std::wstring& name) {
  MY_ASSERTION(!util::Exist(created_render_targets_, unique_id),
               L"unique_idが重複しています。");

  MultiRenderTargetTexture render_target;
  const MultiRenderTargetTexture::Info info{0,      format,      width,
                                            height, clear_color, L""};
  if (!render_target.Init(
          accessor, descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID,
          info)) {
    return false;
  }

  created_render_targets_[unique_id] = render_target;
  return true;
}

//マルチレンダーターゲットの作成
bool RenderResourceManager::CreateRenderTargets(IDirectXAccessor& accessor,
                                                RenderTargetID unique_id,
                                                const std::vector<Info>& info) {
  MY_ASSERTION(!util::Exist(created_render_targets_, unique_id),
               L"unique_idが重複しています。");

  MultiRenderTargetTexture render_target;
  if (!render_target.Init(
          accessor, descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID,
          info)) {
    return false;
  }

  created_render_targets_[unique_id] = render_target;
  return true;
}

//デプス・ステンシルを作成する
bool RenderResourceManager::CreateDepthStencil(
    IDirectXAccessor& accessor, u32 unique_id, DXGI_FORMAT format, u32 width,
    u32 height, float depth_clear_value, u8 stencil_clear_value,
    const std::wstring& name) {
  MY_ASSERTION(!util::Exist(created_depth_stencil_targets_, unique_id),
               L"unique_idが重複しています。");

  DepthStencil depth_stencil;
  if (!depth_stencil.Init(accessor, format, width, height,
                          {depth_clear_value, stencil_clear_value}, name)) {
    return false;
  }

  created_depth_stencil_targets_[unique_id] = depth_stencil;
  return true;
}

//レンダーターゲットをセットする
void RenderResourceManager::SetRenderTarget(RenderTargetID unique_id) {
  //実際にはIDを更新するだけ
  this->current_render_target_id_ = unique_id;
}

//デプス・ステンシルをセットする
void RenderResourceManager::SetDepthStencilTarget(u32 unique_id) {
  //実際にはIDを更新するだけ
  this->current_depth_stencil_target_id_ = unique_id;
}

//レンダーターゲットの情報をパイプラインディスクに書き込む
void RenderResourceManager::WriteRenderTargetInfoToPipelineDesc(
    IDirectXAccessor& accessor, RenderTargetID unique_id,
    shader::GraphicsPipelineState& pipeline) {
  if (unique_id == RenderTargetID::BACK_BUFFER) {
    swap_chain_.GetRenderTarget().WriteInfoToPipelineStateDesc(pipeline);
  } else {
    MY_ASSERTION(util::Exist(created_render_targets_, unique_id),
                 L"未登録のレンダーターゲットが選択されました。");
    created_render_targets_[unique_id].WriteInfoToPipelineDesc(&pipeline);
  }
}

//レンダーターゲットをシェーダーリソースとして利用する
void RenderResourceManager::UseRenderTargetToShaderResource(
    IDirectXAccessor& accessor, RenderTargetID unique_id,
    u32 render_target_number) {
  MY_ASSERTION(
      unique_id != RenderTargetID::BACK_BUFFER,
      L"SwapChainのレンダーターゲットはリソースとして使用できません。");

  created_render_targets_[unique_id].SetToGlobalHeap(accessor,
                                                     render_target_number);
}

//現在セットされているレンダーターゲットをクリアする
void RenderResourceManager::ClearCurrentRenderTarget(
    IDirectXAccessor& accessor) {
  if (current_render_target_id_ == RenderTargetID::BACK_BUFFER) {
    swap_chain_.ClearBackBuffer(accessor);
  } else {
    created_render_targets_[current_render_target_id_].ClearRenderTarget(
        accessor);
  }
}

//現在セットされているデプス・ステンシルをクリアする
void RenderResourceManager::ClearCurrentDepthStencilTarget(
    IDirectXAccessor& accessor) {
  created_depth_stencil_targets_[current_depth_stencil_target_id_]
      .ClearDepthStencil(accessor);
}

//レンダーターゲットをコマンドリストにセットする
void RenderResourceManager::SetRenderTargetsToCommandList(
    IDirectXAccessor& accessor) {
  if (current_render_target_id_ == RenderTargetID::BACK_BUFFER) {
    SetBackBufferToCommandList(accessor);
    return;
  }
  MY_ASSERTION(util::Exist(created_render_targets_, current_render_target_id_),
               L"未登録のレンダーターゲットIDが選択されました。");
  MY_ASSERTION(current_depth_stencil_target_id_ == UINT_LEAST32_MAX ||
                   util::Exist(created_depth_stencil_targets_,
                               current_depth_stencil_target_id_),
               L"未登録のデプス・ステンシルターゲットIDが選択されました。");

  created_render_targets_[current_render_target_id_].PrepareToUseRenderTarget(
      accessor);

  //レンダーターゲットのハンドルを取得する
  //ターゲットがスワップチェインならバックバッファのハンドルを取得する
  const std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> rtv_handles =
      created_render_targets_[current_render_target_id_].GetRTVHandles();
  const u32 render_target_num = static_cast<u32>(rtv_handles.size());

  if (current_depth_stencil_target_id_ == UINT_LEAST32_MAX) {
    accessor.GetCommandList()->OMSetRenderTargets(
        render_target_num, rtv_handles.data(), FALSE, nullptr);
    created_render_targets_[current_render_target_id_].SetViewport(accessor);
    created_render_targets_[current_render_target_id_].SetScissorRect(accessor);
  } else {
    D3D12_CPU_DESCRIPTOR_HANDLE dsv_handle =
        created_depth_stencil_targets_[current_depth_stencil_target_id_]
            .GetCPUHandle();
    accessor.GetCommandList()->OMSetRenderTargets(
        render_target_num, rtv_handles.data(), TRUE, &dsv_handle);
    created_render_targets_[current_render_target_id_].SetViewport(accessor);
    created_render_targets_[current_render_target_id_].SetScissorRect(accessor);
  }
}
void RenderResourceManager::DrawEnd(IDirectXAccessor& accessor) {
  if (current_render_target_id_ == RenderTargetID::BACK_BUFFER) {
    swap_chain_.DrawEnd(accessor);
  } else {
    created_render_targets_[current_render_target_id_].DrawEnd(accessor);
  }
}

bool RenderResourceManager::IsRegisteredRenderTargetID(
    RenderTargetID unique_id) const {
  return util::Exist(created_render_targets_, unique_id);
}

bool RenderResourceManager::Present() { return swap_chain_.Present(); }

u32 RenderResourceManager::GetCurrentFrameIndex() {
  swap_chain_.UpdateCurrentFrameIndex();
  return swap_chain_.GetCurrentFrameIndex();
}

void RenderResourceManager::SetBackBufferToCommandList(
    IDirectXAccessor& accessor) {
  swap_chain_.DrawBegin(accessor);

  D3D12_CPU_DESCRIPTOR_HANDLE rtv_handle[1] = {
      swap_chain_.GetRenderTarget().GetHandle().cpu_handle_};
  //デプス・ステンシルを使用しないときはnullをセットする
  if (current_depth_stencil_target_id_ == UINT_LEAST32_MAX) {
    accessor.GetCommandList()->OMSetRenderTargets(1, rtv_handle, FALSE,
                                                  nullptr);
  } else {
    MY_ASSERTION(util::Exist(created_depth_stencil_targets_,
                             current_depth_stencil_target_id_),
                 L"未登録のデプス・ステンシルが選択されました。");
    D3D12_CPU_DESCRIPTOR_HANDLE dsv_handle =
        created_depth_stencil_targets_[current_depth_stencil_target_id_]
            .GetCPUHandle();
    accessor.GetCommandList()->OMSetRenderTargets(1, rtv_handle, TRUE,
                                                  &dsv_handle);
  }
}

}  // namespace render_target
}  // namespace directx
}  // namespace legend

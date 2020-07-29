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
  if (!swap_chain_.Init(accessor, adapter, target_window, format, frame_count,
                        command_queue)) {
    return false;
  }

  this->current_render_target_id_ = RenderTargetID::BACK_BUFFER;
  this->current_depth_stencil_target_id_ = DepthStencilTargetID::None;
  return true;
}

//レンダーターゲットの作成
bool RenderResourceManager::CreateRenderTarget(IDirectXAccessor& accessor,
                                               RenderTargetID unique_id,
                                               DXGI_FORMAT format, u32 width,
                                               u32 height,
                                               const util::Color4& clear_color,
                                               const std::wstring& name) {
  MY_ASSERTION(!util::Exist(render_targets_, unique_id),
               L"unique_idが重複しています。");

  MultiRenderTargetTexture render_target;
  const MultiRenderTargetTexture::Info info{0,      format,      width,
                                            height, clear_color, L""};
  if (!render_target.Init(
          accessor, descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID,
          info)) {
    return false;
  }

  render_targets_.emplace(unique_id, render_target);
  return true;
}

//マルチレンダーターゲットの作成
bool RenderResourceManager::CreateRenderTargets(IDirectXAccessor& accessor,
                                                RenderTargetID unique_id,
                                                const std::vector<Info>& info) {
  MY_ASSERTION(!util::Exist(render_targets_, unique_id),
               L"unique_idが重複しています。");

  MultiRenderTargetTexture render_target;
  if (!render_target.Init(
          accessor, descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID,
          info)) {
    return false;
  }

  render_targets_.emplace(unique_id, render_target);
  return true;
}

//デプス・ステンシルを作成する
bool RenderResourceManager::CreateDepthStencil(
    IDirectXAccessor& accessor, DepthStencilTargetID unique_id,
    DXGI_FORMAT format, u32 width, u32 height, float depth_clear_value,
    u8 stencil_clear_value, const std::wstring& name) {
  MY_ASSERTION(!util::Exist(depth_stencil_targets_, unique_id),
               L"unique_idが重複しています。");

  DepthStencil depth_stencil;
  if (!depth_stencil.Init(accessor, format, width, height,
                          {depth_clear_value, stencil_clear_value}, name)) {
    return false;
  }

  depth_stencil_targets_.emplace(unique_id, depth_stencil);
  return true;
}

//レンダーターゲットをセットする
void RenderResourceManager::SetRenderTargetID(RenderTargetID unique_id) {
  this->current_render_target_id_ = unique_id;
}

//デプス・ステンシルをセットする
void RenderResourceManager::SetDepthStencilTargetID(
    DepthStencilTargetID unique_id) {
  this->current_depth_stencil_target_id_ = unique_id;
}

//レンダーターゲットの情報をパイプラインに書き込む
void RenderResourceManager::WriteRenderTargetInfoToPipeline(
    IDirectXAccessor& accessor, RenderTargetID unique_id,
    shader::GraphicsPipelineState* pipeline) {
  if (unique_id == RenderTargetID::BACK_BUFFER) {
    swap_chain_.GetRenderTarget().WriteInfoToPipelineState(pipeline);
  } else {
    MY_ASSERTION(util::Exist(render_targets_, unique_id),
                 L"未登録のレンダーターゲットが選択されました。");

    render_targets_.at(unique_id).WriteInfoToPipelineState(pipeline);
  }
}

//デプス・ステンシルの情報をパイプラインに書き込む
void RenderResourceManager::WriteDepthStencilTargetInfoToPipeline(
    IDirectXAccessor& accessor, DepthStencilTargetID unique_id,
    shader::GraphicsPipelineState* pipeline) {
  if (unique_id == DepthStencilTargetID::None) {
    pipeline->SetDSVFormat(DXGI_FORMAT::DXGI_FORMAT_UNKNOWN);
    CD3DX12_DEPTH_STENCIL_DESC sd(D3D12_DEFAULT);
    sd.DepthEnable = false;
    sd.StencilEnable = false;
    pipeline->SetDepthStencilState(sd);
  } else {
    MY_ASSERTION(util::Exist(depth_stencil_targets_, unique_id),
                 L"未登録のデプス・ステンシルが選択されました。");

    depth_stencil_targets_.at(unique_id).WriteInfoToPipelineState(pipeline);
  }
}

//レンダーターゲットをシェーダーリソースとして利用する
void RenderResourceManager::UseRenderTargetToShaderResource(
    IDirectXAccessor& accessor, RenderTargetID unique_id,
    u32 render_target_number) {
  MY_ASSERTION(
      unique_id != RenderTargetID::BACK_BUFFER,
      L"SwapChainのレンダーターゲットはリソースとして使用できません。");
  MY_ASSERTION(util::Exist(render_targets_, unique_id),
               L"未登録のレンダーターゲットが選択されました。");

  render_targets_.at(unique_id).SetToGlobalHeap(accessor, render_target_number);
}

//現在セットされているレンダーターゲットをクリアする
void RenderResourceManager::ClearCurrentRenderTarget(
    IDirectXAccessor& accessor) {
  if (current_render_target_id_ == RenderTargetID::BACK_BUFFER) {
    swap_chain_.ClearBackBuffer(accessor);
  } else {
    MY_ASSERTION(util::Exist(render_targets_, current_render_target_id_),
                 L"未登録のレンダーターゲットが選択されました。");

    render_targets_.at(current_render_target_id_).ClearRenderTarget(accessor);
  }
}

//現在セットされているデプス・ステンシルをクリアする
void RenderResourceManager::ClearCurrentDepthStencilTarget(
    IDirectXAccessor& accessor) {
  if (current_depth_stencil_target_id_ == DepthStencilTargetID::None) return;

  MY_ASSERTION(
      util::Exist(depth_stencil_targets_, current_depth_stencil_target_id_),
      L"未登録のデプス・ステンシルが選択されました。");

  depth_stencil_targets_.at(current_depth_stencil_target_id_)
      .ClearDepthStencil(accessor);
}

//レンダーターゲットをコマンドリストにセットする
void RenderResourceManager::SetRenderTargetsToCommandList(
    IDirectXAccessor& accessor) {
  MY_ASSERTION(current_render_target_id_ == RenderTargetID::BACK_BUFFER ||
                   util::Exist(render_targets_, current_render_target_id_),
               L"未登録のレンダーターゲットIDが選択されました。");

  const bool use_depth_stencil =
      current_depth_stencil_target_id_ != DepthStencilTargetID::None;

  if (current_render_target_id_ == RenderTargetID::BACK_BUFFER) {
    swap_chain_.DrawBegin(accessor);
  } else {
    render_targets_.at(current_render_target_id_).SetViewport(accessor);
    render_targets_.at(current_render_target_id_).SetScissorRect(accessor);
    render_targets_.at(current_render_target_id_)
        .PrepareToUseRenderTarget(accessor);
  }

  //レンダーターゲットのハンドルを取得する
  //ターゲットがスワップチェインならバックバッファのハンドルを取得する
  const std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> rtv_handles = [&]() {
    if (current_render_target_id_ == RenderTargetID::BACK_BUFFER) {
      return std::vector{swap_chain_.GetRenderTarget().GetHandle().cpu_handle_};
    }
    return render_targets_.at(current_render_target_id_).GetRTVHandles();
  }();

  const u32 render_target_num = static_cast<u32>(rtv_handles.size());

  //デプス・ステンシルを使用するとき
  if (use_depth_stencil) {
    MY_ASSERTION(
        util::Exist(depth_stencil_targets_, current_depth_stencil_target_id_),
        L"未登録のデプス・ステンシルターゲットIDが選択されました。");

    depth_stencil_targets_.at(current_depth_stencil_target_id_)
        .PrepareToSetCommandList(accessor);
    D3D12_CPU_DESCRIPTOR_HANDLE dsv_handle =
        depth_stencil_targets_.at(current_depth_stencil_target_id_)
            .GetCPUHandle();
    accessor.GetCommandList()->OMSetRenderTargets(
        render_target_num, rtv_handles.data(), TRUE, &dsv_handle);
  } else {
    accessor.GetCommandList()->OMSetRenderTargets(
        render_target_num, rtv_handles.data(), FALSE, nullptr);
  }
}

//描画終了
void RenderResourceManager::DrawEnd(IDirectXAccessor& accessor) {
  if (current_render_target_id_ == RenderTargetID::BACK_BUFFER) {
    swap_chain_.DrawEnd(accessor);
  } else {
    render_targets_.at(current_render_target_id_).DrawEnd(accessor);
  }
}

//レンダーターゲットIDが登録されているか
bool RenderResourceManager::IsRegisteredRenderTargetID(
    RenderTargetID unique_id) const {
  return util::Exist(render_targets_, unique_id);
}

//デプス・ステンシルIDが登録されているか
bool RenderResourceManager::IsRegisterdDepthStencilTargetID(
    DepthStencilTargetID unique_id) const {
  return util::Exist(depth_stencil_targets_, unique_id);
}

//バックバッファを表示する
bool RenderResourceManager::Present() { return swap_chain_.Present(); }

//フレームのインデックスを更新する
void RenderResourceManager::UpdateCurrentFrameIndex() {
  swap_chain_.UpdateCurrentFrameIndex();
}

//現在のフレームインデックスを取得する
u32 RenderResourceManager::GetCurrentFrameIndex() const {
  return swap_chain_.GetCurrentFrameIndex();
}

}  // namespace render_target
}  // namespace directx
}  // namespace legend

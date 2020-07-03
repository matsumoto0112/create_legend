#include "src/directx/buffer/render_target.h"

namespace legend {
namespace directx {
namespace buffer {

//コンストラクタ
RenderTarget::RenderTarget() {}

//デストラクタ
RenderTarget::~RenderTarget() {}

//初期化
bool RenderTarget::Init(IDirectXAccessor& accessor, DXGI_FORMAT format,
                        u32 width, u32 height, const util::Color4& clear_color,
                        const std::wstring& name) {
  CD3DX12_CLEAR_VALUE clear_value = {};
  clear_value.Format = format;
  memcpy(clear_value.Color, clear_color.Get().data(), sizeof(float) * 4);

  const CommittedResource::TextureBufferDesc desc(
      name, format, width, height,
      D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET,
      clear_value);
  if (!resource_.InitAsTex2D(accessor, desc)) {
    return false;
  }

  this->Transition(accessor,
                   D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COMMON);

  this->clear_color_ = clear_color;
  this->rtv_handle_ = accessor.GetHandle(DescriptorHeapType::RTV);
  this->format_ = format;

  D3D12_RENDER_TARGET_VIEW_DESC rtv_desc = {};
  rtv_desc.ViewDimension = D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_TEXTURE2D;
  rtv_desc.Format = format;
  accessor.GetDevice()->CreateRenderTargetView(
      resource_.GetResource(), &rtv_desc, rtv_handle_.cpu_handle_);

  return true;
}

//バッファから初期化する
bool RenderTarget::InitFromBuffer(IDirectXAccessor& accessor,
                                  ComPtr<ID3D12Resource> buffer,
                                  const util::Color4& clear_color,
                                  const std::wstring& name) {
  if (!resource_.InitFromBuffer(accessor, buffer, name)) {
    return false;
  }

  const D3D12_RESOURCE_DESC desc = resource_.GetResource()->GetDesc();
  D3D12_RENDER_TARGET_VIEW_DESC rtv_desc = {};
  rtv_desc.ViewDimension = D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_TEXTURE2D;
  rtv_desc.Format = desc.Format;

  this->clear_color_ = clear_color;
  this->rtv_handle_ = accessor.GetHandle(DescriptorHeapType::RTV);
  this->format_ = desc.Format;

  accessor.GetDevice()->CreateRenderTargetView(
      resource_.GetResource(), &rtv_desc, rtv_handle_.cpu_handle_);

  return true;
}

//デプス・ステンシルを作成する
bool RenderTarget::CreateDepthStencil(
    IDirectXAccessor& accessor, DXGI_FORMAT format, u32 width, u32 height,
    const DepthStencil::ClearValue& clear_value, const std::wstring& name) {
  depth_stencil_ = std::make_unique<DepthStencil>();
  if (!depth_stencil_->Init(accessor, format, width, height, clear_value,
                            name)) {
    return false;
  }

  return true;
}

//レンダーターゲットをセットする
void RenderTarget::SetRenderTarget(IDirectXAccessor& accessor) {
  this->Transition(accessor,
                   D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET);

  if (depth_stencil_) {
    depth_stencil_->PrepareToSetCommandList(accessor);
    accessor.GetCommandList()->OMSetRenderTargets(
        1, &rtv_handle_.cpu_handle_, TRUE, &depth_stencil_->GetCPUHandle());

  } else {
    accessor.GetCommandList()->OMSetRenderTargets(1, &rtv_handle_.cpu_handle_,
                                                  FALSE, nullptr);
  }
}

//レンダーターゲットをクリアする
void RenderTarget::ClearRenderTarget(IDirectXAccessor& accessor) const {
  accessor.GetCommandList()->ClearRenderTargetView(
      rtv_handle_.cpu_handle_, clear_color_.Get().data(), 0, nullptr);
  if (depth_stencil_) {
    depth_stencil_->ClearDepthStencil(accessor);
  }
}

//描画終了
void RenderTarget::DrawEnd(IDirectXAccessor& accessor) {
  this->Transition(accessor,
                   D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT);
}

//状態遷移
void RenderTarget::Transition(IDirectXAccessor& accessor,
                              D3D12_RESOURCE_STATES next_state) {
  resource_.Transition(accessor, next_state);
}

//パイプラインステートデスクに情報を書き込む
void RenderTarget::WriteInfoToPipelineStateDesc(
    D3D12_GRAPHICS_PIPELINE_STATE_DESC* pipeline_state_desc,
    bool write_with_depth_stencil) const {
  pipeline_state_desc->NumRenderTargets = 1;
  pipeline_state_desc->RTVFormats[0] = format_;

  if (write_with_depth_stencil) {
    if (depth_stencil_) {
      depth_stencil_->WriteInfoToPipelineStateDesc(pipeline_state_desc);
    } else {
      MY_LOG(L"書き込むデプス・ステンシルの情報が存在しません。");
    }
  }
}

}  // namespace buffer
}  // namespace directx
}  // namespace legend
#include "src/directx/render_target/render_target.h"

namespace legend {
namespace directx {
namespace render_target {

//�R���X�g���N�^
RenderTarget::RenderTarget() {}

//�f�X�g���N�^
RenderTarget::~RenderTarget() {}

//������
bool RenderTarget::Init(IDirectXAccessor& accessor, DXGI_FORMAT format,
                        u32 width, u32 height, const util::Color4& clear_color,
                        const std::wstring& name) {
  CD3DX12_CLEAR_VALUE clear_value = {};
  clear_value.Format = format;
  memcpy(clear_value.Color, clear_color.Get().data(), sizeof(float) * 4);

  const buffer::CommittedResource::TextureBufferDesc desc(
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

//�o�b�t�@���珉��������
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

//�����_�[�^�[�Q�b�g���N���A����
void RenderTarget::ClearRenderTarget(IDirectXAccessor& accessor) const {
  accessor.GetCommandList()->ClearRenderTargetView(
      rtv_handle_.cpu_handle_, clear_color_.Get().data(), 0, nullptr);
}

//�`��I��
void RenderTarget::DrawEnd(IDirectXAccessor& accessor) {
  this->Transition(accessor,
                   D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT);
}

//��ԑJ��
void RenderTarget::Transition(IDirectXAccessor& accessor,
                              D3D12_RESOURCE_STATES next_state) {
  resource_.Transition(accessor, next_state);
}

//�p�C�v���C���X�e�[�g�f�X�N�ɏ�����������
void RenderTarget::WriteInfoToPipelineStateDesc(
    D3D12_GRAPHICS_PIPELINE_STATE_DESC* pipeline_state_desc,
    bool write_with_depth_stencil) const {
  pipeline_state_desc->NumRenderTargets = 1;
  pipeline_state_desc->RTVFormats[0] = format_;
}

}  // namespace render_target
}  // namespace directx
}  // namespace legend
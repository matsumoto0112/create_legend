#include "src/directx/buffer/texture_2d.h"

#include "src/directx/directx_helper.h"
#include "src/directx/heap_manager.h"

namespace legend {
namespace directx {
namespace buffer {

//コンストラクタ
Texture2D::Texture2D() {}

//デストラクタ
Texture2D::~Texture2D() {}

//初期化
bool Texture2D::Init(DirectX12Device& device, u32 register_num,
                     DXGI_FORMAT format, u32 width, u32 height,
                     const std::wstring& name) {
  if (!texture_.InitAsTex2D(device, format, width, height, name)) {
    return false;
  }

  const u64 immediate_size =
      GetRequiredIntermediateSize(texture_.GetResource(), 0, 1);
  if (!texture_immediate_.InitAsBuffer(device, immediate_size,
                                       L"Immediate" + name)) {
    return false;
  }

  //パラメータの設定
  this->register_num_ = register_num;
  this->format_ = format;
  this->width_ = width;
  this->height_ = height;

  DescriptorHandle handle = device.GetHeapManager().GetLocalHandle();
  this->cpu_handle_ = handle.cpu_handle_;
  this->gpu_handle_ = handle.gpu_handle_;

  this->srv_view_.Texture2D.MipLevels = 1;
  this->srv_view_.Format = format;
  this->srv_view_.Shader4ComponentMapping =
      D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
  this->srv_view_.ViewDimension =
      D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2D;
  device.GetDevice()->CreateShaderResourceView(
      texture_.GetResource(), &this->srv_view_, this->cpu_handle_);

  return true;
}

//リソースを書き込む
void Texture2D::WriteResource(DirectX12Device& device, const void* data) {
  const u64 row = width_ * util::CalcPixelSizeFromFormat(format_);
  const u64 slice = row * height_;

  texture_.Transition(device,
                      D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST);

  CommittedResource::UpdateSubresource(device, &texture_, &texture_immediate_,
                                       data, row, slice);
  texture_.Transition(device,
                      D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);
}

void Texture2D::SetToHeap(DirectX12Device& device) {
  device.GetHeapManager().SetHandleToLocalHeap(register_num_, ResourceType::Srv,
                                          cpu_handle_);
}

}  // namespace buffer
}  // namespace directx
}  // namespace legend

#include "src/directx/buffer/texture_2d.h"

#include "src/directx/descriptor_heap/heap_manager.h"
#include "src/directx/directx_helper.h"
#include "src/libs/stb_image.h"
#include "src/util/loader/texture_loader.h"

namespace legend {
namespace directx {
namespace buffer {

//コンストラクタ
Texture2D::Texture2D() {}

//デストラクタ
Texture2D::~Texture2D() {}

//初期化
bool Texture2D::Init(DirectX12Device& device, const Desc& desc) {
  return InitTexBuffer(device, desc);
}

//初期化
bool Texture2D::InitAndWrite(DirectX12Device& device, u32 register_num,
                             const std::filesystem::path& filename,
                             const descriptor_heap::DescriptorHandle& handle) {
  //テクスチャを読み込む
  const util::loader::texture_loader::LoadedTextureData data =
      util::loader::texture_loader::Load(filename);

  //バッファを初期化する
  const Desc desc{register_num, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
                  data.width,   data.height,
                  handle,       data.name};
  if (!InitTexBuffer(device, desc)) {
    return false;
  }

  //テクスチャのピクセルデータを書き込む
  WriteResource(device, data.pixels.data());
  return true;
}

//初期化と書きこみ
bool Texture2D::InitAndWrite(DirectX12Device& device, u32 register_num,
                             DXGI_FORMAT format, const std::vector<u8>& data,
                             const descriptor_heap::DescriptorHandle& handle,
                             const std::wstring& name) {
  const util::loader::texture_loader::LoadedTextureData loaded_data =
      util::loader::texture_loader::LoadFromMemory(data);

  const Desc desc{register_num,       format, loaded_data.width,
                  loaded_data.height, handle, name};
  if (!Init(device, desc)) {
    return false;
  }
  WriteResource(device, loaded_data.pixels.data());
  return true;
}

//リソースを書き込む
void Texture2D::WriteResource(DirectX12Device& device, const void* data) {
  const u64 row = width_ * directx_helper::CalcPixelSizeFromFormat(format_);
  const u64 slice = row * height_;

  texture_.Transition(device,
                      D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST);

  CommittedResource::UpdateSubresource(device, &texture_, &texture_immediate_,
                                       data, row, slice);
  texture_.Transition(device,
                      D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);
}

//ヒープに追加する
void Texture2D::SetToHeap(DirectX12Device& device) {
  device.GetHeapManager().SetHandleToLocalHeap(register_num_, ResourceType::Srv,
                                               handle_.cpu_handle_);
}

//ヒープに追加する
void Texture2D::SetToHeap(DirectX12Device& device, u32 overwrite_register_num) {
  device.GetHeapManager().SetHandleToLocalHeap(
      overwrite_register_num, ResourceType::Srv, handle_.cpu_handle_);
}

//テクスチャバッファを初期化する
bool Texture2D::InitTexBuffer(DirectX12Device& device, const Desc& desc) {
  const CommittedResource::TextureBufferDesc buffer_desc{
      desc.name, desc.format, desc.width, desc.height};

  if (!texture_.InitAsTex2D(device, buffer_desc)) {
    return false;
  }

  const u64 immediate_size =
      GetRequiredIntermediateSize(texture_.GetResource(), 0, 1);
  if (!texture_immediate_.InitAsBuffer(device, immediate_size,
                                       L"Immediate" + desc.name)) {
    return false;
  }

  //パラメータの設定
  this->register_num_ = desc.register_num;
  this->format_ = desc.format;
  this->width_ = desc.width;
  this->height_ = desc.height;
  this->handle_ = desc.handle;

  D3D12_SHADER_RESOURCE_VIEW_DESC srv_view = {};
  srv_view.Texture2D.MipLevels = 1;
  srv_view.Format = desc.format;
  srv_view.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
  srv_view.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2D;
  device.GetDevice()->CreateShaderResourceView(texture_.GetResource(),
                                               &srv_view, handle_.cpu_handle_);

  return true;
}  // namespace buffer

}  // namespace buffer
}  // namespace directx
}  // namespace legend

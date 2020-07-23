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
bool Texture2D::Init(DirectX12Device& device, u32 register_num,
                     DXGI_FORMAT format, u32 width, u32 height,
                     descriptor_heap::DescriptorHandle handle,
                     const std::wstring& name) {
  return InitTexBuffer(device, register_num, format, width, height, handle,
                       name);
}

//初期化
bool Texture2D::InitAndWrite(DirectX12Device& device, u32 register_num,
                             const std::filesystem::path& filename,
                             descriptor_heap::DescriptorHandle handle) {
  const util::loader::texture_loader::LoadedTextureData data =
      util::loader::texture_loader::Load(filename);

  if (!InitTexBuffer(device, register_num,
                     DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, data.width,
                     data.height, handle, data.name)) {
    return false;
  }

  WriteResource(device, data.pixels.data());
  return true;
}

//初期化と書きこみ
bool Texture2D::InitAndWrite(DirectX12Device& device, u32 register_num,
                             const std::vector<u8>& data,
                             descriptor_heap::DescriptorHandle handle,
                             const std::wstring& name) {
  const util::loader::texture_loader::LoadedTextureData loaded_data =
      util::loader::texture_loader::LoadFromMemory(data);

  if (!Init(device, 0, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
            loaded_data.width, loaded_data.height, handle, name)) {
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
bool Texture2D::InitTexBuffer(DirectX12Device& device, u32 register_num,
                              DXGI_FORMAT format, u32 width, u32 height,
                              descriptor_heap::DescriptorHandle handle,
                              const std::wstring& name) {
  const CommittedResource::TextureBufferDesc desc =
      CommittedResource::TextureBufferDesc(name, format, width, height);

  if (!texture_.InitAsTex2D(device, desc)) {
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
  this->handle_ = handle;

  D3D12_SHADER_RESOURCE_VIEW_DESC srv_view = {};
  srv_view.Texture2D.MipLevels = 1;
  srv_view.Format = format;
  srv_view.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
  srv_view.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2D;
  device.GetDevice()->CreateShaderResourceView(texture_.GetResource(),
                                               &srv_view, handle_.cpu_handle_);

  return true;
}  // namespace buffer

}  // namespace buffer
}  // namespace directx
}  // namespace legend

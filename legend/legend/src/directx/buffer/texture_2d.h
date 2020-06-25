#ifndef LEGEND_DIRECTX_BUFFER_TEXTURE_2D_H_
#define LEGEND_DIRECTX_BUFFER_TEXTURE_2D_H_

/**
 * @file texture_2d.h
 * @brief 2Dテクスチャクラス定義
 */

#include "src/directx/buffer/committed_resource.h"
#include "src/directx/directx12_device.h"

namespace legend {
namespace directx {
namespace buffer {
class Texture2D {
 public:
  Texture2D();
  ~Texture2D();
  bool Init(DirectX12Device& device, DXGI_FORMAT format, u32 width, u32 height,
            D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle,
            D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle, const std::wstring& name);
  void WriteResource(DirectX12Device& device, const void* data);
  void SetGraphicsCommandList(DirectX12Device& device, u32 index);

 private:
  buffer::CommittedResource texture_;
  buffer::CommittedResource texture_immediate_;
  DXGI_FORMAT format_;
  u32 width_;
  u32 height_;
  D3D12_SHADER_RESOURCE_VIEW_DESC srv_view_;
  D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle_;
  D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle_;
};

}  // namespace buffer
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_BUFFER_TEXTURE_2D_H_

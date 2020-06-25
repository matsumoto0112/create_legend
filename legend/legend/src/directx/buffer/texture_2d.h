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
/**
 * @brief 2Dテクスチャクラス
 */
class Texture2D {
 public:
  /**
   * @brief コンストラクタ
   */
  Texture2D();
  /**
   * @brief デストラクタ
   */
  ~Texture2D();
  /**
   * @brief 初期化
   * @param device DirectX12デバイス
   * @param format テクスチャのフォーマット
   * @param width テクスチャの幅
   * @param height テクスチャの高さ
   * @param cpu_handle 使用するCPUハンドル
   * @param gpu_handle 使用するGPUハンドル
   * @param name リソース名
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(DirectX12Device& device, DXGI_FORMAT format, u32 width, u32 height,
            D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle,
            D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle, const std::wstring& name);
  /**
   * @brief テクスチャデータを書き込む
   * @param device DirectX12デバイス
   * @param data テクスチャデータ
   */
  void WriteResource(DirectX12Device& device, const void* data);
  /**
   * @brief CPUハンドルを取得する
   */
  D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle() const { return cpu_handle_; }
  /**
   * @brief GPUハンドルを取得する
   */
  D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle() const { return gpu_handle_; }

 private:
  //! テクスチャデータ
  buffer::CommittedResource texture_;
  //! コピー用のテクスチャリソース
  buffer::CommittedResource texture_immediate_;
  //! テクスチャのフォーマット
  DXGI_FORMAT format_;
  //! テクスチャの幅
  u32 width_;
  //! テクスチャの高さ
  u32 height_;
  //! シェーダーリソースビューデスク
  D3D12_SHADER_RESOURCE_VIEW_DESC srv_view_;
  //! CPUハンドル
  D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle_;
  //! GPUハンドル
  D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle_;
};

}  // namespace buffer
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_BUFFER_TEXTURE_2D_H_

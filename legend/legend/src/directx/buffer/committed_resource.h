#ifndef LEGEND_DIRECTX_BUFFER_BUFFER_H_
#define LEGEND_DIRECTX_BUFFER_BUFFER_H_

/**
 * @file buffer.h
 * @brief バッファ管理クラス定義
 */

#include "src/directx/directx12_device.h"

namespace legend {
namespace directx {
namespace buffer {
/**
 * @brief CreateCommittedResourceで作成されるリソース管理クラス
 */
class CommittedResource {
 public:
  /**
   * @brief コンストラクタ
   */
  CommittedResource();
  /**
   * @brief デストラクタ
   */
  ~CommittedResource();
  /**
   * @brief バッファとして初期化する
   * @param device DirectX12デバイス
   * @param buffer_size バッファのメモリサイズ
   * @param name リソース名
   * @return 初期化に成功したらtrueを返す
   */
  bool InitAsBuffer(DirectX12Device& device, u64 buffer_size,
                    const std::wstring& name);
  bool InitAsTex2D(DirectX12Device& device, DXGI_FORMAT format, u32 width,
                   u32 height, const std::wstring& name);

  void Transition(DirectX12Device& device, D3D12_RESOURCE_STATES next_state);
  /**
   * @brief リソースを取得する
   */
  ID3D12Resource* GetResource() const { return resource_.Get(); }
  /**
   * @brief リソースにデータを書き込む
   * @param data 書き込むデータ
   * @return 書き込みに成功したらtrueを返す
   */
  bool WriteResource(const void* data);

  u64 GetBufferSize() const { return buffer_size_; }

 public:
  static void UpdateSubresource(DirectX12Device& device,
                                CommittedResource* dest_resource,
                                CommittedResource* immediate_resource,
                                const void* data, u64 row, u64 slice);

 private:
  //! 管理しているリソース
  ComPtr<ID3D12Resource> resource_;
  //! バッファのメモリサイズ
  u64 buffer_size_;
  //! リソースの状態
  D3D12_RESOURCE_STATES current_state_;
};

}  // namespace buffer
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_BUFFER_BUFFER_H_

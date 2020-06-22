#ifndef LEGEND_DIRECTX_BUFFER_VERTEX_BUFFER_H_
#define LEGEND_DIRECTX_BUFFER_VERTEX_BUFFER_H_

/**
 * @file vertex_buffer_h
 * @brief 頂点バッファ定義
 */

#include "src/directx/directx12_device.h"

namespace legend {
namespace directx {
namespace buffer {
/**
 * @brief 頂点バッファ
 */
class VertexBuffer {
 public:
  /**
   * @brief コンストラクタ
   */
  VertexBuffer();
  /**
   * @brief デストラクタ
   */
  ~VertexBuffer();
  /**
   * @brief 初期化
   * @param device デバイス
   * @param vertex_size 頂点構造体のメモリサイズ
   * @param vertex_num 頂点数
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(DirectX12Device& device, u32 vertex_size, u32 vertex_num);
  /**
   * @brief リソースにデータを書き込む書き込む
   * @param vertices 書き込む頂点データ
   * @return 書き込みに成功したらtrueを返す
   */
  template <class T>
  bool WriteBufferResource(const std::vector<T>& vertices);
  /**
   * @brief コマンドリストにセットする
   */
  void SetGraphicsCommandList(DirectX12Device& device);

 protected:
  //! 頂点リソース
  ComPtr<ID3D12Resource> vertex_buffer_resource_;
  //! 頂点バッファビュー
  D3D12_VERTEX_BUFFER_VIEW vertex_buffer_view_;
};

//リソースに書き込む
template <class T>
inline bool VertexBuffer::WriteBufferResource(const std::vector<T>& vertices) {
  util::debug::Assertion(
      vertices.size() * sizeof(T) == vertex_buffer_view_.SizeInBytes,
      L"vertices size is incorrect.");

  void* data_begin;
  if (FAILED(vertex_buffer_resource_->Map(0, nullptr, &data_begin))) {
    return false;
  }

  memcpy_s(data_begin, vertex_buffer_view_.SizeInBytes, vertices.data(),
           vertex_buffer_view_.SizeInBytes);
  vertex_buffer_resource_->Unmap(0, nullptr);
  return true;
}

}  // namespace buffer
}  // namespace directx
}  // namespace legend

#endif  // !LEGEND_DIRECTX_BUFFER_VERTEX_BUFFER_H_

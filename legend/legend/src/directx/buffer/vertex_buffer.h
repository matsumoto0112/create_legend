#ifndef LEGEND_DIRECTX_BUFFER_VERTEX_BUFFER_H_
#define LEGEND_DIRECTX_BUFFER_VERTEX_BUFFER_H_

/**
 * @file vertex_buffer_h
 * @brief 頂点バッファ定義
 */

#include "src/directx/buffer/committed_resource.h"
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
   * @param name リソース名
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(DirectX12Device& device, u32 vertex_size, u32 vertex_num,
            const std::wstring& name);
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
  CommittedResource resource_;
  //! 頂点バッファビュー
  D3D12_VERTEX_BUFFER_VIEW vertex_buffer_view_;
};

//リソースに書き込む
template <class T>
inline bool VertexBuffer::WriteBufferResource(const std::vector<T>& vertices) {
  MY_ASSERTION(vertices.size() * sizeof(T) == vertex_buffer_view_.SizeInBytes,
               L"vertices size is incorrect.");

  return resource_.WriteResource(vertices.data());
}

}  // namespace buffer
}  // namespace directx
}  // namespace legend

#endif  // !LEGEND_DIRECTX_BUFFER_VERTEX_BUFFER_H_

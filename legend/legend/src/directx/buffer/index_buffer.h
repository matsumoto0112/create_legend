#ifndef LEGEND_DIRECTX_BUFFER_INDEX_BUFFER_H_
#define LEGEND_DIRECTX_BUFFER_INDEX_BUFFER_H_

/**
 * @file index_buffer.h
 * @brief インデックスバッファ定義
 */

#include "src/directx/buffer/committed_resource.h"
#include "src/directx/directx12_device.h"
#include "src/directx/primitive_topology.h"

namespace legend {
namespace directx {
namespace buffer {

/**
 * @brief インデックスバッファクラス
 */
class IndexBuffer {
  using Index = u16;

 public:
  /**
   * @brief コンストラクタ
   */
  IndexBuffer();
  /**
   * @brief デストラクタ
   */
  ~IndexBuffer();
  /**
   * @brief 初期化
   * @param device DirectX12デバイス
   * @param index_num インデックス数
   * @param topology プリミティブの形状
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(DirectX12Device& device, u32 index_num, PrimitiveTopology topology);
  /**
   * @brief バッファにリソースを書き込む
   * @param indices インデックス配列
   * @return 書き込みに成功したらtrueを返す
   */
  bool WriteBufferResource(const std::vector<Index>& indices);
  /**
   * @brief グラフィックスコマンドリストに追加する
   * @param device DirectX12デバイス
   */
  void SetGraphicsCommandList(DirectX12Device& device);
  /**
   * @brief 描画
   * @param device DirectX12デバイス
   */
  void Draw(DirectX12Device& device);

 private:
  //! インデックスバッファのリソースデータ
  CommittedResource resource_;
  //! バッファビュー
  D3D12_INDEX_BUFFER_VIEW index_buffer_view_;
  //! インデックス数
  u32 index_num_;
  //! プリミティブ形状
  D3D12_PRIMITIVE_TOPOLOGY primitive_toporogy_;
};
}  // namespace buffer
}  // namespace directx
}  // namespace legend
#endif  //! LEGEND_DIRECTX_BUFFER_INDEX_BUFFER_H_

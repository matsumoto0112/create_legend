#ifndef LEGEND_DIRECTX_BUFFER_INDEX_BUFFER_H_
#define LEGEND_DIRECTX_BUFFER_INDEX_BUFFER_H_

/**
 * @file index_buffer.h
 * @brief インデックスバッファ定義
 */

#include "src/directx/buffer/committed_resource.h"
#include "src/directx/directx_accessor.h"
#include "src/directx/primitive_topology.h"

namespace legend {
namespace directx {
namespace buffer {

/**
 * @class IndexBuffer
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
   * @param accessor DirectX12デバイスアクセサ
   * @param index_num インデックス数
   * @param topology プリミティブの形状
   * @param name リソース名
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(IDirectXAccessor& accessor, u32 index_num,
            PrimitiveTopology topology, const std::wstring& name);
  /**
   * @brief 初期化と書き込みを同時にする
   * @param accessor DirectX12デバイスアクセサ
   * @param indices インデックス配列
   * @param topology プリミティブの形状
   * @param name リソース名
   * @return 初期化、書き込みに成功したらtrueを返す
   */
  bool InitAndWrite(IDirectXAccessor& accessor,
                    const std::vector<Index>& indices,
                    PrimitiveTopology topology, const std::wstring& name);
  /**
   * @brief バッファにリソースを書き込む
   * @param indices インデックス配列
   * @return 書き込みに成功したらtrueを返す
   */
  bool WriteBufferResource(const std::vector<Index>& indices);
  /**
   * @brief グラフィックスコマンドリストに追加する
   * @param accessor DirectX12デバイスアクセサ
   */
  void SetGraphicsCommandList(IDirectXAccessor& accessor);
  /**
   * @brief 描画
   * @param accessor DirectX12デバイスアクセサ
   */
  void Draw(IDirectXAccessor& accessor);

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

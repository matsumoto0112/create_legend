#ifndef LEGEND_DIRECTX_BUFFER_VERTEX_BUFFER_H_
#define LEGEND_DIRECTX_BUFFER_VERTEX_BUFFER_H_

/**
 * @file vertex_buffer_h
 * @brief 頂点バッファ定義
 */

#include "src/directx/buffer/committed_resource.h"
#include "src/directx/device/command_list.h"
#include "src/directx/device/directx_accessor.h"

namespace legend {
namespace directx {
namespace buffer {

/**
 * @class VertexBuffer
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
   * @param accessor DirectXデバイスアクセサ
   * @param vertex_size 頂点構造体の大きさ
   * @param vertex_num 頂点数
   * @param name リソース名
   */
  bool Init(device::IDirectXAccessor& accessor, u32 vertex_size, u32 vertex_num,
            const std::wstring& name);
  /**
   * @brief リソースにデータを書き込む
   * @param data 書き込むデータ
   */
  bool WriteBufferResource(const void* data);
  /**
   * @brief コマンドリストにセットする
   * @param command_list コマンドリスト
   */
  void SetGraphicsCommandList(device::CommandList& command_list);

 protected:
  //! 頂点リソース
  CommittedResource resource_;
  //! 頂点バッファビュー
  D3D12_VERTEX_BUFFER_VIEW vertex_buffer_view_;
};

}  // namespace buffer
}  // namespace directx
}  // namespace legend

#endif  // !LEGEND_DIRECTX_BUFFER_VERTEX_BUFFER_H_

#ifndef LEGEND_DIRECTX_RENDER_TARGET_DEPTH_STENCIL_H_
#define LEGEND_DIRECTX_RENDER_TARGET_DEPTH_STENCIL_H_

/**
 * @file depth_stencil.h
 * @brief デプス・ステンシルクラス定義
 */

#include "src/directx/buffer/committed_resource.h"
#include "src/directx/descriptor_heap/descriptor_handle.h"
#include "src/directx/directx_accessor.h"

namespace legend {
namespace directx {
namespace buffer {
/**
 * @class DepthStencil
 * @brief デプス・ステンシルクラス
 */
class DepthStencil {
 public:
  /**
   * @struct ClearValue
   * @brief デプス・ステンシルのクリア値
   */
  struct ClearValue {
    //! 深度値
    float depth;
    //! ステンシル値
    u8 stencil;

    /**
     * @brief コンストラクタ
     * @param depth 深度値
     * @param stencil ステンシル値
     */
    ClearValue(float depth = 1.0f, u8 stencil = 0)
        : depth(depth), stencil(stencil) {}
  };

 public:
  /**
   * @brief コンストラクタ
   */
  DepthStencil();
  /**
   * @brief デストラクタ
   */
  ~DepthStencil();
  /**
   * @brief 初期化
   * @param accessor DirextX12デバイスアクセサ
   * @param format フォーマット
   * @param width 幅
   * @param height 高さ
   * @param clear_value クリア値
   * @param name リソース名
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(IDirectXAccessor& accessor, DXGI_FORMAT format, u32 width,
            u32 height, const ClearValue& clear_value,
            const std::wstring& name);
  /**
   * @brief デプス・ステンシル値のクリア
   * @param accessor DirextX12デバイスアクセサ
   */
  void ClearDepthStencil(IDirectXAccessor& accessor) const;
  /**
   * @brief コマンドリストにセットする準備をする
   * @param accessor DirextX12デバイスアクセサ
   * @details RTVセットと同時にDSVもセットするため、そのセット前に呼ぶ必要がある
   */
  void PrepareToSetCommandList(IDirectXAccessor& accessor);
  /**
   * @brief パイプラインステートデスクにRTV情報を書き込む
   * @param pipeline_state_desc 書き込む対象
   */
  void WriteInfoToPipelineStateDesc(
      D3D12_GRAPHICS_PIPELINE_STATE_DESC* pipeline_state_desc) const;

  /**
   * @brief CPUハンドルを取得する
   * @details RTVのセット時に同時にDSVもセットするため、RTV向けに公開
   */
  D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle() const {
    return handle_.cpu_handle_;
  }

 private:
  //! リソース
  legend::directx::buffer::CommittedResource resource_;
  //! ハンドル
  legend::directx::DescriptorHandle handle_;
  //! フォーマット
  DXGI_FORMAT format_;
  //! クリア値
  ClearValue clear_value_;
};

}  // namespace buffer
}  // namespace directx
}  // namespace legend

#endif //! LEGEND_DIRECTX_RENDER_TARGET_DEPTH_STENCIL_H_

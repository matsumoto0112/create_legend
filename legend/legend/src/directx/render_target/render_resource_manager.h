#ifndef LEGEND_DIRECTX_RENDER_TARGET_RENDER_RESOURCE_MANAGER_H_
#define LEGEND_DIRECTX_RENDER_TARGET_RENDER_RESOURCE_MANAGER_H_

/**
 * @file render_resource_manager.h
 * @brief レンダーターゲットリソース管理クラス定義
 */

#include "src/directx/device/dxgi_adapter.h"
#include "src/directx/device/swap_chain.h"
#include "src/directx/render_target/depth_stencil.h"
#include "src/directx/render_target/multi_render_target_texture.h"
#include "src/directx/render_target/render_target_id.h"

namespace legend {
namespace directx {
namespace render_target {

/**
 * @class RenderResourceManager
 * @brief レンダーターゲットリソース管理クラス
 */
class RenderResourceManager {
 private:
  /**
   * @brief 複数フレーム分のターゲット管理クラス
   * @tparam T 管理するターゲットの型
   */
  template <class T>
  struct MultiframeTarget {
    /**
     * @brief コンストラクタ
     * @param frame_count フレーム数
     * @param swap_chain スワップチェイン
     */
    inline MultiframeTarget(u32 frame_count, device::SwapChain& swap_chain)
        : swap_chain(swap_chain) {
      this->targets.resize(frame_count);
    }
    /**
     * @brief 現在フレームのターゲットを取得する
     */
    inline T& Get() { return targets[swap_chain.GetCurrentBackBufferIndex()]; }
    /**
     * @brief 現在フレームのターゲットを取得する
     */
    inline const T& Get() const {
      return targets[swap_chain.GetCurrentBackBufferIndex()];
    }
    /**
     * @brief indexからターゲットを取得する
     */
    inline T& Get(u32 index) { return targets[index]; }

   private:
    //! スワップチェイン
    device::SwapChain& swap_chain;
    //! フレーム数分のターゲット
    std::vector<T> targets;
  };

  //! マルチレンダーターゲット
  using MultiFrameMultiRenderTargetTexture =
      MultiframeTarget<MultiRenderTargetTexture>;
  //! デプス・ステンシル
  using MultiFrameDepthStencil = MultiframeTarget<DepthStencil>;
  using RenderTargetMap =
      std::unordered_map<RenderTargetID, MultiFrameMultiRenderTargetTexture>;
  using DepthStencilTargetMap =
      std::unordered_map<DepthStencilTargetID, MultiFrameDepthStencil>;

 public:
  using Info = MultiRenderTargetTexture::Info;

 public:
  /**
   * @brief コンストラクタ
   */
  RenderResourceManager();
  /**
   * @brief デストラクタ
   */
  ~RenderResourceManager();
  /**
   * @brief 初期化
   * @param accessor DirectXデバイスアクセサ
   * @param adapter アダプター
   * @param frame_count フレーム数
   * @param width 幅
   * @param height 高さ
   * @param format フォーマット
   * @param hwnd ウィンドウハンドル
   * @param command_queue コマンドキュー
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(device::IDirectXAccessor& accessor, device::DXGIAdapter& adapter,
            u32 frame_count, u32 width, u32 height, DXGI_FORMAT format,
            HWND hwnd, ID3D12CommandQueue* command_queue);
  /**
   * @brief レンダーターゲットをセットする
   * @param command_list コマンドリスト
   * @param render_target_id レンダーターゲットID
   * @param clear_render_target レンダーターゲットをクリアするか
   * @param depth_stencil_target_id デプス・ステンシルID
   * @param clear_depth_stencil_target デプス・ステンシルをクリアするか
   */
  void SetRenderTargets(device::CommandList& command_list,
                        RenderTargetID render_target_id,
                        bool clear_render_target,
                        DepthStencilTargetID depth_stencil_target_id,
                        bool clear_depth_stencil_target);
  /**
   * @brief バックバッファのインデックスを取得する
   */
  u32 GetCurrentBackBufferIndex() const;
  /**
   * @brief 描画終了処理
   * @param command_list コマンドリスト
   */
  void DrawEnd(device::CommandList& command_list);
  /**
   * @brief 描画内容を表示する
   * @return 成功したらtrueを返す
   */
  bool Present();
  /**
   * @brief レンダーターゲットを追加する
   * @param id レンダーターゲットID
   * @param accessor DirectXデバイスアクセサ
   * @param infos レンダーターゲット情報
   * @return 追加に成功したらtrueを返す
   */
  bool AddRenderTarget(
      RenderTargetID id, device::IDirectXAccessor& accessor,
      const std::vector<MultiRenderTargetTexture::Info>& infos);
  /**
   * @brief デプス・ステンシルを追加する
   * @param id レンダーターゲットID
   * @param accessor DirectXデバイスアクセサ
   * @param desc デプス・ステンシルデスク
   * @return 追加に成功したらtrueを返す
   */
  bool AddDepthStencil(DepthStencilTargetID id,
                       device::IDirectXAccessor& accessor,
                       const DepthStencil::DepthStencilDesc& desc);
  /**
   * @brief シェーダーリソースとして使用する
   * @param accessor DirectXデバイスアクセサ
   * @param id レンダーターゲットID
   * @param render_target_index 使用するマルチレンダーターゲットのインデックス
   */
  void UseAsSRV(device::IDirectXAccessor& accessor, RenderTargetID id,
                u32 render_target_index);

 private:
  //! フレーム数
  u32 frame_count_;
  //! スワップチェイン
  device::SwapChain swap_chain_;
  //! レンダーターゲットマップ
  RenderTargetMap render_targets_;
  //! デプス・ステンシルマップ
  DepthStencilTargetMap depth_stencil_targets_;
};

}  // namespace render_target
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_RENDER_TARGET_RENDER_RESOURCE_MANAGER_H_

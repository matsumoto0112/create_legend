#ifndef LEGEND_DIRECTX_RENDER_TARGET_RENDER_RESOURCE_MANAGER_H_
#define LEGEND_DIRECTX_RENDER_TARGET_RENDER_RESOURCE_MANAGER_H_

/**
 * @file render_resource_manager.h
 * @brief レンダーターゲットリソース管理クラス定義
 */

#include "src/directx/device/dxgi_adapter.h"
#include "src/directx/device/swap_chain.h"
#include "src/directx/directx_accessor.h"
#include "src/directx/render_target/depth_stencil.h"
#include "src/directx/render_target/depth_stencil_target_id.h"
#include "src/directx/render_target/multi_render_target_texture.h"
#include "src/directx/render_target/render_target_id.h"
#include "src/window/window.h"

namespace legend {
namespace directx {
namespace render_target {

/**
 * @class RenderResourceManager
 * @brief レンダーターゲットリソース管理クラス
 */
class RenderResourceManager {
 private:
  using RenderTargetMap =
      std::unordered_map<RenderTargetID, MultiRenderTargetTexture>;
  using DepthStencilTargetMap =
      std::unordered_map<DepthStencilTargetID, DepthStencil>;

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
   * @param accessor DirectX12デバイスアクセサ
   * @param adapter アダプター
   * @param target_window 描画対象のウィンドウ
   * @param frame_count バックバッファ数
   * @param format バックバッファのフォーマット
   * @param command_queue スワップチェイン
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(IDirectXAccessor& accessor, device::DXGIAdapter& adapter,
            window::Window& target_window, u32 frame_count, DXGI_FORMAT format,
            ID3D12CommandQueue* command_queue);
  /**
   * @brief レンダーターゲットを作成する
   * @param accessor DirectX12デバイスアクセサ
   * @param unique_id レンダーターゲットID
   * @param format フォーマット
   * @param width 幅
   * @param height 高さ
   * @param clear_color クリア色
   * @param name レンダーターゲット名
   * @return 作成に成功したらtrueを返す
   */
  bool CreateRenderTarget(IDirectXAccessor& accessor, RenderTargetID unique_id,
                          DXGI_FORMAT format, u32 width, u32 height,
                          const util::Color4& clear_color,
                          const std::wstring& name);
  /**
   * @brief マルチレンダーターゲットを作成する
   * @param accessor DirectX12デバイスアクセサ
   * @param unique_id レンダーターゲットID
   * @param info それぞれのレンダーターゲットの情報
   * @return 作成に成功したらtrueを返す
   */
  bool CreateRenderTargets(IDirectXAccessor& accessor, RenderTargetID unique_id,
                           const std::vector<Info>& info);
  /**
   * @brief デプス・ステンシルを作成する
   * @param accessor DirectX12デバイスアクセサ
   * @param unique_id デプス・ステンシルID
   * @param format フォーマット
   * @param width 幅
   * @param height 高さ
   * @param depth_clear_value デプス値のクリア値
   * @param stencil_clear_value ステンシル値のクリア値k
   * @param name デプス・ステンシル名
   * @return 作成に成功したらtrueを返す
   */
  bool CreateDepthStencil(IDirectXAccessor& accessor,
                          DepthStencilTargetID unique_id, DXGI_FORMAT format,
                          u32 width, u32 height, float depth_clear_value,
                          u8 stencil_clear_value, const std::wstring& name);
  /**
   * @brief 対象となるレンダーターゲットを切り替える
   コマンドリストにはセットしない
   */
  void SetRenderTargetID(RenderTargetID unique_id);
  /**
   * @brief 対象となるデプス・ステンシルを切り替える
    コマンドリストにはセットしない
  */
  void SetDepthStencilTargetID(DepthStencilTargetID unique_id);
  /**
   * @brief レンダーターゲットの情報をパイプラインに書き込む
   * @param accessor DirectX12デバイスアクセサ
   * @param unique_id レンダーターゲットID
   * @param pipeline 書き込む対象のパイプライン
   */
  void WriteRenderTargetInfoToPipeline(IDirectXAccessor& accessor,
                                       RenderTargetID unique_id,
                                       shader::GraphicsPipelineState* pipeline);
  /**
   * @brief デプス・ステンシルの情報をパイプラインに書き込む
   * @param accessor DirectX12デバイスアクセサ
   * @param unique_id デプス・ステンシルID
   * @param pipeline 書き込む対象のパイプライン
   */
  void WriteDepthStencilTargetInfoToPipeline(
      IDirectXAccessor& accessor, DepthStencilTargetID unique_id,
      shader::GraphicsPipelineState* pipeline);
  /**
   * @brief レンダーターゲットをシェーダーリソースとして利用する
   * @param accessor DirectX12デバイスアクセサ
   * @param unique_id レンダーターゲットID
   * @param render_target_number
   * 使用したいレンダーターゲットの番号
   */
  void UseRenderTargetToShaderResource(IDirectXAccessor& accessor,
                                       RenderTargetID unique_id,
                                       u32 render_target_number = 0);
  /**
   * @brief 現在対象となっているレンダーターゲットをクリアする
   * @param accessor DirectX12デバイスアクセサ
   */
  void ClearCurrentRenderTarget(IDirectXAccessor& accessor);
  /**
   * @brief 現在対象となっているデプス・ステンシルをクリアする
   * @param accessor DirectX12デバイスアクセサ
   */
  void ClearCurrentDepthStencilTarget(IDirectXAccessor& accessor);
  /**
   * @brief 現在対象となっているレンダーターゲットをコマンドリストにセットする
   * @param accessor DirectX12デバイスアクセサ
   */
  void SetRenderTargetsToCommandList(IDirectXAccessor& accessor);
  /**
   * @brief 描画を終了する
   * @param accessor
   */
  void DrawEnd(IDirectXAccessor& accessor);
  /**
   * @brief 引数のIDがすでに登録されているか
   * @param unique_id 判定するID
   * @return 登録されていたらtrueを返す
   */
  bool IsRegisteredRenderTargetID(RenderTargetID unique_id) const;
  /**
   * @brief 引数のIDがすでに登録されているか
   * @param unique_id 判定するID
   * @return 登録されていたらtrueを返す
   */
  bool IsRegisterdDepthStencilTargetID(DepthStencilTargetID unique_id) const;
  /**
   * @brief バックバッファを画面に表示する
   * @return 表示に成功したらtrueを返す
   */
  bool Present();
  /**
   * @brief 現在フレームのインデックスを更新する
   */
  void UpdateCurrentFrameIndex();
  /**
   * @brief 現在フレームのインデックスを取得する
   */
  u32 GetCurrentFrameIndex() const;

 private:
  //! スワップチェイン
  device::SwapChain swap_chain_;
  //! レンダーターゲットマップ
  RenderTargetMap render_targets_;
  //! 現在対象となっているレンダーターゲットID
  RenderTargetID current_render_target_id_;
  //! デプス・ステンシルマップ
  DepthStencilTargetMap depth_stencil_targets_;
  //! 現在対象となっているデプス・ステンシルID
  DepthStencilTargetID current_depth_stencil_target_id_;
};

}  // namespace render_target
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_RENDER_TARGET_RENDER_RESOURCE_MANAGER_H_

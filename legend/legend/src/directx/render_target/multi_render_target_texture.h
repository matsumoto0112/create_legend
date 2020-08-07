#ifndef LEGEND_DIRECTX_RENDER_TARGET_MULTI_RENDER_TARGET_TEXTURE_H_
#define LEGEND_DIRECTX_RENDER_TARGET_MULTI_RENDER_TARGET_TEXTURE_H_

/**
 * @file multi_render_target_texture.h
 * @brief テクスチャ使用可能なレンダーターゲットクラス定義
 */

#include "src/directx/descriptor_heap/heap_parameter.h"
#include "src/directx/device/command_list.h"
#include "src/directx/render_target/render_target.h"

namespace legend {
namespace directx {
namespace render_target {

/**
 * @class MultiRenderTargetTexture
 * @brief テクスチャ使用可能なレンダーターゲット
 */
class MultiRenderTargetTexture {
 private:
  /**
   * @struct RenderTargetTexture
   * @brief レンダーターゲットテクスチャ構造体
   */
  struct RenderTargetTexture {
    //! シェーダーのレジスター番号
    u32 register_num;
    //! レンダーターゲット
    RenderTarget render_target;
    //! SRVハンドル
    descriptor_heap::DescriptorHandle srv_handle;
  };

 public:
  /**
   * @struct Info
   * @brief レンダーターゲットの情報
   */
  struct Info {
    //! シェーダーのレジスター番号
    u32 register_num;
    //! フォーマット
    DXGI_FORMAT format;
    //! 幅
    u32 width;
    //! 高さ
    u32 height;
    //! クリア色
    util::Color4 clear_color;
    //! リソース名
    std::wstring name;
  };

 public:
  /**
   * @brief コンストラクタ
   */
  MultiRenderTargetTexture();
  /**
   * @brief デストラクタ
   */
  ~MultiRenderTargetTexture();
  /**
   * @brief シングルレンダーターゲットとして初期化する
   * @param accessor DirectXデバイスアクセサ
   * @param srv_local_heap_id シェーダーリソースのローカルヒープID
   * @param info レンダーターゲットの情報
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(device::IDirectXAccessor& accessor,
            descriptor_heap::heap_parameter::LocalHeapID srv_local_heap_id,
            const Info& info);
  /**
   * @brief マルチレンダーターゲットとして初期化する
   * @param accessor DirectXデバイスアクセサ
   * @param srv_local_heap_id シェーダーリソースのローカルヒープID
   * @param infos 各レンダーターゲットの情報
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(device::IDirectXAccessor& accessor,
            descriptor_heap::heap_parameter::LocalHeapID srv_local_heap_id,
            const std::vector<Info>& infos);
  /**
   * @brief バッファから初期化する
   * @param accessor DirectXデバイスアクセサ
   * @param buffer バッファ
   * @param clear_color レンダーターゲットのクリア色
   * @param name リソース名
   * @return 初期化に成功したらtrueを返す
   * @details 主にバックバッファのレンダーターゲットに使用する
   シェーダーリソースとしては使用しない設定にする
   */
  bool InitFromBuffer(device::IDirectXAccessor& accessor,
                      ComPtr<ID3D12Resource> buffer,
                      const util::Color4& clear_color,
                      const std::wstring& name);
  /**
   * @brief レンダーターゲットの色をクリアする
   * @param command_list コマンドリスト
   * @details レンダーターゲットにセットされていないときは無効
   */
  void ClearRenderTarget(device::CommandList& command_list) const;
  /**
   * @brief シェーダーリソースビューとして使用する
   * @param accessor DirectXデバイスアクセサ
   * @param render_target_number 使用するレンダーターゲット番号
   * @details render_target_numberは何番目のテクスチャを使用するかを指定する
   事前にTransitionでシェーダーリソースとして扱えるようにしておく必要がある
   */
  void UseAsSRV(device::IDirectXAccessor& accessor, u32 render_target_number);
  /**
   * @brief ハンドルを取得する
   */
  std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> GetRTVHandles() const {
    return rtv_handles_;
  }
  /**
   * @brief ビューポートをセットする
   * @param command_list コマンドリスト
   */
  void SetViewport(device::CommandList& command_list) const;
  /**
   * @brief シザー矩形をセットする
   * @param command_list コマンドリスト
   */
  void SetScissorRect(device::CommandList& command_list) const;
  /**
   * @brief 状態を遷移させる
   * @param command_list コマンドリスト
   * @param next_state 次の状態
   */
  void Transition(device::CommandList& command_list,
                  D3D12_RESOURCE_STATES next_state);

 private:
  //! レンダーターゲット数
  u32 render_target_num_;
  //! ビューポート
  std::vector<D3D12_VIEWPORT> viewports_;
  //! シザー矩形
  std::vector<D3D12_RECT> scissor_rects_;
  //! レンダーターゲット
  std::vector<RenderTargetTexture> render_targets_;
  //! ハンドル
  std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> rtv_handles_;
};

}  // namespace render_target
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_RENDER_TARGET_MULTI_RENDER_TARGET_TEXTURE_H_

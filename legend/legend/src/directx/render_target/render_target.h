#ifndef LEGEND_DIRECTX_RENDER_TARGET_RENDER_TARGET_H_
#define LEGEND_DIRECTX_RENDER_TARGET_RENDER_TARGET_H_

/**
 * @file render_target.h
 * @brief レンダーターゲットクラス定義
 */

#include "src/directx/buffer/committed_resource.h"
#include "src/directx/device/directx_accessor.h"
#include "src/util/color_4.h"

namespace legend {
namespace directx {
namespace render_target {

/**
 * @class RenderTarget
 * @brief レンダーターゲット
 */
class RenderTarget {
 public:
  /**
   * @brief レンダーターゲットデスク
   */
  struct RenderTargetDesc {
    //! リソース名
    std::wstring name;
    //! フォーマット
    DXGI_FORMAT format;
    //! 幅
    u32 width;
    //! 高さ
    u32 height;
    //! クリア色
    util::Color4 clear_color;
  };

 public:
  /**
   * @brief コンストラクタ
   */
  RenderTarget();
  /**
   * @brief デストラクタ
   */
  ~RenderTarget();
  /**
   * @brief 初期化
   * @param accessor DirectXデバイスアクセサ
   * @param desc レンダーターゲットデスク
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(device::IDirectXAccessor& accessor, const RenderTargetDesc& desc);
  /**
   * @brief バッファから初期化する
   * @param accessor DirectXデバイスアクセサ
   * @param buffer バッファ
   * @param clear_color クリア色
   * @param name リソース名
   * @return 初期化に成功したらtrueを返す
   */
  bool InitFromBuffer(device::IDirectXAccessor& accessor,
                      ComPtr<ID3D12Resource> buffer,
                      const util::Color4& clear_color,
                      const std::wstring& name);
  /**
   * @brief 状態を遷移させる
   * @param command_list コマンドリスト
   * @param next_state 次の状態
   */
  void Transition(device::CommandList& command_list,
                  D3D12_RESOURCE_STATES next_state);
  /**
   * @brief レンダーターゲットをクリアする
   * @param command_list コマンドリスト
   */
  void ClearRenderTarget(device::CommandList& command_list) const;

 public:
  /**
   * @brief リソースを取得する
   */
  inline ID3D12Resource* GetResource() const { return resource_.GetResource(); }
  /**
   * @brief ハンドルを取得する
   */
  inline descriptor_heap::DescriptorHandle GetHandle() const {
    return rtv_handle_;
  }
  /**
   * @brief フォーマットを取得する
   */
  inline DXGI_FORMAT GetFormat() const { return format_; }
  /**
   * @brief ビューポートを取得する
   */
  inline D3D12_VIEWPORT GetViewport() const { return viewport_; }
  /**
   * @brief シザー矩形を取得する
   */
  inline D3D12_RECT GetScissorRect() const { return scissor_rect_; }

 private:
  //! リソース
  buffer::CommittedResource resource_;
  //! レンダーターゲットハンドル
  descriptor_heap::DescriptorHandle rtv_handle_;
  //! バッファのクリア色
  util::Color4 clear_color_;
  //! レンダーターゲットのフォーマット
  DXGI_FORMAT format_;
  //! ビューポート
  D3D12_VIEWPORT viewport_;
  //! シザー矩形
  D3D12_RECT scissor_rect_;
};

}  // namespace render_target
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_RENDER_TARGET_RENDER_TARGET_H_

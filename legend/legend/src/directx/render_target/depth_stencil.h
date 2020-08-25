#ifndef LEGEND_DIRECTX_RENDER_TARGET_DEPTH_STENCIL_H_
#define LEGEND_DIRECTX_RENDER_TARGET_DEPTH_STENCIL_H_

/**
 * @file depth_stencil.h
 * @brief デプス・ステンシルクラス定義
 */

#include "src/directx/buffer/committed_resource.h"
#include "src/directx/device/command_list.h"
#include "src/directx/device/directx_accessor.h"

namespace legend {
namespace directx {
namespace render_target {
/**
 * @class DepthStencil
 * @brief デプス・ステンシルクラス
 */
class DepthStencil {
 public:
  /**
   * @brief デプス・ステンシルデスク
   */
  struct DepthStencilDesc {
    //! リソース名
    std::wstring name;
    //! フォーマット
    DXGI_FORMAT format;
    //! 幅
    u32 width;
    //! 高さ
    u32 height;
    //! デプス値
    float depth_value;
    //! ステンシル値
    u8 stencil_value;
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
   * @param accessor DirectXデバイスアクセサ
   * @param desc デプス・ステンシルデスク
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(device::IDirectXAccessor& accessor, const DepthStencilDesc& desc);
  /**
   * @brief デプス・ステンシル値のクリア
   * @param command_list コマンドリスト
   */
  void ClearDepthStencil(device::CommandList& command_list) const;
  /**
   * @brief 状態を遷移させる
   * @param command_list コマンドリスト
   * @param next_states 次の状態
   */
  void Transition(device::CommandList& command_list,
                  D3D12_RESOURCE_STATES next_states);

  void UseAsSRV(device::IDirectXAccessor& accessor,
                device::CommandList& command_list, u32 register_num);
  /**
   * @brief ディスクリプタハンドルを取得する
   */
  descriptor_heap::DescriptorHandle GetHandle() const { return handle_; }

  DXGI_FORMAT GetFormat() const { return format_; }

  D3D12_DEPTH_STENCIL_DESC GetDepthStencilDesc() const;

 private:
  //! リソース
  buffer::CommittedResource resource_;
  //! ハンドル
  descriptor_heap::DescriptorHandle handle_;
  descriptor_heap::DescriptorHandle srv_handle_;
  //! フォーマット
  DXGI_FORMAT format_;
  //! デプス値
  float depth_value_;
  //! ステンシル値
  u8 stencil_value_;
};

}  // namespace render_target
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_RENDER_TARGET_DEPTH_STENCIL_H_

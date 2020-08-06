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
  struct DepthStencilDesc {
    std::wstring name;
    DXGI_FORMAT format;
    u32 width;
    u32 height;
    float depth_value;
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
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(device::IDirectXAccessor& accessor, const DepthStencilDesc& desc);
  /**
   * @brief デプス・ステンシル値のクリア
   */
  void ClearDepthStencil(device::CommandList& command_list) const;
  void Transition(device::CommandList& command_list,
                  D3D12_RESOURCE_STATES next_states);
  descriptor_heap::DescriptorHandle GetHandle() const { return handle_; }

 private:
  //! リソース
  buffer::CommittedResource resource_;
  //! ハンドル
  descriptor_heap::DescriptorHandle handle_;
  //! フォーマット
  DXGI_FORMAT format_;
  //! クリア値
  float depth_value_;
  u8 stencil_value_;
};

}  // namespace render_target
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_RENDER_TARGET_DEPTH_STENCIL_H_

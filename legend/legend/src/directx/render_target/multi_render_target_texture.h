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
   * @param accessor DirectX12アクセサ
   * @param info レンダーターゲットの情報
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(device::IDirectXAccessor& accessor,
            descriptor_heap::heap_parameter::LocalHeapID srv_local_heap_id,
            const Info& info);
  /**
   * @brief マルチレンダーターゲットとして初期化する
   * @param accessor DirectX12アクセサ
   * @param info 各レンダーターゲットの情報
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(device::IDirectXAccessor& accessor,
            descriptor_heap::heap_parameter::LocalHeapID srv_local_heap_id,
            const std::vector<Info>& infos);
  bool InitFromBuffer(device::IDirectXAccessor& accessor,
                      ComPtr<ID3D12Resource> buffer,
                      const util::Color4& clear_color,
                      const std::wstring& name);
  /**
   * @brief レンダーターゲットの色をクリアする
   * @param accessor DirextX12アクセサ
   * @details レンダーターゲットにセットされていないときは無効
   */
  void ClearRenderTarget(device::CommandList& command_list) const;
  /**
   * @brief 描画終了
   * @param accessor DirextX12アクセサ
   */
  void DrawEnd(device::CommandList& command_list);

  void PrepareToUseRenderTargets(directx::device::CommandList& command_list);
  void UseAsSRV(device::IDirectXAccessor& accessor, u32 render_target_number);
  std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> GetRTVHandles() const {
    return rtv_handles_;
  }
  void SetViewport(device::CommandList& command_list) const;
  void SetScissorRect(device::CommandList& command_list) const;
  void Transition(device::CommandList& command_list,
                  D3D12_RESOURCE_STATES next_state);

 private:
  u32 render_target_num_;
  std::vector<D3D12_VIEWPORT> viewports_;
  std::vector<D3D12_RECT> scissor_rects_;
  std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> rtv_handles_;
  std::vector<RenderTargetTexture> render_targets_;
};

}  // namespace render_target
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_RENDER_TARGET_MULTI_RENDER_TARGET_TEXTURE_H_

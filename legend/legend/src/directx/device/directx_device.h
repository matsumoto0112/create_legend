#ifndef LEGEND_DIRECTX_DEVICE_DIRECTX_DEVICE_H_
#define LEGEND_DIRECTX_DEVICE_DIRECTX_DEVICE_H_

/**
 * @file directx_device.h
 * @brief DirectXデバイスクラス定義
 */

#include "src/directx/descriptor_heap/heap_manager.h"
#include "src/directx/device/command_list.h"
#include "src/directx/device/directx_accessor.h"
#include "src/directx/device/dxgi_adapter.h"
#include "src/directx/device/swap_chain.h"
#include "src/directx/frame_resource.h"
#include "src/directx/render_target/depth_stencil.h"
#include "src/directx/render_target/render_resource_manager.h"
#include "src/directx/shader/root_signature.h"

namespace legend {
namespace directx {
namespace device {

/**
 * @class DirectXDevice
 * @brief DirectXデバイスクラス
 */
class DirectXDevice : public IDirectXAccessor {
 public:
  /**
   * @brief コンストラクタ
   */
  DirectXDevice();
  /**
   * @brief デストラクタ
   */
  ~DirectXDevice();
  /**
   * @brief 初期化
   * @param width 画面幅
   * @param height 画面高さ
   * @param hwnd ウィンドウハンドル
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(u32 width, u32 height, HWND hwnd);
  /**
   * @brief 描画準備
   * @return 成功したらtrueを返す
   */
  bool Prepare();
  /**
   * @brief 描画処理
   * @return 成功したらtrueを返す
   */
  bool Present();
  /**
   * @brief デバイスを破棄する
   */
  void Destroy();

 public:
  /**
   * @brief デバイスを取得する
   */
  ID3D12Device* GetDevice() const override { return device_.Get(); }
  /**
   * @brief レンダーターゲットハンドルを取得する
   */
  descriptor_heap::DescriptorHandle GetRTVHandle() override;
  /**
   * @brief デプス・ステンシルハンドルを取得する
   */
  descriptor_heap::DescriptorHandle GetDSVHandle() override;
  /**
   * @brief ハンドルをグローバルに登録する
   * @param register_num シェーダーのレジスター番号
   * @param type リソースの種類
   * @param handle 登録するハンドル
   */
  void RegisterHandle(u32 register_num, shader::ResourceType type,
                      descriptor_heap::DescriptorHandle handle);
  /**
   * @brief ローカルハンドルを取得する
   * @param heap_id 使用するローカルハンドルのID
   */
  descriptor_heap::DescriptorHandle GetLocalHandle(
      descriptor_heap::heap_parameter::LocalHeapID heap_id) override;
  /**
   * @brief ヒープ管理オブジェクトを取得する
   */
  descriptor_heap::HeapManager& GetHeapManager() const {
    return *heap_manager_;
  }
  /**
   * @brief コマンドリストを実行する
   * @param command_lists セットするコマンドリスト
   */
  void ExecuteCommandList(const std::vector<CommandList>& command_lists);
  /**
   * @brief コマンドの実行を待機する
   */
  void WaitExecute();
  /**
   * @brief 描画リソース管理オブジェクトを取得する
   */
  render_target::RenderResourceManager& GetRenderResourceManager() {
    return render_resource_manager_;
  }
  /**
   * @brief 現在のフレームリソースを取得する
   */
  FrameResource* GetCurrentFrameResource() const { return current_resource_; }
  /**
   * @brief デフォルトのルートシグネチャを取得する
   */
  shader::RootSignature* GetDefaultRootSignature() const {
    return default_root_signature_.get();
  }
  /**
   * @brief 現在のフレームインデックスを取得する
   */
  u32 GetCurrentBackBufferIndex() const override;

 private:
  //! アダプター
  DXGIAdapter adapter_;
  //!デバイス
  ComPtr<ID3D12Device> device_;
  //! コマンドキュー
  ComPtr<ID3D12CommandQueue> command_queue_;

  //! フレームインデックス
  u32 frame_index_;
  //! 同期用フェンス
  ComPtr<ID3D12Fence> fence_;
  //! 同期値
  UINT64 fence_value_;

  //! フレームリソース
  FrameResource resources_[defines::FRAME_COUNT];
  //! 現在のフレームリソース
  FrameResource* current_resource_;
  //! ヒープ管理オブジェクト
  std::unique_ptr<descriptor_heap::HeapManager> heap_manager_;
  //! 描画リソース管理オブジェクト
  render_target::RenderResourceManager render_resource_manager_;
  //! デフォルトのルートシグネチャ
  std::unique_ptr<shader::RootSignature> default_root_signature_;
  //! デバイスフラグ
  DeviceOptionFlags option_flags_;
};

}  // namespace device
}  // namespace directx
}  // namespace legend
#endif  //! LEGEND_DIRECTX_DEVICE_DIRECTX_DEVICE_H_

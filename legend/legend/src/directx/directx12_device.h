#ifndef LEGEND_DIRECTX_DIRECTX12_DEVICE_H_
#define LEGEND_DIRECTX_DIRECTX12_DEVICE_H_

/**
 * @file directx12_device.h
 * @brief DirectX12デバイス定義
 */

#include <wrl/wrappers/corewrappers.h>

#include "src/directx/device/command_list.h"
#include "src/directx/device/dxgi_adapter.h"
#include "src/directx/device/swap_chain.h"
#include "src/directx/directx_accessor.h"
#include "src/directx/descriptor_heap/heap_manager.h"
#include "src/directx/render_target/render_resource_manager.h"
#include "src/directx/shader/root_signature.h"
#include "src/libs/d3dx12.h"
#include "src/window/window.h"

namespace legend {
namespace directx {

/**
 * @class DirectX12Device
 * @brief DirectX12デバイス
 */
class DirectX12Device : public IDirectXAccessor {
 public:
  /**
   * @brief コンストラクタ
   */
  DirectX12Device();
  /**
   * @brief デストラクタ
   */
  ~DirectX12Device();
  /**
   * @brief 初期化
   * @param target_window 描画対象のウィンドウ
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(std::weak_ptr<window::Window> target_window);
  /**
   * @brief 初期化後の処理
   * @return その処理に成功したらtrueを返す
   */
  bool InitAfter();
  /**
   * @brief 描画準備
   * @return 成功したらtrueを返す
   */
  bool Prepare();
  /**
   * @brief 描画内容を画面に映す
   * @return 成功したらtrueを返す
   */
  bool Present();

  /**
   * @brief バックバッファのレンダーターゲットをセットする
   */
  void SetBackBuffer();

  /**
   * @brief GPUの処理を待機する
   */
  void WaitForGPU() noexcept;
  /**
   * @brief ディスクリプタハンドルを取得する
   * @param heap_type 取得するディスクリプタヒープの種類
   */
  virtual DescriptorHandle GetHandle(DescriptorHeapType heap_type) override;
  /**
   * @brief グローバルヒープにディスクリプタハンドルをセットする
   * @param register_num セットするハンドルのシェーダにおけるレジスター番号
   * @param resource_type リソースの種類
   * @param handle セットするハンドル
   */
  virtual void SetToGlobalHeap(u32 register_num, ResourceType resource_type,
                               const DescriptorHandle& handle) override;

  virtual DescriptorHandle GetBackBufferHandle() const override;
  virtual void ClearBackBufferTarget(IDirectXAccessor& accessor) override;
  virtual DXGI_FORMAT GetBackBufferFormat() const;
  virtual void SetBackBuffer(IDirectXAccessor& accessor) override;
 public:
  virtual inline ID3D12Device* GetDevice() const override {
    return device_.Get();
  }
  virtual inline ID3D12GraphicsCommandList4* GetCommandList() const override {
    return command_lists_[frame_index_].GetCommandList();
  }
  const inline render_target::RenderTarget& GetRenderTarget() const {
    return swap_chain_.GetRenderTarget();
  }
  inline render_target::RenderResourceManager& GetRenderResourceManager() {
    return render_resource_manager_;
  }
  /**
   * @brief ディスクリプタヒープ管理者を取得する
   */
  inline HeapManager& GetHeapManager() { return heap_manager_; }
  /**
   * @brief デフォルトのルートシグネチャを取得する
   */
  inline std::shared_ptr<shader::RootSignature> GetDefaultRootSignature()
      const {
    return default_root_signature_;
  }

 private:
  /**
   * @brief デバイスの作成
   * @return 成功したらtrueを返す
   */
  bool CreateDevice();
  /**
   * @brief 次のフレームに遷移させる
   * @return
   */
  bool MoveToNextFrame();

 private:
  //! トリプルバッファを使用する
  static constexpr u32 FRAME_COUNT = 3;

 private:
  //! デバイス
  ComPtr<ID3D12Device> device_;
  //! アダプター
  device::DXGIAdapter adapter_;
  //! スワップチェイン
  device::SwapChain swap_chain_;
  //! レンダーターゲットとなるウィンドウ
  std::weak_ptr<window::Window> target_window_;
  //! レンダーターゲットとなるスクリーンの大きさ
  math::IntVector2 render_target_screen_size_;
  //! コマンドキュー
  ComPtr<ID3D12CommandQueue> command_queue_;
  //! バッファインデックス
  i32 frame_index_;
  //! コマンドアロケータ
  std::array<device::CommandList, FRAME_COUNT> command_lists_;
  //! コマンドフェンス
  ComPtr<ID3D12Fence> fence_;
  //!< 現在のレンダーターゲットの状態
  D3D12_RESOURCE_STATES current_resource_state_;
  //! フェンス値
  std::array<u64, FRAME_COUNT> fence_values_;
  //! フェンスイベント
  Microsoft::WRL::Wrappers::Event fence_event_;
  //! ディスクリプタヒープ管理
  HeapManager heap_manager_;
  //! デフォルトのルートシグネチャ
  std::shared_ptr<shader::RootSignature> default_root_signature_;
  render_target::RenderResourceManager render_resource_manager_;
};
}  // namespace directx
}  // namespace legend

#endif  // !LEGEND_DIRECTX_DIRECTX12_DEVICE_H_

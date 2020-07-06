#ifndef LEGEND_DIRECTX_DIRECTX12_DEVICE_H_
#define LEGEND_DIRECTX_DIRECTX12_DEVICE_H_

/**
 * @file directx12_device.h
 * @brief DirectX12デバイス定義
 */

#include <wrl/wrappers/corewrappers.h>

#include "src/directx/buffer/render_target.h"
#include "src/directx/device/dxgi_adapter.h"
#include "src/directx/directx_accessor.h"
#include "src/directx/heap_manager.h"
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
  bool Init(std::shared_ptr<window::Window> target_window);
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

  void SetBackBuffer();

  /**
   * @brief GPUの処理を待機する
   */
  void WaitForGPU() noexcept;

  virtual DescriptorHandle GetHandle(DescriptorHeapType heap_type) override;
  virtual void SetToGlobalHeap(u32 register_num, ResourceType resource_type,
                               const DescriptorHandle& handle) override;

 public:
  virtual ID3D12Device* GetDevice() const override { return device_.Get(); }
  virtual ID3D12GraphicsCommandList4* GetCommandList() const override {
    return command_list_.Get();
  }
  const buffer::RenderTarget& GetRenderTarget() const {
    return render_targets_[frame_index_];
  }
  /**
   * @brief ディスクリプタヒープ管理者を取得する
   */
  HeapManager& GetHeapManager() { return heap_manager_; }

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
  ComPtr<ID3D12Device> device_;
  device::DXGIAdapter adapter_;
  //! レンダーターゲットとなるウィンドウ
  std::weak_ptr<window::Window> target_window_;
  //! レンダーターゲットとなるスクリーンの大きさ
  math::IntVector2 render_target_screen_size_;
  //! コマンドキュー
  ComPtr<ID3D12CommandQueue> command_queue_;
  //! スワップチェイン
  ComPtr<IDXGISwapChain4> swap_chain_;
  //! バッファインデックス
  i32 frame_index_;
  //! レンダーターゲットのリソース
  std::array<buffer::RenderTarget, FRAME_COUNT> render_targets_;
  //! レンダーターゲットのヒープサイズ
  i32 rtv_heap_size_;
  //! コマンドアロケータ
  std::array<ComPtr<ID3D12CommandAllocator>, FRAME_COUNT> command_allocator_;
  //! コマンドフェンス
  ComPtr<ID3D12Fence> fence_;
  //! コマンドリスト
  ComPtr<ID3D12GraphicsCommandList4> command_list_;
  //!< 現在のレンダーターゲットの状態
  D3D12_RESOURCE_STATES current_resource_state_;
  //! フェンス値
  std::array<u64, FRAME_COUNT> fence_values_;
  //! フェンスイベント
  Microsoft::WRL::Wrappers::Event fence_event_;
  //! ディスクリプタヒープ管理
  HeapManager heap_manager_;
};
}  // namespace directx
}  // namespace legend

#endif  // !LEGEND_DIRECTX_DIRECTX12_DEVICE_H_

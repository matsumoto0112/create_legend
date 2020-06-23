#ifndef LEGEND_DIRECTX_DIRECTX12_DEVICE_H_
#define LEGEND_DIRECTX_DIRECTX12_DEVICE_H_

/**
 * @file directx12_device.h
 * @brief DirectX12デバイス定義
 */

#include <wrl/wrappers/corewrappers.h>

#include "src/libs/d3dx12.h"
#include "src/window/window.h"

namespace legend {
namespace directx {
/**
 * @class DirectX12Device
 * @brief DirectX12デバイス
 */
class DirectX12Device {
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

  bool Prepare();
  bool Present();

 public:
  ID3D12Device* GetDevice() const { return device_.Get(); }
  ID3D12GraphicsCommandList4* GetCommandList() const {
    return command_list_.Get();
  }

 private:
  bool CreateDevice();
  ComPtr<IDXGIAdapter1> GetHardwareAdapter();
  bool MoveToNextFrame();

 private:
  //! トリプルバッファを使用する
  static constexpr u32 FRAME_COUNT = 3;

 private:
  //! レンダーターゲットとなるウィンドウ
  std::weak_ptr<window::Window> target_window_;
  //! レンダーターゲットとなるスクリーンの大きさ
  math::IntVector2 render_target_screen_size_;
  //! DXGIファクトリ
  ComPtr<IDXGIFactory4> factory_;
  //! D3D12デバイス
  ComPtr<ID3D12Device> device_;
  //! コマンドキュー
  ComPtr<ID3D12CommandQueue> command_queue_;
  //! スワップチェイン
  ComPtr<IDXGISwapChain4> swap_chain_;
  //! バッファインデックス
  i32 frame_index_;
  //! レンダーターゲットのディスクリプタヒープ
  ComPtr<ID3D12DescriptorHeap> rtv_heap_;
  //! レンダーターゲットのリソース
  std::array<ComPtr<ID3D12Resource>, FRAME_COUNT> render_targets_;
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
};
}  // namespace directx
}  // namespace legend

#endif  // !LEGEND_DIRECTX_DIRECTX12_DEVICE_H_

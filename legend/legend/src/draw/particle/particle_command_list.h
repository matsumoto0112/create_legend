#ifndef LEGEND_DRAW_PARTICLE_PARTICLE_COMMAND_LIST_H_
#define LEGEND_DRAW_PARTICLE_PARTICLE_COMMAND_LIST_H_

/**
 * @file particle_command_list.h
 * @brief パーティクル用コマンドリスト定義
 */

#include "src/directx/device/directx_accessor.h"
#include "src/directx/device/directx_device.h"
#include "src/directx/frame_resource.h"

namespace legend {
namespace draw {
namespace particle {

/**
 * @brief パーティクル用コマンドリストクラス
 */
class ParticleCommandList {
 public:
  /**
   * @brief コンストラクタ
   */
  ParticleCommandList();
  /**
   * @brief デストラクタ
   */
  virtual ~ParticleCommandList();
  /**
   * @brief 初期化
   * @param accessor DirectXデバイスアクセサ
   * @param frame_count バックバッファのフレーム数
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(directx::device::IDirectXAccessor& accessor, u32 frame_count);
  /**
   * @brief フレーム開始時処理
   * @param device DirectXデバイス
   */
  void BeginFrame(directx::device::DirectXDevice& device);
  /**
   * @brief コマンドリストの実行処理
   */
  void Execute();
  /**
   * @brief コマンドリストを取得する
   */
  directx::device::CommandList& GetCommandList() {
    return current_frame_resource_->GetCommandList();
  }

 private:
  //! フレームリソース
  std::vector<directx::FrameResource> frame_resources_;
  //! 現在フレームのリソース
  directx::FrameResource* current_frame_resource_;
  //! コマンドキュー
  ComPtr<ID3D12CommandQueue> command_queue_;
  //! コマンドフェンス
  ComPtr<ID3D12Fence> fence_;
  //! フェンス値
  u64 fence_value_;
};
}  // namespace particle
}  // namespace draw
}  // namespace legend
#endif  //! LEGEND_DRAW_PARTICLE_PARTICLE_COMMAND_LIST_H_

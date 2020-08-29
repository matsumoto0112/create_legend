#ifndef LEGEND_DRAW_PARTICLE_PARTICLE_COMMAND_LIST_H_
#define LEGEND_DRAW_PARTICLE_PARTICLE_COMMAND_LIST_H_

/**
 * @file particle_command_list.h
 * @brief パーティクル用コマンドリスト定義
 */

#include "src/directx/device/directx_accessor.h"
#include "src/directx/device/directx_device.h"
#include "src/directx/frame_resource.h"
#include "src/draw/particle/particle_emitter.h"

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

  template <class T>
  std::shared_ptr<T> CreateParticle(directx::device::CommandList& command_list);

  void UpdateParticles();
  void RenderParticle(directx::device::CommandList& render_command_list);
  /**
   * @brief フレーム開始時処理
   * @param device DirectXデバイス
   */
  void BeginFrame(directx::device::DirectXDevice& device);
  ///**
  // * @brief コマンドリストの実行処理
  // */
  // void Execute();
  ///**
  // * @brief コマンドリストを取得する
  // */
  // directx::device::CommandList& GetCommandList() {
  //  return current_frame_resource_->GetCommandList();
  //}

 private:
  std::vector<std::shared_ptr<ParticleEmitter>> particle_emitters_;
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
template <class T>
inline std::shared_ptr<T> ParticleCommandList::CreateParticle(
    directx::device::CommandList& command_list) {
  auto res = std::make_shared<T>();
  res->Init(command_list);
  particle_emitters_.emplace_back(res);
  return res;
}

}  // namespace particle
}  // namespace draw
}  // namespace legend
#endif  //! LEGEND_DRAW_PARTICLE_PARTICLE_COMMAND_LIST_H_

#ifndef LEGEND_DRAW_PARTICLE_PARTICLE_EMITTER_H_
#define LEGEND_DRAW_PARTICLE_PARTICLE_EMITTER_H_

/**
 * @file particle_emitter.h
 * @brief パーティクルエミッター基底クラス定義
 */

#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/buffer/constant_buffer_structure.h"
#include "src/directx/device/command_list.h"
#include "src/directx/shader/pipeline_state.h"
#include "src/util/transform.h"

namespace legend {
namespace draw {
namespace particle {

/**
 * @class ParticleEmitter
 * @brief パーティクルエミッター
 */
class ParticleEmitter {
 protected:
  using TransformStruct = directx::buffer::constant_buffer_structure::Transform;
  using TransformConstantBuffer =
      directx::buffer::ConstantBuffer<TransformStruct>;

 public:
  /**
   * @brief コンストラクタ
   * @param particle_max_size パーティクルの最大数
   * @param particle_structure_size パーティクルの構造体のサイズ
   * @param dispatch_x 作業スレッドグループ数(X)
   * @param dispatch_y 作業スレッドグループ数(Y)
   * @param name パーティクル名
   */
  ParticleEmitter(u32 particle_max_size, u32 particle_structure_size,
                  u32 dispatch_x, u32 dispatch_y, const std::wstring& name);
  /**
   * @brief デストラクタ
   */
  virtual ~ParticleEmitter();
  /**
   * @brief 初期化
   * @param command_list バッファをコピーするのに使用するコマンドリスト
   * @param data コピー元のデータ
   * @param graphics_desc グラフィックスパイプラインステートディスク
   * @param compute_desc コンピュートパイプラインステートディスク
   * @return 初期化に成功したらtrueを返す
   */
  virtual bool Init(directx::device::CommandList& command_list,
                    const void* data,
                    D3D12_GRAPHICS_PIPELINE_STATE_DESC graphics_desc,
                    D3D12_COMPUTE_PIPELINE_STATE_DESC compute_desc);
  /**
   * @brief 更新処理
   * @param command_list パーティクルの更新処理用のコマンドリスト
   */
  virtual void Update(directx::device::CommandList& command_list);
  /**
   * @brief 描画処理
   * @param graphics_command_list 描画処理を実行するコマンドリスト
   */
  virtual void Render(directx::device::CommandList& graphics_command_list);

  void SetTransform(const util::Transform& transform) {
    this->transform_ = transform;
  }
  util::Transform GetTransform() const { return transform_; }
  /**
   * @brief トランスフォームの参照を返す
   * @return
   */
  util::Transform& GetTransformRef() { return transform_; }

 protected:
  //! パーティクル名
  const std::wstring name_;
  //! パーティクルの最大数
  const u32 particle_max_num_;
  //! パーティクル構造体のサイズ
  const u32 particle_structure_size_;
  //! 作業スレッドグループ数(X)
  const u32 dispatch_x_;
  //! 作業スレッドグループ数(Y)
  const u32 dispatch_y_;

 protected:
  //! グラフィックスパイプライン
  directx::shader::PipelineState graphics_pipeline_state_;
  //! コンピュートパイプライン
  directx::shader::PipelineState compute_pipeline_state_;

  //! トランスフォーム
  util::Transform transform_;
  //! トランスフォームコンスタントバッファ
  TransformConstantBuffer transform_cb_;

  //! パーティクルデータのUAV兼頂点バッファ
  ComPtr<ID3D12Resource> particle_uav_;
  //! パーティクルデータのアップロード用バッファ
  ComPtr<ID3D12Resource> particle_uav_upload_;
  //! UAVハンドル
  directx::descriptor_heap::DescriptorHandle handle_;
  //! 頂点バッファビュー
  D3D12_VERTEX_BUFFER_VIEW vertex_buffer_view_;
};

}  // namespace particle
}  // namespace draw
}  // namespace legend
#endif  //! LEGEND_DRAW_PARTICLE_PARTICLE_EMITTER_H_

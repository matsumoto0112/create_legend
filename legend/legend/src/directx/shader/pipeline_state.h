#ifndef LEGEND_DIRECTX_SHADER_PIPELINE_STATE_H_
#define LEGEND_DIRECTX_SHADER_PIPELINE_STATE_H_

/**
 * @file pipeline_state.h
 * @brief パイプラインステートクラス定義
 */

#include "src/directx/device/command_list.h"
#include "src/directx/device/directx_accessor.h"

namespace legend {
namespace directx {
namespace shader {

/**
 * @class PipelineState
 * @brief パイプラインステートクラス
 */
class PipelineState {
 public:
  using GraphicsPipelineStateDesc = D3D12_GRAPHICS_PIPELINE_STATE_DESC;
  using ComputePipelineStateDesc = D3D12_COMPUTE_PIPELINE_STATE_DESC;

 public:
  /**
   * @brief コンストラクタ
   */
  PipelineState();
  /**
   * @brief デストラクタ
   */
  ~PipelineState();
  /**
   * @brief 初期化
   * @param accessor DirectXデバイスアクセサ
   * @param desc グラフィックスパイプラインデスク
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(device::IDirectXAccessor& accessor,
            const GraphicsPipelineStateDesc& desc);
  /**
   * @brief 初期化
   * @param accessor DirectXデバイスアクセサ
   * @param desc コンピュートパイプラインデスク
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(device::IDirectXAccessor& accessor,
            const ComputePipelineStateDesc& desc);
  /**
   * @brief コマンドリストにセットする
   * @param command_list コマンドリスト
   */
  void SetCommandList(device::CommandList& command_list);

 protected:
  //! パイプラインステート
  ComPtr<ID3D12PipelineState> pipeline_state_;
};

}  // namespace shader
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_SHADER_PIPELINE_STATE_H_

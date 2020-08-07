#ifndef LEGEND_DIRECTX_SHADER_GRAPHICS_PIPELINE_STATE_H_
#define LEGEND_DIRECTX_SHADER_GRAPHICS_PIPELINE_STATE_H_

/**
 * @file graphics_pipeline_state.h
 * @brief グラフィックパイプラインステートクラス定義
 */

#include "src/directx/device/command_list.h"
#include "src/directx/device/directx_accessor.h"

namespace legend {
namespace directx {
namespace shader {

/**
 * @class GraphicsPipelineState
 * @brief パイプラインステートクラス
 */
class GraphicsPipelineState {
 public:
  using PSODesc = D3D12_GRAPHICS_PIPELINE_STATE_DESC;

 public:
  /**
   * @brief コンストラクタ
   */
  GraphicsPipelineState();
  /**
   * @brief デストラクタ
   */
  ~GraphicsPipelineState();
  /**
   * @brief 初期化
   * @param accessor DirectXデバイスアクセサ
   * @param desc パイプラインデスク
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(device::IDirectXAccessor& accessor, const PSODesc& desc);
  /**
   * @brief コマンドリストにセットする
   * @param command_list コマンドリスト
   */
  void SetGraphicsCommandList(device::CommandList& command_list);

 protected:
  //! パイプラインステート
  ComPtr<ID3D12PipelineState> pipeline_state_;
};

}  // namespace shader
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_SHADER_GRAPHICS_PIPELINE_STATE_H_

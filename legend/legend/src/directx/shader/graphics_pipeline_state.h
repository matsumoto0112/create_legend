#ifndef LEGEND_DIRECTX_SHADER_GRAPHICS_PIPELINE_STATE_H_
#define LEGEND_DIRECTX_SHADER_GRAPHICS_PIPELINE_STATE_H_

/**
 * @file graphics_pipeline_state.h
 */

#include "src/directx/device/command_list.h"
#include "src/directx/device/directx_accessor.h"

namespace legend {
namespace directx {
namespace shader {

/**
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

  bool Init(device::IDirectXAccessor& accessor, const PSODesc& desc);

  void SetGraphicsCommandList(device::CommandList& command_list);
 protected:
  //! パイプラインステート
  ComPtr<ID3D12PipelineState> pipeline_state_;
};

}  // namespace shader
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_SHADER_GRAPHICS_PIPELINE_STATE_H_

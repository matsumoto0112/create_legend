#ifndef LEGEND_DIRECTX_SHADER_PIPELINE_STATE_H_
#define LEGEND_DIRECTX_HADER_PIPELINE_STATE_H_

/**
 * @file pipeline_state.h
 * @brief パイプラインステート定義
 */

#include "src/directx/directx12_device.h"
#include "src/directx/shader/pixel_shader.h"
#include "src/directx/shader/vertex_shader.h"

namespace legend {
namespace directx {
namespace shader {
class GraphicsPipelineState {
 public:
  GraphicsPipelineState();
  ~GraphicsPipelineState();
  bool Init(DirectX12Device& device);
  void SetVertexShader(std::shared_ptr<VertexShader> vertex_shader);
  void SetPixelShader(std::shared_ptr<PixelShader> pixel_shader);
  bool CreatePipelineState(DirectX12Device& device);
  void SetGraphicsCommandList(DirectX12Device& device);

 protected:
  D3D12_GRAPHICS_PIPELINE_STATE_DESC pipeline_state_desc_;
  std::shared_ptr<VertexShader> vertex_shader_;
  std::shared_ptr<PixelShader> pixel_shader_;

  ComPtr<ID3D12PipelineState> pipeline_state_;
  ComPtr<ID3D12RootSignature> root_signature_;
};

}  // namespace shader
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_SHADER_PIPELINE_STATE_H_

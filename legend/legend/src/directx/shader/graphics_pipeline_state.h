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
 * @brief �p�C�v���C���X�e�[�g�N���X
 */
class GraphicsPipelineState {
 public:
  using PSODesc = D3D12_GRAPHICS_PIPELINE_STATE_DESC;

 public:
  /**
   * @brief �R���X�g���N�^
   */
  GraphicsPipelineState();
  /**
   * @brief �f�X�g���N�^
   */
  ~GraphicsPipelineState();

  bool Init(device::IDirectXAccessor& accessor, const PSODesc& desc);

  void SetGraphicsCommandList(device::CommandList& command_list);
 protected:
  //! �p�C�v���C���X�e�[�g
  ComPtr<ID3D12PipelineState> pipeline_state_;
};

}  // namespace shader
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_SHADER_GRAPHICS_PIPELINE_STATE_H_

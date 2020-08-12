#ifndef LEGEND_DIRECTX_SHADER_PIPELINE_STATE_H_
#define LEGEND_DIRECTX_SHADER_PIPELINE_STATE_H_

/**
 * @file pipeline_state.h
 * @brief �p�C�v���C���X�e�[�g�N���X��`
 */

#include "src/directx/device/command_list.h"
#include "src/directx/device/directx_accessor.h"

namespace legend {
namespace directx {
namespace shader {

/**
 * @class PipelineState
 * @brief �p�C�v���C���X�e�[�g�N���X
 */
class PipelineState {
 public:
  using GraphicsPipelineStateDesc = D3D12_GRAPHICS_PIPELINE_STATE_DESC;
  using ComputePipelineStateDesc = D3D12_COMPUTE_PIPELINE_STATE_DESC;

 public:
  /**
   * @brief �R���X�g���N�^
   */
  PipelineState();
  /**
   * @brief �f�X�g���N�^
   */
  ~PipelineState();
  /**
   * @brief ������
   * @param accessor DirectX�f�o�C�X�A�N�Z�T
   * @param desc �O���t�B�b�N�X�p�C�v���C���f�X�N
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init(device::IDirectXAccessor& accessor,
            const GraphicsPipelineStateDesc& desc);
  /**
   * @brief ������
   * @param accessor DirectX�f�o�C�X�A�N�Z�T
   * @param desc �R���s���[�g�p�C�v���C���f�X�N
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init(device::IDirectXAccessor& accessor,
            const ComputePipelineStateDesc& desc);
  /**
   * @brief �R�}���h���X�g�ɃZ�b�g����
   * @param command_list �R�}���h���X�g
   */
  void SetCommandList(device::CommandList& command_list);

 protected:
  //! �p�C�v���C���X�e�[�g
  ComPtr<ID3D12PipelineState> pipeline_state_;
};

}  // namespace shader
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_SHADER_PIPELINE_STATE_H_

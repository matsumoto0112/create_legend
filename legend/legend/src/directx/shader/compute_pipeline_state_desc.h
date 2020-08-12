#ifndef LEGEND_DIRECTX_SHADER_COMPUTE_PIPELINE_STATE_DESC_H_
#define LEGEND_DIRECTX_SHADER_COMPUTE_PIPELINE_STATE_DESC_H_

/**
 * @file compute_pipeline_state_desc.h
 * @brief �R���s���[�g�p�C�v���C���X�e�[�g�f�B�X�N��`
 */

#include "src/directx/shader/compute_shader.h"
#include "src/directx/shader/root_signature.h"

namespace legend {
namespace directx {
namespace shader {

/**
 * @struct ComputePipelineStateDesc
 * @brief �R���s���[�g�p�C�v���C���X�e�[�g�f�B�X�N
 */
struct ComputePipelineStateDesc : public D3D12_COMPUTE_PIPELINE_STATE_DESC {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  ComputePipelineStateDesc() = default;
  /**
   * @brief ���[�g�V�O�l�`�����Z�b�g����
   */
  void SetRootSignature(RootSignature* root_signature);
  /**
   * @brief �R���s���[�g�V�F�[�_�[���Z�b�g����
   */
  void SetComputeShader(ComputeShader* shader);
};

}  // namespace shader
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_SHADER_COMPUTE_PIPELINE_STATE_DESC_H_

#ifndef LEGEND_DIRECTX_SHADER_GRAPHICS_PIPELINE_STATE_DESC_H_
#define LEGEND_DIRECTX_SHADER_GRAPHICS_PIPELINE_STATE_DESC_H_

/**
 * @file graphics_pipeline_state_desc.h
 * @brief �O���t�B�b�N�X�p�C�v���C���X�e�[�g�f�B�X�N��`
 */

#include "src/directx/render_target/depth_stencil.h"
#include "src/directx/render_target/multi_render_target_texture.h"
#include "src/directx/shader/geometry_shader.h"
#include "src/directx/shader/pixel_shader.h"
#include "src/directx/shader/root_signature.h"
#include "src/directx/shader/vertex_shader.h"

namespace legend {
namespace directx {
namespace shader {

/**
 * @struct GraphicsPipelineStateDesc
 * @brief �O���t�B�b�N�X�p�C�v���C���X�e�[�g�f�B�X�N
 */
struct GraphicsPipelineStateDesc : public D3D12_GRAPHICS_PIPELINE_STATE_DESC {
  /**
   * @brief �R���X�g���N�^
   */
  GraphicsPipelineStateDesc() = default;
  /**
   * @brief ���[�g�V�O�l�`�����Z�b�g����
   */
  void SetRootSignature(RootSignature* root_signature);
  /**
   * @brief ���_�V�F�[�_�[�̏����Z�b�g����
   */
  void SetVertexShader(VertexShader* shader);
  /**
   * @brief �s�N�Z���V�F�[�_�[�̏����Z�b�g����
   */
  void SetPixelShader(PixelShader* shader);
  /**
   * @brief �W�I���g���V�F�[�_�[�̏����Z�b�g����
   */
  void SetGeometryShader(GeometryShader* shader);
  /**
   * @brief �����_�[�^�[�Q�b�g�̏����Z�b�g����
   */
  void SetRenderTargets(
      const render_target::MultiRenderTargetTexture& render_target);
  /**
   * @brief �f�v�X�E�X�e���V���̏����Z�b�g����
   */
  void SetDepthStencilTarget(
      const render_target::DepthStencil& depth_stencil_target);
};
}  // namespace shader
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_SHADER_GRAPHICS_PIPELINE_STATE_DESC_H_

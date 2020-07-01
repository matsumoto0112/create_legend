#ifndef LEGEND_DIRECTX_SHADER_PIPELINE_STATE_H_
#define LEGEND_DIRECTX_SHADER_PIPELINE_STATE_H_

/**
 * @file pipeline_state.h
 * @brief �p�C�v���C���X�e�[�g��`
 */

#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/buffer/texture_2d.h"
#include "src/directx/descriptor_heap/descriptor_heap.h"
#include "src/directx/directx12_device.h"
#include "src/directx/shader/pixel_shader.h"
#include "src/directx/shader/root_signature.h"
#include "src/directx/shader/vertex_shader.h"

namespace legend {
namespace directx {
namespace shader {
/**
 * @brief �p�C�v���C���X�e�[�g�N���X
 */
class GraphicsPipelineState {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  GraphicsPipelineState();
  /**
   * @brief �f�X�g���N�^
   */
  ~GraphicsPipelineState();
  /**
   * @brief ������
   * @param device DirectX12�f�o�C�X
   */
  bool Init(DirectX12Device& device);
  /**
   * @brief ���[�g�V�O�l�`�����Z�b�g����
   */
  void SetRootSignature(std::shared_ptr<RootSignature> root_signature);
  /**
   * @brief ���_�V�F�[�_�[���Z�b�g����
   */
  void SetVertexShader(std::shared_ptr<VertexShader> vertex_shader);
  /**
   * @brief �s�N�Z���V�F�[�_�[���Z�b�g����
   */
  void SetPixelShader(std::shared_ptr<PixelShader> pixel_shader);
  /**
   * @brief �p�C�v���C���X�e�[�g���쐬����
   * @param device DirectX12�f�o�C�X
   */
  bool CreatePipelineState(DirectX12Device& device);
  /**
   * @brief �R�}���h���X�g�ɃZ�b�g����
   * @param device DirectX12�f�o�C�X
   */
  void SetGraphicsCommandList(DirectX12Device& device);

 protected:
  //! �p�C�v���C���X�e�[�g�f�X�N
  D3D12_GRAPHICS_PIPELINE_STATE_DESC pipeline_state_desc_;
  //! ���_�V�F�[�_�[
  std::shared_ptr<VertexShader> vertex_shader_;
  //! �s�N�Z���V�F�[�_�[
  std::shared_ptr<PixelShader> pixel_shader_;
  //! �p�C�v���C���X�e�[�g
  ComPtr<ID3D12PipelineState> pipeline_state_;
  //! ���[�g�V�O�l�`��
  std::shared_ptr<RootSignature> root_signature_;
};

}  // namespace shader
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_SHADER_PIPELINE_STATE_H_

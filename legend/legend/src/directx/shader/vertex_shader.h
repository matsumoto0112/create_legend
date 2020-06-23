#ifndef LEGEND_DIRECTX_SHADER_VERTEX_SHADER_H_
#define LEGEND_DIRECTX_SHADER_VERTEX_SHADER_H_

/**
 * @file vertex_shader.h
 * @brief ���_�V�F�[�_�[��`
 */

#include "src/directx/directx12_device.h"

namespace legend {
namespace directx {
namespace shader {
/**
 * @brief ���_�V�F�[�_�[
 */
class VertexShader {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  VertexShader();
  /**
   * @brief �f�X�g���N�^
   */
  ~VertexShader();
  /**
   * @brief ������
   * @param device DirectX�f�o�C�X
   * @param filepath �V�F�[�_�[�t�@�C���p�X
   * @param elements ���̓��C�A�E�g
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init(DirectX12Device& device, const std::filesystem::path& filepath,
            const std::vector<D3D12_INPUT_ELEMENT_DESC>& elements);
  /**
   * @brief �V�F�[�_�[�R�[�h�Ƃ��ĕԂ�
   */
  CD3DX12_SHADER_BYTECODE GetShaderBytecode() const {
    return CD3DX12_SHADER_BYTECODE(vertex_shader_->GetBufferPointer(),
                                   vertex_shader_->GetBufferSize());
  }
  /**
   * @brief ���̓��C�A�E�g��Ԃ�
   */
  D3D12_INPUT_LAYOUT_DESC GetInputLayout() const {
    return D3D12_INPUT_LAYOUT_DESC{elements_.data(),
                                   static_cast<u32>(elements_.size())};
  }

 private:
  //! �V�F�[�_�[�t�@�C��
  ComPtr<ID3DBlob> vertex_shader_;
  //! ���̓��C�A�E�g
  std::vector<D3D12_INPUT_ELEMENT_DESC> elements_;
};

}  // namespace shader
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_SHADER_VERTEX_SHADER_H_

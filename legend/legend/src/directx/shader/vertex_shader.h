#ifndef LEGEND_DIRECTX_SHADER_VERTEX_SHADER_H_
#define LEGEND_DIRECTX_SHADER_VERTEX_SHADER_H_

/**
 * @file vertex_shader.h
 * @brief ���_�V�F�[�_�[��`
 */

#include "src/directx/directx_accessor.h"
#include "src/directx/shader/shader_base.h"

namespace legend {
namespace directx {
namespace shader {
/**
 * @brief ���_�V�F�[�_�[
 */
class VertexShader final : public ShaderBase {
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
  bool Init(IDirectXAccessor& accessor, const std::filesystem::path& filepath,
            const std::vector<D3D12_INPUT_ELEMENT_DESC>& elements);
  /**
   * @brief ������
   * @param accessor DirectX12�f�o�C�X�A�N�Z�T
   * @param filepath �V�F�[�_�[�t�@�C���p�X
   * @return �������ɐ���������true��Ԃ�
   * @details
   * �V�F�[�_�[���t���N�V�����ɂ���ē��̓��C�A�E�g����͂���̂œǂݎ��Ȃ���ނ����������̓��̓G�������g�𒼐ړn���ق����g�p����
   */
  bool Init(IDirectXAccessor& accessor,
            const std::filesystem::path& filepath) override;
  /**
   * @brief ���̓��C�A�E�g��Ԃ�
   */
  D3D12_INPUT_LAYOUT_DESC GetInputLayout() const {
    return D3D12_INPUT_LAYOUT_DESC{elements_.data(),
                                   static_cast<u32>(elements_.size())};
  }

 private:
  //! ���̓��C�A�E�g
  std::vector<D3D12_INPUT_ELEMENT_DESC> elements_;
  //! �V�F�[�_�[���t���N�V�����f�[�^
  ComPtr<ID3D12ShaderReflection> shader_refrection_;
};

}  // namespace shader
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_SHADER_VERTEX_SHADER_H_

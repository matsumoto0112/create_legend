#ifndef LEGEND_DIRECTX_SHADER_PIXEL_SHADER_H_
#define LEGEND_DIRECTX_SHADER_PIXEL_SHADER_H_

/**
 * @file pixel_shader.h
 * @brief �s�N�Z���V�F�[�_�[��`
 */

#include "src/directx/directx12_device.h"

namespace legend {
namespace directx {
namespace shader {

/**
 * @brief �s�N�Z���V�F�[�_�[
 */
class PixelShader {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  PixelShader();
  /**
   * @brief �f�X�g���N�^
   */
  ~PixelShader();
  /**
   * @brief ������
   * @param device DirectX12�f�o�C�X
   * @param filepath �t�@�C���p�X
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init(DirectX12Device& device, const std::filesystem::path& filepath);

  /**
   * @brief �V�F�[�_�[�R�[�h��Ԃ�
   */
  CD3DX12_SHADER_BYTECODE GetShaderBytecode() const {
    return CD3DX12_SHADER_BYTECODE{pixel_shader_data.data(), pixel_shader_data.size()};
  }

 private:
  //! �s�N�Z���V�F�[�_�[�t�@�C��
  std::vector<u8> pixel_shader_data;
};

}  // namespace shader
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_SHADER_PIXEL_SHADER_H_

#ifndef LEGEND_DIRECTX_SHADER_SHADER_BASE_H_
#define LEGEND_DIRECTX_SHADER_SHADER_BASE_H_

/**
 * @file shader_base.h
 * @brief �V�F�[�_�[���N���X��`
 */

#include "src/directx/device/directx_accessor.h"

namespace legend {
namespace directx {
namespace shader {

/**
 * @class ShaderBase
 * @brief �V�F�[�_�[���N���X
 */
class ShaderBase {
 public:
  /**
   * @brief �R���X�g���N�^
   * @return
   */
  ShaderBase(){};
  /**
   * @brief �f�X�g���N�^
   */
  virtual ~ShaderBase() = default;
  /**
   * @brief ������
   * @param accessor DirectX�f�o�C�X�A�N�Z�T
   * @param filepath �t�@�C���ւ̃p�X
   * @return �������ɐ���������true��Ԃ�
   */
  virtual bool Init(device::IDirectXAccessor& accessor,
                    const std::filesystem::path& filepath);
  /**
   * @brief �V�F�[�_�[�R�[�h�Ƃ��ĕԂ�
   */
  CD3DX12_SHADER_BYTECODE GetShaderBytecode() const {
    return CD3DX12_SHADER_BYTECODE{shader_code_.data(), shader_code_.size()};
  }

 protected:
  //! �V�F�[�_�[�R�[�h
  std::vector<u8> shader_code_;
};

}  // namespace shader
}  // namespace directx
}  // namespace legend
#endif  //! LEGEND_DIRECTX_SHADER_SHADER_BASE_H_

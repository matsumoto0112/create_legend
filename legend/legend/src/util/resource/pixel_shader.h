#ifndef LEGEND_UTIL_RESOURCE_PIXEL_SHADER_H_
#define LEGEND_UTIL_RESOURCE_PIXEL_SHADER_H_

/**
 * @file pixel_shader.h
 * @brief �s�N�Z���V�F�[�_�[���\�[�X�Ǘ��N���X��`
 */

#include "src/directx/shader/pixel_shader.h"
#include "src/util/resource/resource_manager.h"

namespace legend {
namespace util {
namespace resource {

/**
 * @class PixelShader
 * @brief �s�N�Z���V�F�[�_�[���\�[�X�Ǘ��N���X
 */
class PixelShader
    : public ResourceManager<std::shared_ptr<directx::shader::PixelShader>> {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  PixelShader();
  /**
   * @brief �f�X�g���N�^
   */
  virtual ~PixelShader();
  /**
   * @brief ���\�[�X��ǂݍ���
   * @param name ���\�[�X��
   * @return �ǂݍ��݂ɐ���������true��Ԃ�
   */
  bool Load(const std::wstring& name) override;
};

}  // namespace resource
}  // namespace util
}  // namespace legend

#endif  //! LEGEND_UTIL_RESOURCE_PIXEL_SHADER_H_

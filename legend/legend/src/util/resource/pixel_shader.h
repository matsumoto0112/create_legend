#ifndef LEGEND_UTIL_RESOURCE_PIXEL_SHADER_H_
#define LEGEND_UTIL_RESOURCE_PIXEL_SHADER_H_

/**
 * @file pixel_shader.h
 * @brief �s�N�Z���V�F�[�_�[���\�[�X�Ǘ��N���X��`
 */

#include "src/directx/shader/pixel_shader.h"
#include "src/util/resource/resource_id.h"
#include "src/util/resource/resource_manager.h"

namespace legend {
namespace util {
namespace resource {

/**
 * @class PixelShader
 * @brief �s�N�Z���V�F�[�_�[���\�[�X�Ǘ��N���X
 */
class PixelShader final
    : public ResourceManager<id::PixelShader,
                             std::shared_ptr<directx::shader::PixelShader>> {
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
   * @brief ���\�[�X��ǂݍ���
   * @param key ���\�[�X����ӂɓ��肷��ID
   * @param filepath �t�@�C���ւ̃p�X
   * @return �ǂݍ��݂ɐ���������true��Ԃ�
   */
  bool Load(id::PixelShader, const std::filesystem::path& filepath) override;
};

}  // namespace resource
}  // namespace util
}  // namespace legend
#endif  //! LEGEND_UTIL_RESOURCE_PIXEL_SHADER_H_

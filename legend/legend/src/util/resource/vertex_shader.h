#ifndef LEGEND_UTIL_RESOURCE_VERTEX_SHADER_H_
#define LEGEND_UTIL_RESOURCE_VERTEX_SHADER_H_

/**
 * @file vertex_shader.h
 * @brief ���_�V�F�[�_�[���\�[�X�Ǘ��N���X��`
 */

#include "src/directx/shader/vertex_shader.h"
#include "src/util/resource/resource_manager.h"

namespace legend {
namespace util {
namespace resource {

/**
 * @enum VertexShaderID
 * @brief ���_�V�F�[�_�[����ӂɓ��肷��ID
 */
enum class VertexShaderID {
  MODEL_VIEW,
  SPRITE,
  MULTI_RENDER_TARGET,
  MULTI_RENDER_TARGET_POST_PROCESS,
  OBB,
};

/**
 * @class VertexShader
 * @brief ���_�V�F�[�_�[���\�[�X�Ǘ��N���X
 */
class VertexShader final
    : public ResourceManager<VertexShaderID,
                             std::shared_ptr<directx::shader::VertexShader>> {
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
   * @brief ���\�[�X��ǂݍ���
   * @param key ���\�[�X����ӂɓ��肷��ID
   * @param filepath �t�@�C���ւ̃p�X
   * @return �ǂݍ��݂ɐ���������true��Ԃ�
   */
  bool Load(VertexShaderID key, const std::filesystem::path& filepath) override;
};
}  // namespace resource
}  // namespace util
}  // namespace legend

#endif  //! LEGEND_UTIL_RESOURCE_VERTEX_SHADER_H_

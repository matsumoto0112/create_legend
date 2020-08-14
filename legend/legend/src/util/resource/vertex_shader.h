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
 * @class VertexShader
 * @brief ���_�V�F�[�_�[���\�[�X�Ǘ��N���X
 */
class VertexShader
    : public ResourceManager<std::shared_ptr<directx::shader::VertexShader>> {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  VertexShader();
  /**
   * @brief �f�X�g���N�^
   */
  virtual ~VertexShader();
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

#endif  //! LEGEND_UTIL_RESOURCE_VERTEX_SHADER_H_

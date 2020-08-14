#ifndef LEGEND_UTIL_RESOURCE_GEOMETRY_SHADER_H
#define LEGEND_UTIL_RESOURCE_GEOMETRY_SHADER_H

/**
 * @file geometry_shader.h
 * @brief �W�I���g���V�F�[�_�[�̃��\�[�X�Ǘ���`
 */

#include "src/directx/shader/geometry_shader.h"
#include "src/util/resource/resource_manager.h"

namespace legend {
namespace util {
namespace resource {

/**
 * @class GeometryShader
 * @brief �W�I���g���V�F�[�_�[�̃��\�[�X�Ǘ��N���X
 */
class GeometryShader
    : public ResourceManager<std::shared_ptr<directx::shader::GeometryShader>> {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  GeometryShader();
  /**
   * @brief �f�X�g���N�^
   */
  virtual ~GeometryShader();
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
#endif  //! LEGEND_UTIL_RESOURCE_GEOMETRY_SHADER_H

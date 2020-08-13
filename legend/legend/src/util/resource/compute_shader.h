#ifndef LEGEND_UTIL_RESOURCE_COMPUTE_SHADER_H
#define LEGEND_UTIL_RESOURCE_COMPUTE_SHADER_H
/**
 * @file compute_shader.h
 * @brief
 * �R���s���[�g�V�F�[�_�[�̃��\�[�X�Ǘ��N���X��`
 */

#include "src/directx/shader/compute_shader.h"
#include "src/util/resource/resource_manager.h"

namespace legend {
namespace util {
namespace resource {

/**
 * @class GeometryShader
 * @brief �R���s���[�g�V�F�[�_�[�̃��\�[�X�Ǘ��N���X
 */
class ComputeShader
    : public ResourceManager<std::shared_ptr<directx::shader::ComputeShader>> {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  ComputeShader();
  /**
   * @brief �f�X�g���N�^
   */
  virtual ~ComputeShader();
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

#endif  //! LEGEND_UTIL_RESOURCE_COMPUTE_SHADER_H

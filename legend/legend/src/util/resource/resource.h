#ifndef LEGEND_UTIL_RESOURCE_RESOURCE_H_
#define LEGEND_UTIL_RESOURCE_RESOURCE_H_

/**
 * @file resource.h
 * @brief ���\�[�X�Ǘ��N���X��`
 */

#include "src/util/resource/pipeline.h"
#include "src/util/resource/pixel_shader.h"
#include "src/util/resource/vertex_shader.h"

namespace legend {
namespace util {
namespace resource {

/**
 * @class Resource
 * @brief ���\�[�X�Ǘ��N���X
 */
class Resource {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  Resource();
  /**
   * @brief �f�X�g���N�^
   */
  ~Resource();
  /**
   * @brief ������
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init();
  /**
   * @brief �s�N�Z���V�F�[�_�[���擾����
   */
  resource::PixelShader& GetPixelShader() { return pixel_shader_; }
  /**
   * @brief ���_�V�F�[�_�[���擾����
   */
  resource::VertexShader& GetVertexShader() { return vertex_shader_; }
  /**
   * @brief �p�C�v���C�����擾����
   */
  resource::Pipeline& GetPipeline() { return pipeline_; }

 private:
  //! �s�N�Z���V�F�[�_�[�Ǘ�
  resource::PixelShader pixel_shader_;
  //! ���_�V�F�[�_�[�Ǘ�
  resource::VertexShader vertex_shader_;
  //! �p�C�v���C���Ǘ�
  resource::Pipeline pipeline_;
};

}  // namespace resource
}  // namespace util
}  // namespace legend

#endif  //! LEGEND_UTIL_RESOURCE_RESOURCE_H_

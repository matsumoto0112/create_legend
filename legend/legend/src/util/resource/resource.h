#ifndef LEGEND_UTIL_RESOURCE_RESOURCE_H_
#define LEGEND_UTIL_RESOURCE_RESOURCE_H_

/**
 * @file resource.h
 * @brief ���\�[�X�Ǘ��N���X��`
 */

#include "src/util/loader/archive_loader.h"
#include "src/util/resource/compute_shader.h"
#include "src/util/resource/geometry_shader.h"
#include "src/util/resource/model.h"
#include "src/util/resource/pipeline.h"
#include "src/util/resource/pixel_shader.h"
#include "src/util/resource/texture.h"
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
  virtual ~Resource();
  /**
   * @brief ������
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init();
  loader::ArchiveLoader& GetArchiveLoader() { return archive_loader_; }

  /**
   * @brief �s�N�Z���V�F�[�_�[���擾����
   */
  resource::PixelShader& GetPixelShader() { return pixel_shader_; }
  /**
   * @brief ���_�V�F�[�_�[���擾����
   */
  resource::VertexShader& GetVertexShader() { return vertex_shader_; }
  /**
   * @brief �W�I���g���V�F�[�_�[���擾����
   */
  resource::GeometryShader& GetGeometryShader() { return geometry_shader_; }
  /**
   * @brief �R���s���[�g�V�F�[�_�[���擾����
   */
  resource::ComputeShader& GetComputeShader() { return compute_shader_; }
  /**
   * @brief �p�C�v���C�����擾����
   */
  resource::Pipeline& GetPipeline() { return pipeline_; }
  /**
   * @brief ���f�����擾����
   */
  resource::Model& GetModel() { return model_; }
  /**
   * @brief �e�N�X�`�����擾����
   */
  resource::Texture& GetTexture() { return texture_; }

 private:
  loader::ArchiveLoader archive_loader_;
  //! �s�N�Z���V�F�[�_�[�Ǘ�
  resource::PixelShader pixel_shader_;
  //! ���_�V�F�[�_�[�Ǘ�
  resource::VertexShader vertex_shader_;
  //! �W�I���g���V�F�[�_�[�Ǘ�
  resource::GeometryShader geometry_shader_;
  //! �R���s���[�g�V�F�[�_�[�Ǘ�
  resource::ComputeShader compute_shader_;
  //! �p�C�v���C���Ǘ�
  resource::Pipeline pipeline_;
  //! ���f���Ǘ�
  resource::Model model_;
  //! �e�N�X�`���Ǘ�
  resource::Texture texture_;
};

}  // namespace resource
}  // namespace util
}  // namespace legend

#endif  //! LEGEND_UTIL_RESOURCE_RESOURCE_H_

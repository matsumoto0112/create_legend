#ifndef LEGEND_UTIL_RESOURCE_PIPELINE_H_
#define LEGEND_UTIL_RESOURCE_PIPELINE_H_

/**
 * @file pipeline.h
 */

#include "src/directx/shader/graphics_pipeline_state.h"
#include "src/util/resource/resource_manager.h"

namespace legend {
namespace util {
namespace resource {

/**
 * @brief �p�C�v���C������ӂɓ��肷��ID
 */
enum class PipelineID {
  MODEL_VIEW,
};

class Pipeline
    : public ResourceManager<
          PipelineID, std::shared_ptr<directx::shader::GraphicsPipelineState>> {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  Pipeline();
  /**
   * @brief �f�X�g���N�^
   */
  ~Pipeline();
  /**
   * @brief ������
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init() override;
  /**
   * @brief ���\�[�X���t�@�C���p�X����ǂݍ��ށi���쐬�j
   * @return ���false��Ԃ��B�����Register�֐����g�p���邱�ƁB
   * @details csv�ȂǂŊO������p�C�v���C�����\�z�ł���悤�ɂ�����
   */
  bool Load(PipelineID key, const std::filesystem::path& filepath) override {
    return false;
  }
  bool Unload(PipelineID key) override;
};

}  // namespace resource
}  // namespace util
}  // namespace legend
#endif  //! LEGEND_UTIL_RESOURCE_PIPELINE_H_

#ifndef LEGEND_UTIL_RESOURCE_PIPELINE_H_
#define LEGEND_UTIL_RESOURCE_PIPELINE_H_

/**
 * @file pipeline.h
 * @brief �p�C�v���C�����\�[�X�Ǘ��N���X��`
 */

#include "src/directx/shader/graphics_pipeline_state.h"
#include "src/util/resource/resource_manager.h"

namespace legend {
namespace util {
namespace resource {

/**
 * @enum PipelineID
 * @brief �p�C�v���C������ӂɓ��肷��ID
 */
enum class PipelineID {
  MODEL_VIEW,
  MULTI_RENDER_TARGET_PRE,
  MULTI_RENDER_TARGET_POST_PROCESS,
};

/**
 * @class Pipeline
 * @brief �p�C�v���C�����\�[�X�Ǘ��N���X
 */
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
   * @brief ���\�[�X���t�@�C���p�X����ǂݍ��ށi���쐬�j
   * @return ���false��Ԃ��B�����Register�֐����g�p���邱�ƁB
   * @details csv�ȂǂŊO������p�C�v���C�����\�z�ł���悤�ɂ�����
   */
  bool Load(PipelineID key, const std::filesystem::path& filepath) override {
    MY_ASSERTION(
        false,
        L"resource::Pipeline::Load���g�p���Ȃ��ł��������B\n�����resource::"
        L"Pipeline::Register���g�p���Ă��������B");
    return false;
  }
};

}  // namespace resource
}  // namespace util
}  // namespace legend
#endif  //! LEGEND_UTIL_RESOURCE_PIPELINE_H_

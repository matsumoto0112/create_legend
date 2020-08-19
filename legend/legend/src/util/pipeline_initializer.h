#ifndef LEGEND_UTIL_PIPELINE_INITIALIZER_H
#define LEGEND_UTIL_PIPELINE_INITIALIZER_H

/**
 * @file pipeline_initializer.h
 * @brief ����̃A�v���P�[�V�����Ŏg�p����p�C�v���C���̏������@�\��`
 */

namespace legend {
namespace util {

/**
 * @brief �p�C�v���C���̏������@�\�N���X
 */
class PipelineInitializer {
 public:
  /**
   * @brief ������
   * @param filepath �p�C�v���C���f�[�^��`�t�@�C����assets����̑��΃p�X
   */
  static bool Init(const std::filesystem::path& filepath);
};

}  // namespace util
}  // namespace legend
#endif  //! LEGEND_UTIL_PIPELINE_INITIALIZER_H

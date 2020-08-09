#ifndef LEGEND_STAGE_GENERATE_STAGE_GENERATOR_H_
#define LEGEND_STAGE_GENERATE_STAGE_GENERATOR_H_

namespace legend {
namespace stage_generate {

/**
 * @file stage_generator.h
 * @brief �X�e�[�W�f�[�^��ǂݍ��ݐ�������N���X
 */

class StageGenerator {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  StageGenerator();
  /**
   * @brief �f�X�g���N�^
   */
  ~StageGenerator();
  /**
   * @brief �ǂݍ��ݏ���
   * @return �������������I��������true��Ԃ�
   */
  std::vector<std::string> LoadStageData(std::wstring filepath);

 private:
};
}  // namespace stage_generate
}  // namespace legend

#endif  //! LEGEND_STAGE_GENERATE_STAGEGENERATOR_H_

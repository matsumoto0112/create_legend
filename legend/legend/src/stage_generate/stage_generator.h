#ifndef LEGEND_STAGE_GENERATE_STAGE_GENERATOR_H_
#define LEGEND_STAGE_GENERATE_STAGE_GENERATOR_H_

#include "src/enemy/enemy_manager.h"
#include "src/object/desk.h"
#include "src/player/player.h"
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
   * @brief �ǂݍ���&��������
   * @return �������������I��������true��Ԃ�
   */
  bool LoadStage(std::filesystem::path filepath, object::Desk* desk,
                 player::Player* player, enemy::EnemyManager* enemy_manager);
  /**
   * @brief �e�L�X�g�̓ǂݍ��ݏ���
   * @return �������������I��������ǂݍ��܂ꂽ���e(string)��Ԃ�
   */
  std::vector<std::string> LoadStringStageData(std::filesystem::path filepath);
  /**
   * @brief �X�e�[�W�̐ݒ菈��
   * @return �������������I��������true��Ԃ�
   */
  bool SetMapActors(std::vector<std::string> indexs, object::Desk* desk,
                    player::Player* player, enemy::EnemyManager* enemy_manager);

 private:
  /**
   * @brief String��float�ɕϊ�����
   * @paramstrings ��������
   * @return �ϊ����ꂽfloat
   */
  float String_2_Float(const std::string& string);
  /**
   * @brief String��Vector3�ɕϊ�����
   * @param x,y,z ��������
   * @return �ϊ����ꂽVector3
   */
  math::Vector3 String_2_Vector3(const std::string& x, const std::string& y,
                                 const std::string& z);
  /**
   * @brief String��Vector3�ɕϊ�����
   * @param x,y,z ��������
   * @return �ϊ����ꂽVector3
   */
  util::Transform String_2_Transform(
      const std::string& posx, const std::string& posy, const std::string& posz,
      const std::string& eularx, const std::string& eulary,
      const std::string& eularz, const std::string& sclaex,
      const std::string& sclaey, const std::string& sclaez);
  /**
   * @brief String��String�̃��X�g�֕ϊ�
   * @param x,y,z ��������
   * @return �ϊ����ꂽString�̃��X�g
   */
  std::vector<std::string> StringSplit(const std::string& string, char border);

 private:
};
}  // namespace stage_generate
}  // namespace legend

#endif  //! LEGEND_STAGE_GENERATE_STAGEGENERATOR_H_
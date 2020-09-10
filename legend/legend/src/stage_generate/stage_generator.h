#ifndef LEGEND_STAGE_GENERATE_STAGE_GENERATOR_H_
#define LEGEND_STAGE_GENERATE_STAGE_GENERATOR_H_

#include "src/enemy/enemy_manager.h"
#include "src/object/desk.h"
#include "src/object/graffiti.h"
#include "src/object/obstacle.h"
#include "src/player/player.h"
#include "src/skill/skill_item_box.h"
#include "src/stage_generate/camera_generate_info.h"

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
   * @brief �e�L�X�g�̓ǂݍ��ݏ���
   * @return �������������I��������ǂݍ��܂ꂽ���e(string)��Ԃ�
   */
  std::vector<std::string> LoadStringStageData(std::filesystem::path filepath,
                                               const std::string map_name);
  /**
   * @brief �X�e�[�W�̐ݒ菈��
   * @return �������������I��������true��Ԃ�
   */
  bool GetMapActors(
      const i32 turn_count, player::Player::InitializeParameter& player,
      std::vector<object::Desk::InitializeParameter>& desks,
      std::vector<object::Obstacle::InitializeParameter>& obstacles,
      std::vector<object::GraffitiInitializeParameter>& graffities,
      std::vector<skill::SkillItemBox::InitializeParameter>& itemboxes,
      std::vector<enemy::Enemy::InitializeParameter>& enemys,
      std::vector<enemy::Boss::InitializeParameter>& bosses);

  bool GetResultActors(
      player::Player::InitializeParameter& player,
      std::vector<object::Desk::InitializeParameter>& desks,
      std::vector<object::Obstacle::InitializeParameter>& obstacles,
      std::vector<enemy::Enemy::InitializeParameter>& enemys,
      std::vector<enemy::Boss::InitializeParameter>& bosses);

  std::vector<enemy::Enemy::InitializeParameter> GetEnemyParameters(
      const i32 turn_count);

  std::vector<enemy::Boss::InitializeParameter> GetBossParameters(
      const i32 turn_count);

  std::vector<CameraGenerateInfo> GetCameraGenerateInfos() const;

  i32 GetBossGenerateTurn();

 private:
  /**
   * @brief String��float�ɕϊ�����
   * @paramstrings ��������
   * @return �ϊ����ꂽfloat
   */
  float String_2_Float(const std::string& string) const;
  /**
   * @brief String��Vector3�ɕϊ�����
   * @param x,y,z ��������
   * @return �ϊ����ꂽVector3
   */
  math::Vector3 String_2_Vector3(const std::string& x, const std::string& y,
                                 const std::string& z) const;
  /**
   * @brief String��Vector3�ɕϊ�����
   * @param x,y,z ��������
   * @return �ϊ����ꂽVector3
   */
  util::Transform String_2_Transform(
      const std::string& posx, const std::string& posy, const std::string& posz,
      const std::string& eularx, const std::string& eulary,
      const std::string& eularz, const std::string& sclaex,
      const std::string& sclaey, const std::string& sclaez) const;
  /**
   * @brief String��String�̃��X�g�֕ϊ�
   * @param x,y,z ��������
   * @return �ϊ����ꂽString�̃��X�g
   */
  std::vector<std::string> StringSplit(const std::string& string,
                                       char border) const;

 private:
  std::vector<std::string> indexs_;
  std::string map_name_;
};
}  // namespace stage_generate
}  // namespace legend

#endif  //! LEGEND_STAGE_GENERATE_STAGEGENERATOR_H_

#include "stage_generator.h"

#include <stdio.h>

#include <sstream>

#include "src\\stdafx.h"

namespace legend {
namespace stage_generate {

//�R���X�g���N�^
StageGenerator::StageGenerator() {}

//�f�X�g���N�^
StageGenerator::~StageGenerator() {}

/*bool StageGenerator::LoadStage(
    std::filesystem::path filepath, const std::string map_name,
    system::PhysicsField* physics_field,
    std::vector<actor::Actor<physics::BoundingBox>>* actors,
    enemy::EnemyManager* enemy_manager)*/
bool StageGenerator::LoadStage(std::filesystem::path filepath,
                               const std::string map_name,
                               system::PhysicsField* physics_field,
                               std::vector<object::Desk>* desks,
                               std::vector<object::Obstacle>* obstacles,
                               player::Player* player,
                               enemy::EnemyManager* enemy_manager) {
  //�e�L�X�g�f�[�^��ǂݍ���
  std::vector<std::string> indexs = LoadStringStageData(filepath);

  //�e�A�N�^�[�𐶐�
  // return SetMapActors(map_name, indexs, physics_field, actors,
  // enemy_manager);
  return SetMapActors(map_name, indexs, physics_field, desks, obstacles, player,
                      enemy_manager);
}

//�t�@�C���̓ǂݍ��ݏ���
std::vector<std::string> StageGenerator::LoadStringStageData(
    std::filesystem::path filepath) {
  ////�t�@�C���p�X����o�C�i���t�@�C����ǂݍ���
  // std::fstream fstream;
  // fstream.open(filepath, std::ios::binary | std::ios::out);
  //
  // while (!fstream.eof()) {
  //    char* text;
  //    fstream.read((char*)&text, sizeof(char*));
  //}

  //�t�@�C���p�X����e�L�X�g�f�[�^��ǂݍ���
  std::ifstream ifstream(filepath);
  std::vector<std::string> indexs;
  if (ifstream.fail()) {
    MY_LOG(L"�X�e�[�W�f�[�^���J���܂���ł����B");
    indexs[0] = "error";
    return indexs;
  }
  std::string index;
  i32 counter = 0;
  while (std::getline(ifstream, index)) {
    indexs.push_back(index);
  }

  return indexs;
}

// bool StageGenerator::SetMapActors(
//    const std::string map_name, const std::vector<std::string>& indexs,
//    system::PhysicsField* physics_field,
//    std::vector<actor::Actor<physics::BoundingBox>>* actors,
//    enemy::EnemyManager* enemy_manager)
bool StageGenerator::SetMapActors(const std::string map_name,
                                  const std::vector<std::string>& indexs,
                                  system::PhysicsField* physics_field,
                                  std::vector<object::Desk>* desks,
                                  std::vector<object::Obstacle>* obstacles,
                                  player::Player* player,
                                  enemy::EnemyManager* enemy_manager) {
  bool is_all_ok = true;

  for (auto&& index : indexs) {
    //������𕪊�
    std::vector<std::string> infomation = StringSplit(index, ',');

    //�{���͔w�iID�Ȃǂ�ǂݍ��ނ����݂͖���
    if (infomation[0] == map_name) continue;

    // Transform��ǂݍ���(scale�͌��󖳎�)
    util::Transform transform = String_2_Transform(
        infomation[1], infomation[2], infomation[3], infomation[4],
        infomation[5], infomation[6], "1", "1", "1");

    //���̏�����
    if (infomation[0] == "floor") {
      object::Desk::InitializeParameter parameter;
      parameter.transform = transform;
      parameter.bounding_box_length = math::Vector3(1.2f, 0.05f, 0.8f) / 4.0f;
      parameter.normal = math::Vector3::kUpVector;

      auto& desk = desks->emplace_back();

      if (!desk.Init(parameter)) {
        MY_LOG(L"���̐����Ɏ��s���܂����B");
        is_all_ok = false;
        continue;
      }
      physics_field->AddObstacle(desk.GetCollisionRef());
      continue;
    }

    //�v���C���[�̐���
    if (infomation[0] == "startpoint") {
      player::Player::InitializeParameter parameter;
      parameter.transform = transform;
      parameter.bouding_box_length = math::Vector3(0.06f, 0.025f, 0.14f) / 4.0f;
      parameter.min_power = 0;
      parameter.max_power = 1;

      if (!player->Init(parameter)) {
        MY_LOG(L"�v���C���[�̐����Ɏ��s���܂���");
        is_all_ok = false;
        continue;
      }
      //physics_field->AddObstacle(player->GetCollisionRef());
      continue;
    }

    //�G�̐���
    if (infomation[0] == "enemy") {
      enemy::Enemy::InitializeParameter parameter;
      parameter.transform = transform;
      parameter.bouding_box_length =
          math::Vector3(0.06f, 0.025f, 0.14f) / 4.0f;
      enemy_manager->Add(parameter, physics_field);
      continue;
    }

    //��Q���̐���
    if (infomation[0] == "obstacle") {
      object::Obstacle::InitializeParameter parameter;
      parameter.position = transform.GetPosition();
      parameter.rotation = transform.GetRotation();
      parameter.model_id = 0;
      parameter.bounding_box_length =
          math::Vector3(0.06f, 0.025f, 0.14f) / 4.0f;

      auto& obstacle = obstacles->emplace_back();

      if (!obstacle.Init(parameter)) {
        MY_LOG(L"��Q���̐����Ɏ��s���܂����B");
        is_all_ok = false;
        continue;
      }
      physics_field->AddObstacle(obstacle.GetCollisionRef());
      continue;
    }
  }

  return is_all_ok;
}

float StageGenerator::String_2_Float(const std::string& string) {
  return std::stof(string.c_str());
}

math::Vector3 StageGenerator::String_2_Vector3(const std::string& x,
                                               const std::string& y,
                                               const std::string& z) {
  float fx = String_2_Float(x);
  float fy = String_2_Float(y);
  float fz = String_2_Float(z);

  return math::Vector3(fx, fy, fz);
}

util::Transform StageGenerator::String_2_Transform(
    const std::string& posx, const std::string& posy, const std::string& posz,
    const std::string& eularx, const std::string& eulary,
    const std::string& eularz, const std::string& sclaex,
    const std::string& sclaey, const std::string& sclaez) {
  //�e�v�f��Vector3��
  math::Vector3 position = String_2_Vector3(posx, posy, posz);
  math::Vector3 eular = String_2_Vector3(eularx, eulary, eularz);
  math::Vector3 scale = String_2_Vector3(sclaex, sclaey, sclaez);
  return util::Transform(position, math::Quaternion::FromEular(eular), scale);
}

std::vector<std::string> StageGenerator::StringSplit(const std::string& string,
                                                     char border) {
  std::vector<std::string> elements;
  std::stringstream ss(string);
  std::string item;

  while (std::getline(ss, item, border)) {
    if (item.empty()) continue;

    elements.push_back(item);
  }

  return elements;
}

}  // namespace stage_generate
}  // namespace legend
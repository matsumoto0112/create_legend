#include "stage_generator.h"

#include <stdio.h>

#include <sstream>

#include "src\\stdafx.h"

namespace legend {
namespace stage_generate {

//コンストラクタ
StageGenerator::StageGenerator() {}

//デストラクタ
StageGenerator::~StageGenerator() {}

/*bool StageGenerator::LoadStage(
    std::filesystem::path filepath, const std::string map_name,
    system::PhysicsField* physics_field,
    std::vector<actor::Actor<physics::BoundingBox>>* actors,
    enemy::EnemyManager* enemy_manager)*/
bool StageGenerator::LoadStage(
    std::filesystem::path filepath, const std::string map_name,
    player::Player::InitializeParameter& player,
    std::vector<object::Desk::InitializeParameter>& desks,
    std::vector<object::Obstacle::InitializeParameter>& obstacles,
    std::vector<object::GraffitiInitializeParameter>& graffities,
    std::vector<skill::SkillItemBox::InitializeParameter>& itemboxes) {
  //テキストデータを読み込み
  indexs_ = LoadStringStageData(filepath);
  map_name_ = map_name;

  //各アクターを生成
  // return SetMapActors(map_name, indexs, physics_field, actors,
  // enemy_manager);
  return SetMapActors(player, desks, obstacles, graffities, itemboxes);
}

//ファイルの読み込み処理
std::vector<std::string> StageGenerator::LoadStringStageData(
    std::filesystem::path filepath) {
  ////ファイルパスからバイナリファイルを読み込む
  // std::fstream fstream;
  // fstream.open(filepath, std::ios::binary | std::ios::out);
  //
  // while (!fstream.eof()) {
  //    char* text;
  //    fstream.read((char*)&text, sizeof(char*));
  //}

  //ファイルパスからテキストデータを読み込み
  std::ifstream ifstream(filepath);
  std::vector<std::string> indexs;
  if (ifstream.fail()) {
    MY_LOG(L"ステージデータを開けませんでした。");
    indexs.push_back("error");
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
bool StageGenerator::SetMapActors(
    player::Player::InitializeParameter& player,
    std::vector<object::Desk::InitializeParameter>& desks,
    std::vector<object::Obstacle::InitializeParameter>& obstacles,
    std::vector<object::GraffitiInitializeParameter>& graffities,
    std::vector<skill::SkillItemBox::InitializeParameter>& itemboxes) {
  bool is_all_ok = true;

  if (indexs_.empty() || indexs_[0] == "error") {
    MY_LOG(L"データが読み込まれていないか、読み込みに失敗しています。");
    return false;
  }

  for (auto&& index : indexs_) {
    //文字列を分割
    std::vector<std::string> infomation = StringSplit(index, ',');

    //本来は背景IDなどを読み込むが現在は無視
    if (infomation[0] == map_name_) continue;

    // Transformを読み込み(scaleは現状無視)
    util::Transform transform =
        String_2_Transform(infomation[1], infomation[2], infomation[3],
                           infomation[4], infomation[5], infomation[6],
                           infomation[7], infomation[8], infomation[9]);

    //机の初期化
    if (infomation[0] == "floor") {
      object::Desk::InitializeParameter parameter;
      parameter.transform = transform;
      parameter.bounding_box_length = math::Vector3(120.0f, 5.0f, 80.0f) / 4.0f;
      parameter.normal = math::Vector3::kUpVector;
      desks.emplace_back(parameter);
      continue;
    }

    //プレイヤーの生成
    if (infomation[0] == "startpoint") {
      player::Player::InitializeParameter parameter;
      parameter.transform = transform;
      parameter.transform.SetPosition(parameter.transform.GetPosition() +
                                      math::Vector3(0.0f, 10.0f, 0.0f));
      parameter.bouding_box_length = math::Vector3(6.0f, 2.5f, 14.0f) / 4.0f;
      parameter.min_power = 0;
      parameter.max_power = 1;
      parameter.max_strength = 3;
      parameter.min_strength = 0.5f;
      player = parameter;
      continue;
    }

    //障害物の生成
    if (infomation[0] == "obstacle") {
      object::Obstacle::InitializeParameter parameter;
      parameter.position = transform.GetPosition();
      parameter.rotation = transform.GetRotation();
      parameter.model_id = 0;
      parameter.bounding_box_length = math::Vector3(6.0f, 2.5f, 14.0f) / 4.0f * 3.0f;
      obstacles.emplace_back(parameter);

      continue;
    }

    if (infomation[0] == "graffiti") {
      object::GraffitiInitializeParameter parameter;
      parameter.transform = transform;
      parameter.bounding_box_length = math::Vector3(4.0f, 2.0f, 4.0f);
      parameter.remaining_graffiti = 100.0f;
      graffities.emplace_back(parameter);

      continue;
    }

    if (infomation[0] == "Stationery") {
        skill::SkillItemBox::InitializeParameter parameter;
        parameter.transform = transform;
        parameter.bounding_box_length = math::Vector3(2.0f, 2.0f, 2.0f);
        itemboxes.emplace_back(parameter);
    }
  }

  return is_all_ok;
}

std::vector<enemy::Enemy::InitializeParameter>
StageGenerator::GetEnemyParameters(const i32 turn_count) {
  std::vector<enemy::Enemy::InitializeParameter> enemy_parameters;

  if (indexs_.empty() || indexs_[0] == "error") {
    MY_LOG(L"データが読み込まれていないか、読み込みに失敗しています。");
    return enemy_parameters;
  }

  for (auto&& index : indexs_) {
    //文字列を分割
    std::vector<std::string> infomation = StringSplit(index, ',');

    //本来は背景IDなどを読み込むが現在は無視
    if (infomation[0] == map_name_) continue;

    //敵の生成
    if (infomation[0] == "enemy") {
      if ((int)String_2_Float(infomation[15]) != turn_count) continue;

      enemy::Enemy::InitializeParameter parameter;

      // Transformを読み込み(scaleは現状無視)
      parameter.transform =
          String_2_Transform(infomation[1], infomation[2], infomation[3],
                             infomation[4], infomation[5], infomation[6],
                             infomation[7], infomation[8], infomation[9]);
      parameter.transform.SetPosition(parameter.transform.GetPosition() +
                                      math::Vector3(0.0f, 10.0f, 0.0f));
      parameter.bouding_box_length = math::Vector3(6.0f, 2.5f, 14.0f) / 4.0f;
      enemy_parameters.push_back(parameter);
    }
  }
  return enemy_parameters;
}

std::vector<enemy::Boss::InitializeParameter> StageGenerator::GetBossParameters(
    const i32 turn_count) {
  std::vector<enemy::Boss::InitializeParameter> boss_parameters;

  if (indexs_.empty() || indexs_[0] == "error") {
    MY_LOG(L"データが読み込まれていないか、読み込みに失敗しています。");
    return boss_parameters;
  }

  for (auto&& index : indexs_) {
    //文字列を分割
    std::vector<std::string> infomation = StringSplit(index, ',');

    //本来は背景IDなどを読み込むが現在は無視
    if (infomation[0] == map_name_) continue;

    //敵の生成
    if (infomation[0] == "boss") {
      if ((int)String_2_Float(infomation[15]) != turn_count) continue;

      enemy::Boss::InitializeParameter parameter;
      math::Vector3 scale = math::Vector3::kUnitVector * 1.25f;

      // Transformを読み込み(scaleは現状無視)
      parameter.transform =
          String_2_Transform(infomation[1], infomation[2], infomation[3],
                             infomation[4], infomation[5], infomation[6],
                             infomation[7], infomation[8], infomation[9]);
      parameter.transform.SetPosition(parameter.transform.GetPosition() +
                                      math::Vector3(0.0f, 10.0f, 0.0f));
      parameter.transform.SetScale(scale);
      parameter.bouding_box_length =
          math::Vector3(6.0f, 2.5f, 14.0f) / 4.0f * 1.25f;
      boss_parameters.push_back(parameter);
    }
  }
  return boss_parameters;
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
  //各要素をVector3へ
  math::Vector3 position = String_2_Vector3(posx, posy, posz);
  math::Vector3 eular =
      String_2_Vector3(eularx, eulary, eularz) * math::util::DEG_2_RAD;
  eular.y += math::util::DEG_2_RAD * 180;
  math::Vector3 scale = String_2_Vector3("1", "1", "1");
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
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

bool StageGenerator::LoadStage(std::filesystem::path filepath, object::Desk* desk, player::Player* player, enemy::EnemyManager* enemy_manager)
{
    //テキストデータを読み込み
    std::vector<std::string> indexs = LoadStringStageData(filepath);

    return SetMapActors(indexs, desk, player, enemy_manager);
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

bool StageGenerator::SetMapActors(std::vector<std::string> indexs,
                                  object::Desk* desk, player::Player* player,
                                  enemy::EnemyManager* enemy_manager) {
  for (auto&& index : indexs) {
    std::vector<std::string> infomation = StringSplit(index, ',');

    if (infomation[0] == "aaaa") continue;

    util::Transform transform =
        String_2_Transform(infomation[1], infomation[2], infomation[3],
                           infomation[4], infomation[5], infomation[6],
                           "1", "1", "1");

    //プレイヤーの初期化
    if (infomation[0] == "startpoint") {
      player::Player::InitializeParameter player_parameter;
      player_parameter.transform = transform;
      player_parameter.bouding_box_length = math::Vector3(0.1f, 0.05f, 0.2f);
      player_parameter.min_power = 0;
      player_parameter.max_power = 1;
      if (!player->Init(player_parameter)) {
        MY_LOG(L"プレイヤーの初期化に失敗しました。");
      }
    }

    //机の初期化
    if (infomation[0] == "floor") {
        object::Desk::InitializeParameter desk_parameter;
        desk_parameter.transform = transform;
        desk_parameter.bounding_box_length = math::Vector3(0.3f, 0.05f, 0.2f);
        desk_parameter.normal = math::Vector3::kUpVector;
        if (!desk->Init(desk_parameter)) {
            MY_LOG(L"机の初期化に失敗しました。");
        }
    }

    //if (infomation[0] == "enemy") {
    //    enemy_manager->Add()
    //}
  }

  return true;
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
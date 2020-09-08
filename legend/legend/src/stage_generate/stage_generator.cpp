#include "stage_generator.h"

#include <stdio.h>

#include <sstream>

#include "src/util/resource/resource_names.h"
#include "src\\stdafx.h"

namespace legend {
namespace stage_generate {

//コンストラクタ
StageGenerator::StageGenerator() {}

//デストラクタ
StageGenerator::~StageGenerator() {}

//ファイルの読み込み処理
std::vector<std::string> StageGenerator::LoadStringStageData(
    std::filesystem::path filepath, const std::string map_name) {
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

  map_name_ = map_name;
  indexs_ = indexs;

  return indexs;
}

bool StageGenerator::GetMapActors(
    const i32 turn_count, player::Player::InitializeParameter& player,
    std::vector<object::Desk::InitializeParameter>& desks,
    std::vector<object::Obstacle::InitializeParameter>& obstacles,
    std::vector<object::GraffitiInitializeParameter>& graffities,
    std::vector<skill::SkillItemBox::InitializeParameter>& itemboxes,
    std::vector<enemy::Enemy::InitializeParameter>& enemys,
    std::vector<enemy::Boss::InitializeParameter>& bosses) {
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
    if (infomation[0] == "floor" && turn_count == 0) {
      object::Desk::InitializeParameter parameter;
      parameter.transform = transform;
      parameter.bounding_box_length = math::Vector3(30.0f, 1.25f, 20.0f);
      parameter.normal = math::Vector3::kUpVector;
      desks.emplace_back(parameter);
      continue;
    }

    //プレイヤーの生成
    if (infomation[0] == "startpoint" && turn_count == 0) {
      player::Player::InitializeParameter parameter;
      parameter.transform = transform;
      parameter.transform.SetPosition(parameter.transform.GetPosition() +
                                      math::Vector3(0.0f, 10.0f, 0.0f));
      parameter.bouding_box_length = math::Vector3(1.25f, 0.5f, 2.75f);
      parameter.min_power = 0;
      parameter.max_power = 1;
      parameter.max_strength = 3;
      parameter.min_strength = 0.5f;
      player = parameter;
      continue;
    }

    //障害物の生成
    if (infomation[0] == "obstacle" && turn_count == 0) {
      object::Obstacle::InitializeParameter parameter;
      parameter.position = transform.GetPosition();
      parameter.rotation = transform.GetRotation();
      parameter.model_id = 0;
      parameter.bounding_box_length = math::Vector3(4.5f, 2.5f, 10.0f);
      obstacles.emplace_back(parameter);

      continue;
    }

    //ラクガキの生成
    if (infomation[0] == "graffiti" && turn_count == 0) {
      object::GraffitiInitializeParameter parameter;
      parameter.transform = transform;
      parameter.bounding_box_length = math::Vector3(3.25f, 0.25f, 3.25f);
      parameter.remaining_graffiti = 100.0f;
      parameter.can_erase_speed = 0.3f;
      parameter.model_id = (int)String_2_Float(infomation[13]);
      graffities.emplace_back(parameter);

      continue;
    }

    //文房具の生成
    if (infomation[0] == "stationery" &&
        (i32)String_2_Float(infomation[15]) == turn_count) {
      skill::SkillItemBox::InitializeParameter parameter;
      parameter.transform = transform;
      parameter.bounding_box_length = math::Vector3::kUnitVector * 1.5f;
      auto& resource = game::GameDevice::GetInstance()->GetResource();
      parameter.skill_model_num = static_cast<i32>(String_2_Float(infomation[14]));
      itemboxes.emplace_back(parameter);
      continue;
    }
	
  //* -----------------------------------------------
    //* infomation[0] : 名前
    //* infomation[1] : ポジション_X
    //* infomation[2] : ポジション_Y
    //* infomation[3] : ポジション_Z
    //* infomation[4] : ローテーション_X
    //* infomation[5] : ローテーション_Y
    //* infomation[6] : ローテーション_Z
    //* infomation[7] : スケール_X
    //* infomation[8] : スケール_Y
    //* infomation[9] : スケール_Z
    //* infomation[10]: おもさ
    //* infomation[11]: 摩擦
    //* infomation[12]: 反発力
    //* infomation[13]: モデルID
    //* infomation[14]: AI_ID
    //* infomation[15]: ターン数
    //* -----------------------------------------------

    //敵の生成
    if (infomation[0] == "enemy" &&
        (int)String_2_Float(infomation[15]) == turn_count) {
      enemy::Enemy::InitializeParameter parameter;

      parameter.transform = transform;
      parameter.transform.SetPosition(parameter.transform.GetPosition() +
                                      math::Vector3(0.0f, 10.0f, 0.0f));
      parameter.bouding_box_length = math::Vector3(1.25f, 0.5f, 2.75f);
      parameter.mass = String_2_Float(infomation[10]);
      parameter.friction = String_2_Float(infomation[11]);
      parameter.restitution = String_2_Float(infomation[12]);
      parameter.model_id = (int)String_2_Float(infomation[13]);
      parameter.type_index = (int)String_2_Float(infomation[14]);
      enemys.push_back(parameter);
      continue;
    }

    //ボスの生成
    if (infomation[0] == "boss" &&
        (int)String_2_Float(infomation[15]) == turn_count) {
      enemy::Boss::InitializeParameter parameter;
      math::Vector3 scale = math::Vector3::kUnitVector * 1.25f;

      parameter.transform = transform;
      parameter.transform.SetPosition(parameter.transform.GetPosition() +
                                      math::Vector3(0.0f, 10.0f, 0.0f));
      parameter.transform.SetScale(scale);
      parameter.bouding_box_length = math::Vector3(3.0f, 1.0f, 5.5f);
      parameter.mass = String_2_Float(infomation[10]);
      parameter.friction = String_2_Float(infomation[11]);
      parameter.restitution = String_2_Float(infomation[12]);
      parameter.model_id = (int)String_2_Float(infomation[13]);
      parameter.type_index = (int)String_2_Float(infomation[14]);
      bosses.push_back(parameter);
      continue;
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

std::vector<CameraGenerateInfo> StageGenerator::GetCameraGenerateInfos() const {
  std::vector<CameraGenerateInfo> res;
  for (auto&& index : indexs_) {
    //文字列を分割
    std::vector<std::string> infomation = StringSplit(index, ',');
    if (infomation[0] != "camera") continue;

    //とりあえず座標だけ
    CameraGenerateInfo info;
    info.position =
        String_2_Vector3(infomation[1], infomation[2], infomation[3]);
    res.emplace_back(info);
  }

  return res;
}

float StageGenerator::String_2_Float(const std::string& string) const {
  return std::stof(string.c_str());
}

math::Vector3 StageGenerator::String_2_Vector3(const std::string& x,
                                               const std::string& y,
                                               const std::string& z) const {
  float fx = String_2_Float(x);
  float fy = String_2_Float(y);
  float fz = String_2_Float(z);

  return math::Vector3(fx, fy, fz);
}

util::Transform StageGenerator::String_2_Transform(
    const std::string& posx, const std::string& posy, const std::string& posz,
    const std::string& eularx, const std::string& eulary,
    const std::string& eularz, const std::string& sclaex,
    const std::string& sclaey, const std::string& sclaez) const {
  //各要素をVector3へ
  math::Vector3 position = String_2_Vector3(posx, posy, posz);
  math::Vector3 eular =
      String_2_Vector3(eularx, eulary, eularz) * math::util::DEG_2_RAD;
  eular.y += math::util::DEG_2_RAD * 180;
  math::Vector3 scale = String_2_Vector3("1", "1", "1");
  return util::Transform(position, math::Quaternion::FromEular(eular), scale);
}

std::vector<std::string> StageGenerator::StringSplit(const std::string& string,
                                                     char border) const {
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
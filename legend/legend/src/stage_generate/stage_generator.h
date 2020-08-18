#ifndef LEGEND_STAGE_GENERATE_STAGE_GENERATOR_H_
#define LEGEND_STAGE_GENERATE_STAGE_GENERATOR_H_

#include "src/enemy/enemy_manager.h"
#include "src/object/desk.h"
#include "src/object/graffiti.h"
#include "src/object/obstacle.h"
#include "src/player/player.h"
#include "src/system/physics_field.h"

namespace legend {
namespace stage_generate {

/**
 * @file stage_generator.h
 * @brief ステージデータを読み込み生成するクラス
 */

class StageGenerator {
 public:
  /**
   * @brief コンストラクタ
   */
  StageGenerator();
  /**
   * @brief デストラクタ
   */
  ~StageGenerator();

  /**
   * @brief 読み込み&生成処理
   * @return 処理が正しく終了したらtrueを返す
   */
  bool LoadStage(std::filesystem::path filepath, const std::string map_name,
                 std::vector<object::Desk>* desks,
                 std::vector<object::Obstacle>* obstacles,
                 player::Player* player,
                 std::vector<object::Graffiti>* graffities);
  /**
   * @brief テキストの読み込み処理
   * @return 処理が正しく終了したら読み込まれた内容(string)を返す
   */
  std::vector<std::string> LoadStringStageData(std::filesystem::path filepath);
  /**
   * @brief ステージの設定処理
   * @return 処理が正しく終了したらtrueを返す
   */
  bool SetMapActors(std::vector<object::Desk>* desks,
                    std::vector<object::Obstacle>* obstacles,
                    player::Player* player,
                    std::vector<object::Graffiti>* graffities);

  std::vector<enemy::Enemy::InitializeParameter> GetEnemyParameters(
      const i32 turn_count);

  std::vector<enemy::Boss::InitializeParameter> GetBossParameters(
      const i32 turn_count);

 private:
  /**
   * @brief Stringをfloatに変換する
   * @paramstrings 元文字列
   * @return 変換されたfloat
   */
  float String_2_Float(const std::string& string);
  /**
   * @brief StringをVector3に変換する
   * @param x,y,z 元文字列
   * @return 変換されたVector3
   */
  math::Vector3 String_2_Vector3(const std::string& x, const std::string& y,
                                 const std::string& z);
  /**
   * @brief StringをVector3に変換する
   * @param x,y,z 元文字列
   * @return 変換されたVector3
   */
  util::Transform String_2_Transform(
      const std::string& posx, const std::string& posy, const std::string& posz,
      const std::string& eularx, const std::string& eulary,
      const std::string& eularz, const std::string& sclaex,
      const std::string& sclaey, const std::string& sclaez);
  /**
   * @brief StringをStringのリストへ変換
   * @param x,y,z 元文字列
   * @return 変換されたStringのリスト
   */
  std::vector<std::string> StringSplit(const std::string& string, char border);

 private:
  std::vector<std::string> indexs_;
  std::string map_name_;
};
}  // namespace stage_generate
}  // namespace legend

#endif  //! LEGEND_STAGE_GENERATE_STAGEGENERATOR_H_

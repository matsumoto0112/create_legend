#ifndef LEGEND_SCENES_DEBUGSCENE_STAGE_GENERATE_TEST_H_
#define LEGEND_SCENES_DEBUGSCENE_STAGE_GENERATE_TEST_H_

/**
 * @brief ステージ生成テスト
 */

#include "src/camera/perspective_camera.h"
#include "src/enemy/enemy_manager.h"
#include "src/object/desk.h"
#include "src/object/obstacle.h"
#include "src/player/player.h"
#include "src/scenes/scene.h"
#include "src/stage_generate/stage_generator.h"
#include "src/system/physics_field.h"

namespace legend {
namespace scenes {
namespace debugscene {
/**
 * @class SoundTest
 * @brief ステージ生成テストシーン
 */
class StageGenerateTest : public Scene {
 public:
  /**
   * @brief コンストラクタ
   */
  StageGenerateTest(ISceneChange* scene_change);
  /**
   * @brief デストラクタ
   */
  ~StageGenerateTest();
  /**
   * @brief 初期化
   */
  bool Initialize() override;
  /**
   * @brief 更新
   */
  bool Update() override;
  /**
   * @brief 描画
   */
  void Draw() override;

  /**
   * @brief 終了処理
   */
  void Finalize() override;

 private:
  //! ステージ生成クラス
  std::unique_ptr<stage_generate::StageGenerator> stage_generator_;
  //! 読み込んだステージデータ格納用
  std::vector<std::string> indexs_;
  //! 物理管理クラス
  system::PhysicsField physics_field_;
  //! メインカメラ
  camera::PerspectiveCamera camera_;
  //! アクター格納リスト
  //std::vector<actor::Actor<physics::BoundingBox>> map_actors_;
  //! プレイヤー
  player::Player player_;
  //! 机リスト
  std::vector<object::Desk> desks_;
  //! 障害物リスト
  std::vector<object::Obstacle> obstacles_;
  //! 敵管理システム
  enemy::EnemyManager enemy_manager_;
};

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_DEBUGSCENE_STAGE_GENERATE_TEST_H_

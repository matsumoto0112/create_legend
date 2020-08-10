#ifndef LEGEND_SCENES_DEBUGSCENE_STAGE_GENERATE_TEST_H_
#define LEGEND_SCENES_DEBUGSCENE_STAGE_GENERATE_TEST_H_

/**
 * @brief ステージ生成テスト
 */

#include "src/camera/perspective_camera.h"
#include "src/object/desk.h"
#include "src/player/player.h"
#include "src/scenes/scene.h"
#include "src/stage_generate/stage_generator.h"
#include "src/enemy/enemy_manager.h"

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
  //! メインカメラ
  camera::PerspectiveCamera camera_;
  //! プレイヤー
  player::Player player_;
  //! 机
  object::Desk desk_;
  //! 敵管理システム
  enemy::EnemyManager enemy_manager_;
};

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_DEBUGSCENE_STAGE_GENERATE_TEST_H_

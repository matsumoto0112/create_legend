#ifndef LEGEND_SCENES_DEBUGSCENE_STAGE_GENERATE_TEST_H_
#define LEGEND_SCENES_DEBUGSCENE_STAGE_GENERATE_TEST_H_

/**
 * @brief ステージ生成テスト
 */

#include "src/scenes/scene.h"

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
};

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_DEBUGSCENE_STAGE_GENERATE_TEST_H_

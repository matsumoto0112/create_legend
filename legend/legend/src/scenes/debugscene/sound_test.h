#ifndef LEGEND_SCENES_DEBUGSCENE_SOUND_TEST_H_
#define LEGEND_SCENES_DEBUGSCENE_SOUND_TEST_H_

/**
 * @brief サウンドテストシーン
 */

#include "src/scenes/scene.h"

namespace legend {
namespace scenes {
namespace debugscene {
/**
 * @class SoundTest
 * @brief モデル描画テストシーン
 */
class SoundTest : public Scene {
 public:
  /**
   * @brief コンストラクタ
   */
  SoundTest(ISceneChange* scene_change);
  /**
   * @brief デストラクタ
   */
  ~SoundTest();
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

#endif  //! LEGEND_SCENES_DEBUGSCENE_SOUND_TEST_H_

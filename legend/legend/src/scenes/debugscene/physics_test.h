#ifndef LEGEND_SCENES_DEBUGSCENE_PHYSICS_TEST_H_
#define LEGEND_SCENES_DEBUGSCENE_PHYSICS_TEST_H_

#include "src/scenes/scene.h"

namespace legend {
namespace scenes {
namespace debugscene {

/**
 * @class PhysicsTest
 * @brief 物理関係のテストシーン
 */
class PhysicsTest : public Scene {
 public:
  /**
   * @brief コンストラクタ
   */
  PhysicsTest(ISceneChange* scene_change);
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
};

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_DEBUGSCENE_PHYSICS_TEST_H_

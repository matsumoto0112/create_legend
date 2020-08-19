#ifndef LEGEND_SCENES_DEBUGSCENE_BULLET_TEST_H_
#define LEGEND_SCENES_DEBUGSCENE_BULLET_TEST_H_

/**
 * @file bullet_test.h
 * @brief bulletテストシーン
 */

#include "src/bullet/bounding_box.h"
#include "src/bullet/physics_field.h"
#include "src/scenes/scene.h"

namespace legend {
namespace scenes {
namespace debugscene {

class BulletTest : public Scene {
 public:
  /**
   * @brief コンストラクタ
   */
  BulletTest(ISceneChange* scene_change);
  /**
   * @brief デストラクタ
   */
  ~BulletTest();
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
  bullet::PhysicsField physics_field_;
};
}  // namespace debugscene
}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_DEBUGSCENE_BULLET_TEST_H_

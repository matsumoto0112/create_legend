#ifndef LEGEND_SCENES_MODEL_VIEW_H_
#define LEGEND_SCENES_MODEL_VIEW_H_

#include "src/scenes/scene.h"

namespace legend {
namespace scenes {

/**
 * @class ModelView
 * @brief モデル描画テストシーン
 */
class ModelView : public Scene {
 public:
  /**
   * @brief コンストラクタ
   */
  ModelView(ISceneChange* scene_change);
  /**
   * @brief 初期化
   */
  void Initialize() override;
  /**
   * @brief 更新
   */
  void Update() override;
  /**
   * @brief 描画
   */
  void Draw() override;
};

}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_MODEL_VIEW_H_

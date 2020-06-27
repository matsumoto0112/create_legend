#ifndef LEGEND_SCENES_SCENE_H_
#define LEGEND_SCENES_SCENE_H_

#include "src/scenes/scene_change.h"
#include "src/scenes/task.h"

namespace legend {
namespace scenes {

/**
 * @class Scene
 * @brief シーンの基底クラス
 */
class Scene : public Task {
 protected:
  ISceneChange* scene_change_;

 public:
  /**
   * @brief コンストラクタ
   */
  Scene(ISceneChange* scene_change);
  /**
   * @brief 仮想デストラクタ
   */
  virtual ~Scene() {}
  /**
   * @brief 初期化
   */
  virtual void Initialize() override {}
  /**
   * @brief 終了
   */
  virtual void Finalize() override;
  /**
   * @brief 更新
   */
  virtual void Update() override {}
  /**
   * @brief 描画
   */
  virtual void Draw() override;
};

}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_SCENE_H_

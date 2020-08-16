#ifndef LEGEND_SCENES_TITLE_H_
#define LEGEND_SCENES_TITLE_H_

#include "src/scenes/scene.h"

namespace legend {
namespace scenes {

/**
 * @class Title
 * @brief タイトルシーン
 */
class Title : public Scene {
 public:
  /**
   * @brief コンストラクタ
   */
  Title(ISceneChange* scene_change);
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

  void Finalize() override;

 private:
  i32 bgm_key_;
};

}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_TITLE_H_

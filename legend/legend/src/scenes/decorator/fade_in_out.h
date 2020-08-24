#ifndef LEGEND_SCENES_DECORATOR_FADE_IN_OUT_H
#define LEGEND_SCENES_DECORATOR_FADE_IN_OUT_H

/**
 * @file fade_in_out.h
 * @brief
 */

#include "src/draw/sprite_2d.h"
#include "src/scenes/scene.h"

namespace legend {
namespace scenes {
namespace decorator {
class FadeInOut : public Scene {
 public:
  /**
   * @brief コンストラクタ
   * @param scene_change シーン変更インターフェース
   * @param inner 内部で管理するシーン
   */
  FadeInOut(ISceneChange* scene_change, std::unique_ptr<Scene> inner);
  /**
   * @brief デストラクタ
   */
  virtual ~FadeInOut();
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
   * @brief 終了
   */
  void Finalize() override;

 private:
  //! 内部管理クラス
  std::unique_ptr<Scene> inner_;
  //! ポーズ中に表示する画像
  draw::Sprite2D pause_image_;
};

}  // namespace decorator
}  // namespace scenes
}  // namespace legend
#endif  //! LEGEND_SCENES_DECORATOR_FADE_IN_OUT_H

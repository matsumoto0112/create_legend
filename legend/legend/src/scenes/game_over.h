#ifndef LEGEND_SCENES_GAME_OVER_H_
#define LEGEND_SCENES_GAME_OVER_H_

#include "src/scenes/fade_in_out.h"
#include "src/scenes/scene.h"
#include "src/ui/ui_board.h"

namespace legend {
namespace scenes {

/**
 *@class GameOver
 *@brief ゲームオーバーシーン
 */
class GameOver : public Scene {
 public:
  /**
   *@brief コンストラクタ
   */
  GameOver(ISceneChange* scene_change);
  /**
   * @brief デストラクタ
   */
  virtual ~GameOver();
  /**
   *@brief 初期化
   */
  bool Initialize() override;
  /**
   *@brief 更新
   */
  bool Update() override;
  /**
   *@brief 描画
   */
  void Draw() override;
  /**
   * @brief 終了処理
   */
  void Finalize() override;

 private:
  ui::UIBoard board_;

  FadeInOut fade_;
  bool is_scene_end_;
};
}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_GAME_OVER_H_

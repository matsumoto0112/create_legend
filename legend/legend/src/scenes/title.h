#ifndef LEGEND_SCENES_TITLE_H_
#define LEGEND_SCENES_TITLE_H_

#include "src/scenes/fade_in_out.h"
#include "src/scenes/scene.h"
#include "src/ui/image.h"
#include "src/ui/ui_board.h"

namespace legend {
namespace scenes {

/**
 * @class Title
 * @brief タイトルシーン
 */
class Title : public Scene {
  enum class Phase {
    TITLE,
    STAGE_SELECT,
    END,
  };
  enum class StageSelectMoveDirection {
    NONE,
    LEFT,
    RIGHT,
  };

  struct StageImageInfo {
    ui::UIComponent* component;
    float base_x;
    StageImageInfo(ui::UIComponent* comp, float base_x)
        : component(comp), base_x(base_x){};
  };

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
  ui::UIBoard board_;
  FadeInOut fade_;
  bool is_scene_end_;

  Phase current_phase_;
  std::vector<ui::UIComponent*> title_images_;
  std::vector<StageImageInfo> stage_images_;
  StageSelectMoveDirection stage_select_move_direction_;
};

}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_TITLE_H_

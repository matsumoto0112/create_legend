#ifndef LEGEND_SCENES_TITLE_H_
#define LEGEND_SCENES_TITLE_H_

#include "src/scenes/fade_in_out.h"
#include "src/scenes/scene.h"
#include "src/ui/image.h"
#include "src/ui/ui_board.h"
#include "src/util/mod_int.h"

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
    float prev_x;
    float next_x;
    StageImageInfo(ui::UIComponent* comp, float base_x, float next_x)
        : component(comp), base_x(base_x), next_x(next_x){};
  };

  struct Stage {
    std::string stage_name;
    const wchar_t* stage_image_name;
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
  /**
   * @brief 終了
   */
  void Finalize() override;

 private:
  static const std::vector<Stage> STAGE_LIST;
  static constexpr float STAGE_ITEM_MOVE_TIME = 0.5f;

 private:
  //! BGM
  i32 bgm_key_;
  //! UI
  ui::UIBoard board_;
  //! フェード
  FadeInOut fade_;
  //! シーン終了フラグ
  bool is_scene_end_;

  Phase current_phase_;
  std::vector<ui::UIComponent*> title_images_;
  std::vector<ui::UIComponent*> stage_non_movable_images_;
  std::vector<StageImageInfo> stage_movable_images_;
  StageSelectMoveDirection stage_select_move_direction_;
  util::ModInt current_select_stage_item_id_;
  util::CountDownTimer stage_move_select_timer_;

  //! SEが鳴ったか
  bool is_play_se_;
};

}  // namespace scenes
}  // namespace legend

#endif  //! LEGEND_SCENES_TITLE_H_

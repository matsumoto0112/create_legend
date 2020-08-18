#ifndef LEGEND_SKILL_SKILL_SELECT_UI_H_
#define LEGEND_SKILL_SKILL_SELECTUI_H_

#include "src/draw/sprite_renderer.h"
#include "src/draw/texture_string.h"
#include "src/skill/skill.h"

/**
 * @file skill_select_ui.h
 * @brief スキル選択UIクラス
 */
namespace legend {
namespace skill {

class SkillSelectUI {
 public:
  /**
   * @brief コンストラクタ
   */
  SkillSelectUI();
  /**
   * @brief デストラクタ
   */
  ~SkillSelectUI();
  /**
   * @brief 初期化処理
   */
  void Init();
  /**
   * @brief 更新処理
   */
  void Update();
  /**
   * @brief 描画処理
   */
  void Draw();
  /**
   * @brief 追加処理
   */
  void AddSkill(/*const Skill* skill*/);
  /**
   * @brief 削除処理
   */
  void RemoveSkillUI(i32 index_num);

 private:
  math::Vector2 icon_base_position_;
  math::Vector2 icon_scale_;

  std::vector<draw::Sprite2D> skill_icons_;
  draw::Sprite2D skill_explanatory_;

  bool is_select_mode_;
  i32 select_number_;
};
}  // namespace skill
}  // namespace legend

#endif  //! LEGEND_SKILL_SKILL_SELECT_UI_H_

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
  void AddSkill(const Skill* skill);
  /**
   * @brief 削除処理
   */
  void RemoveSkillUI(i32 index_num);
  /**
   * @brief 選択中かを取得
   */
  bool GetIsSelectMode() const;
  /**
   * @brief 選択モード切替
   */
  void ChangeIsSelectMode();
  /**
   * @brief 選択中のスキル番号の取得
   */
  i32 GetSkillNumber();
  /**
   * @brief 選択中のスキル番号の更新
   */
  void SelectSkillNumber(i32 select_number);

 private:
  //! スキルアイコンの基本座標
  math::Vector2 icon_base_position_;
  //! スキルアイコンのスケーリング
  math::Vector2 icon_scale_;

  //! 描画するスキルアイコンのリスト
  std::vector<draw::Sprite2D> skill_icons_;
  //! スキルの説明画像
  draw::Sprite2D skill_explanatory_;
  //! スキルの選択中フレーム画像
  draw::Sprite2D skill_select_frame_;

  //! スキル選択モードか
  bool is_select_mode_;
  //! 選択番号
  i32 select_number_;
};
}  // namespace skill
}  // namespace legend

#endif  //! LEGEND_SKILL_SKILL_SELECT_UI_H_

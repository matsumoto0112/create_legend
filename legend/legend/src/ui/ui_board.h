#ifndef LEGEND_UI_UI_BOARD_H
#define LEGEND_UI_UI_BOARD_H

/**
 * @file ui_board.h
 * @brief UI表示用ボード定義
 */

#include "src/ui/ui_component.h"

namespace legend {
namespace ui {

/**
 * @brief UI表示用ボード
 * @details
 * このオブジェクトにUIコンポーネントを追加していき、後でまとめて描画する
 */
class UIBoard {
 public:
  /**
   * @brief コンストラクタ
   */
  UIBoard();
  /**
   * @brief デストラクタ
   */
  virtual ~UIBoard();
  /**
   * @brief コンポーネントをリセットする
   */
  void Reset();
  /**
   * @brief コンポーネントを追加し、そのポインタを返す
   */
  UIComponent* AddComponent(std::unique_ptr<UIComponent> component);
  /**
   * @コンポーネントの描画
   */
  void Draw();

 private:
  //! コンポーネント配列
  std::vector<std::unique_ptr<UIComponent>> ui_components_;
};

}  // namespace ui
}  // namespace legend
#endif  //! LEGEND_UI_UI_BOARD_H

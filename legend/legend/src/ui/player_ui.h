#ifndef LEGEND_UI_PLAYER_UI_H_
#define LEGEND_UI_PLAYER_UI_H_

#include "src/draw/sprite_renderer.h"
#include "src/draw/texture_string.h"
#include "src/skill/skill.h"

namespace legend {
namespace ui {
class PlayerUI {
 public:
  /**
   * @brief コンストラクタ
   */
  PlayerUI();
  /**
   * @brief デストラクタ
   */
  virtual ~PlayerUI();
  /**
   * @brief 初期化
   */
  void Init();
  /**
   * @brief 更新
   */
  void Update();
  /**
   * @brief 描画
   */
  void Draw();
  /**
   * @brief 画像追加
   */
  void AddEquipmentUI(const skill::Skill* skill);
  /**
   * @brief 画像削除
   */
  void RemoveEquipmentUI(i32 remove_num);

 private:
  //! プレイヤー画像
  draw::Sprite2D player_sprite_;
  //! 装備スキル画像
  std::vector<draw::Sprite2D> equipment_sprites_;
};
}  // namespace ui
}  // namespace legend

#endif  //! LEGEND_UI_PLAYER_UI_H_

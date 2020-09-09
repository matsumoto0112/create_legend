#ifndef LEGEND_TURN_CHANGE_TURN_CHANGE_H_
#define LEGEND_TURN_CHANGE_TURN_CHANGE_H_
#include "src/draw/sprite_renderer.h"
#include "src/math/vector_2.h"
#include "src/math/math_util.h"
#include "src/system/mode.h"

namespace legend {
namespace system {
class TurnSystem;
}  // namespace system
namespace turn_change {

class TurnChange {
 public:
     /**
      * @brief コンストラクタ
      */
  TurnChange();
  /**
   * @brief デストラクタ
   */
  ~TurnChange();
  /**
   * @brief 初期化
   */
  bool Init(system::TurnSystem* turn_system);
  /**
   * @brief ターン切り替え演出の開始
   */
  bool ChangeStart(system::Mode next_mode);
  /**
   * @brief ターン切り替え演出の更新
   */
  bool TurnChangeUpdate();
  /**
   * @brief ボス出現ゲージの更新
   */
  bool BossGenerateUIUpdate();
  /**
   * @brief 更新処理
   */
  bool Update();
  /**
   * @brief 描画処理
   */
  void Draw();

private:
    math::Vector2 LerpVector2(math::Vector2 v1, math::Vector2 v2, float t);

 private:
  //! 表示する状態かどうか
  bool is_view_;
  //! タイマー
  float timer_;
  //! ターン切り替え演出の開始までの秒数
  const float start_time_ = 0.15f;
  //! ターン切り替え演出中の秒数
  const float staging_time_ = 0.75f;
  //! ターン切り替え演出の後の秒数
  const float before_time_ = 0.4f;
  //! ウィンドウサイズ
  math::Vector2 screen_size_;
  //! ターンシステムのポインタ
  system::TurnSystem* turn_system_;
  //! 変更前のターン画像
  draw::Sprite2D before_turn_sprite_;
  //! 変更後のターン画像
  draw::Sprite2D next_turn_sprite_;
  //! 変更後のモード
  system::Mode next_mode_;
  
  //! ボスの出現するターン
  i32 boss_generate_turn_;
  //! ボス出現までのUIの位置
  math::Vector2 boss_generate_ui_center_position_;
  //! 移動前のプレイヤーアイコンの位置
  math::Vector2 before_player_icon_position_;
  //! プレイヤーのアイコン画像
  draw::Sprite2D player_icon_;
  //! ボスのアイコン画像
  draw::Sprite2D boss_icon_;
  //! ボス出現ターンまでの背景画像
  draw::Sprite2D boss_generate_ui_bg_;
};

}  // namespace turn_change
}  // namespace legend
#endif  //! LEGEND_TURN_CHANGE_TURN_CHANGE_H_

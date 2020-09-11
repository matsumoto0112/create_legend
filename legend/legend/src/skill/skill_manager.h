#ifndef LEGEND_SKILL_SKILL_MANAGER_H_
#define LEGEND_SKILL_SKILL_MANAGER_H_

#include "src/skill/skill.h"
#include "src/skill/skill_compass.h"
#include "src/skill/skill_paste_stick.h"
#include "src/skill/skill_pencil.h"
#include "src/skill/skill_select_ui.h"
#include "src/skill/skill_type.h"
#include "src/ui/player_ui.h"

/**
 * @file skill_manager.h
 * @brief スキル管理クラス
 */

namespace legend {
namespace skill {
enum class Mode {
  NONE,
  RISE_PLAYER,
  FALL_PLAYER,
};

/**
 * @brief スキルマネージャー
 */
class SkillManager {
 public:
  /**
   * @brief コンストラクタ
   */
  SkillManager();
  /**
   * @brief デストラクタ
   */
  ~SkillManager();
  /**
   * @brief 初期化処理
   */
  void Init(actor::IActorMediator* mediator, player::Player* player);
  /**
   * @brief スキルを拾った際のメソッド
   */
  void GetSkill(std::shared_ptr<Skill> skill);
  /**
   * @brief スキルの獲得処理
   */
  void AddSkill(std::shared_ptr<Skill> skill);
  /**
   * @brief 更新処理
   */
  void Update();
  /**
   * @brief 装備更新処理
   */
  void EquipmentProductionUpdate();
  /**
   * @brief プレイヤーの行動後に1度だけ呼ばれるメソッド
   */
  void PlayerTurnEnd();
  /**
   * @brief 敵の行動後に1度だけ呼ばれるメソッド
   */
  void EnemyTurnEnd();
  /**
   * @brief 描画処理
   */
  void Draw();
  /**
   * @brief 演出中かどうか
   */
  bool IsProductionNow();
  /**
   * @brief スキルリストの取得
   */
  std::vector<std::shared_ptr<Skill>> GetSkillList();
  /**
   * @brief スキルの削除
   */
  void RemoveSkill();
  /**
   * @brief スキルの選択
   */
  bool SelectSkill();
  /**
   * @brief スキルの使用
   */
  void UseSkill();
  /**
   * @brief スキル終了
   */
  void EndSkill();
  /**
   * @brief スキルの座標設定
   */
  void SetPosition(std::shared_ptr<Skill> skill, i32 skill_num);
  /**
   * @brief 選択中スキルの座標設定
   */
  void SetPositionSelectSkill(i32 skill_num);
  /**
   * @brief UIの表示切替
   */
  void ChangeEnable(bool enable);

 private:
  actor::IActorMediator* mediator_;
  //! 所持できる最大スキル数
  i32 skill_max_count_;
  //! 所持しているスキル用リスト
  std::vector<std::shared_ptr<Skill>> skills_;
  //! スキル選択のUI用クラス
  SkillSelectUI select_ui_;
  //! このターン手に入れたスキル
  std::vector<std::shared_ptr<Skill>> this_turn_get_skills_;

  //! 入力判定
  bool select_move_;
  //! プレイヤー
  player::Player* player_;
  //! 糊
  std::vector<SkillPaste*> pastes_;
  //! 装備状態UI
  ui::PlayerUI player_ui_;

  //! 現在の演出状態
  Mode current_mode_;
  //! 装備演出中か
  bool is_equipment_production_;
  //! 装備し終えたか
  bool complete_eqquipment_;
  //! 糊スキルを使ったか
  bool paste_skill_use_;
  //! 選択番号
  i32 select_skill_number_;
  //! 前の選択番号
  i32 previous_select_number_;

  //! UIの表示非表示
  bool ui_enable_;
  //! 装備演出開始のSEが鳴ったか
  bool is_play_fly_se_;
};
}  // namespace skill
}  // namespace legend

#endif  //! LEGEND_SKILL_SKILL_MANAGER_H_

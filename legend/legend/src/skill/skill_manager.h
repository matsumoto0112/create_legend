#ifndef LEGEND_SKILL_SKILL_MANAGER_H_
#define LEGEND_SKILL_SKILL_MANAGER_H_

#include "src/skill/skill.h"
#include "src/skill/skill_pencil.h"
#include "src/skill/skill_select_ui.h"
#include "src/skill/skill_type.h"

namespace legend {
namespace skill {
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
  void Init(actor::IActorMediator *mediator);
  /**
   * @brief スキルを拾った際のメソッド
   */
  void GetSkill(i32 skill_id, player::Player *player);
  /**
   * @brief スキルの獲得処理
   */
  void AddSkill(std::shared_ptr<Skill> skill);
  /**
   * @brief 更新処理
   */
  void Update();
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
   * @brief 選択したスキルの更新
   */
  void SelectUpdate();

 private:
  actor::IActorMediator *mediator_;
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
};
}  // namespace skill
}  // namespace legend

#endif  //! LEGEND_SKILL_SKILL_MANAGER_H_

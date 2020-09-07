#ifndef LEGEND_SKILL_SKILL_COMPASS_H_
#define LEGEND_SKILL_SKILL_COMPASS_H_

#include "src/skill/skill.h"

namespace legend {
namespace skill {

class SkillCompass : public Skill {
  using Parent = Skill;

 public:
  /**
   * @brief コンストラクタ
   */
  SkillCompass();
  /**
   * @brief デストラクタ
   */
  virtual ~SkillCompass();
  /**
   * @brief 初期化
   */
  bool Init(actor::IActorMediator* mediator, player::Player* player) override;
  /**
   * @brief 更新処理
   */
  bool Update() override;
  /**
   * @brief 描画処理
   */
  void Draw() override;
  /**
   * @brief スキルを選択された際のメソッド
   */
  void Use() override;
  /**
   * @brief 発動
   */
  void Action() override;
  /**
   * @brief スキル演出の更新
   */
  void ProductionUpdate() override;
  /**
   * @brief 終了
   */
  void EndAction() override;
  /**
   * @brief 衝突判定
   */
  void OnHit(bullet::Collider* other);

 private:
  //! 弾き飛ばす力
  float flip_off_power_;
  //! 経過時間
  float elapsed_time_;
};

}  // namespace skill
}  // namespace legend

#endif  //! LEGEND_SKILL_SKILL_COMPASS_H_

#ifndef LEGEND_SKILL_SKILL_PENCIL_H_
#define LEGEND_SKILL_SKILL_PENCIL_H_

#include "src/bullet/bounding_sphere.h"
#include "src/skill/explosion_pencil.h"
#include "src/skill/skill.h"
#include "src/util/timer.h"

namespace legend {
namespace skill {

class SkillPencil : public Skill {
  using Parent = Skill;

 public:
  /**
   * @brief コンストラクタ
   */
  SkillPencil();
  /**
   * @brief デストラクタ
   */
  ~SkillPencil();
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
  /**
   * @brief 爆発開始
   */
  void Explosion(util::Transform transform);
  /**
   * @brief 爆発更新
   */
  void ExplosionUpdate();

private:
    /**
     * @brief targetに向きを回転させる
     * @param 方向
     */
    math::Quaternion LookAt(const math::Vector3& target);

 private:
  //! タイマー
  util::CountDownTimer explosion_timer_;
  //! 爆発クラス
  std::shared_ptr<ExplosionPencil> explosion_pencil_;
  //! 爆発中か
  bool is_explosion_;
  //! 射出角度
  float shoot_theta_;
  math::Vector3 velocity_;
};

}  // namespace skill
}  // namespace legend
#endif  //! LEGEND_SKILL_SKILL_PENCIL_H_
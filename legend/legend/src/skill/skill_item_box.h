#ifndef LEGEND_SKILL_SKILL_ITEM_BOX_H_
#define LEGEND_SKILL_SKILL_ITEM_BOX_H_

#include "src/actor/actor.h"
#include "src/bullet/bounding_box.h"
#include "src/draw/particle/particle_emitter.h"
#include "src/skill/skill.h"
#include "src/util/timer.h"

namespace legend {
namespace skill {

class SkillItemBox : public actor::Actor {
  using Parent = actor::Actor;

 public:
  /**
   * @brief 初期化パラメータ
   */
  struct InitializeParameter {
    util::Transform transform;
    math::Vector3 bounding_box_length;
    i32 skill_icon_model_num;
  };

 public:
  /**
   * @brief コンストラクタ
   */
  SkillItemBox();
  /**
   * @brief デストラクタ
   */
  virtual ~SkillItemBox();
  /**
   * @brief 初期化
   * @param 仲介者
   * @param パラメータ
   * @param スキル
   */
  virtual bool Init(actor::IActorMediator* mediator,
                    const InitializeParameter& parameter,
                    std::shared_ptr<Skill> skill);
  /**
   * @brief 更新
   */
  bool Update() override;
  /**
   * @brief 描画
   */
  void Draw() override;

  /**
   * @brief 死亡判定の変更
   */
  void ChangeDead();
  /**
   * @brief 死亡判定の取得
   */
  bool GetIsDead() const;
  /**
   * @brief 設定されたスキルの取得
   */
  std::shared_ptr<Skill> GetSkill() const;

 private:
  //! 死亡判定
  bool is_dead_;
  //! スキル
  std::shared_ptr<Skill> skill_;
  //! 削除時間
  util::CountDownTimer delete_time_;
  //! コライダー
  std::shared_ptr<bullet::BoundingBox> box_;
  //! スキル内容のアイコンモデル
  std::shared_ptr<draw::Model> skill_icon_model_;
  directx::buffer::ConstantBuffer<
      directx::buffer::constant_buffer_structure::Transform>
      skill_icon_transform_cb_;
  //! 移動パーティクル
  std::shared_ptr<draw::particle::ParticleEmitter> move_particle_;
  //! 速度
  math::Vector3 velocity_;
};

}  // namespace skill
}  // namespace legend

#endif  //! LEGEND_SKILL_SKILL_ITEM_BOX_H_

#ifndef LEGEND_SKILL_SKILL_PASTE_H_
#define LEGEND_SKILL_SKILL_PASTE_H_

#include "src/actor/actor.h"
#include "src/bullet/bounding_box.h"

namespace legend {
namespace skill {
class SkillPaste : public actor::Actor {
  using Parent = actor::Actor;

 public:
  /**
   * @brief コンストラクタ
   */
  SkillPaste();
  /**
   * @brief デストラクタ
   */
  virtual ~SkillPaste();
  /**
   * @brief 初期化
   */
  void Init(math::Vector3 position, actor::IActorMediator* mediator);
  /**
   * @brief 更新
   */
  bool Update();
  /**
   * @brief 描画
   */
  void Draw();
  /**
   * @brief 削除
   */
  void Destroy(actor::IActorMediator* mediator);

 private:
  //! コライダー
  std::shared_ptr<bullet::BoundingBox> box_;
};
}  // namespace skill
}  // namespace legend

#endif  //! LEGEND_SKILL_SKILL_PASTE_H_

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
   * @brief �R���X�g���N�^
   */
  SkillPaste();
  /**
   * @brief �f�X�g���N�^
   */
  virtual ~SkillPaste();
  /**
   * @brief ������
   */
  void Init(math::Vector3 position, actor::IActorMediator* mediator);
  /**
   * @brief �X�V
   */
  bool Update();
  /**
   * @brief �`��
   */
  void Draw();
  /**
   * @brief �폜
   */
  void Destroy(actor::IActorMediator* mediator);

 private:
  //! �R���C�_�[
  std::shared_ptr<bullet::BoundingBox> box_;
};
}  // namespace skill
}  // namespace legend

#endif  //! LEGEND_SKILL_SKILL_PASTE_H_

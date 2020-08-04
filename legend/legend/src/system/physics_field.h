#ifndef LEGEND_SYSTEM_PHYSICS_FIELD_H_
#define LEGEND_SYSTEM_PHYSICS_FIELD_H_

#include "src/actor/actor.h"
#include "src/object/desk.h"
#include "src/player/player.h"

namespace legend {
namespace system {
namespace id {
enum class Object {
  PLAYER,
  ENEMY,
  DESK,
};
}

/**
 * @class PhysicsField
 * @brief �t�B�[���h���OBB�Ǘ��N���X
 */
class PhysicsField {
 public:
  //�d��
  static const float gravity;

 public:
  PhysicsField();
  ~PhysicsField();
  bool Update();
  void AddCollision(id::Object object_id, physics::BoundingBox add_obb);

 private:
  std::map<id::Object, physics::BoundingBox> obbs_;
};

}  // namespace system
}  // namespace legend

#endif  //! LEGEND_SYSTEM_PHYSICS_FIELD_H_

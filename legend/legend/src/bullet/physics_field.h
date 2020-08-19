#ifndef LEGEND_BULLET_PHYSICS_FIELD_H_
#define LEGEND_BULLET_PHYSICS_FIELD_H_

/**
 * @file physics_field.h
 * @brief �������Z�Ǘ��N���X
 */
#include <btBulletDynamicsCommon.h>

#include "src/bullet/collider.h"
#include "src/directx/bullet_debug_draw.h"

namespace legend {
namespace bullet {

class PhysicsField {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  PhysicsField();
  /**
   * @brief �f�X�g���N�^
   */
  ~PhysicsField();
  /**
   * @brief ����������
   */
  bool Init();
  /**
   * @brief �X�V
   */
  bool Update();
  /**
   * @brief �f�o�b�N�p�̕`��
   */
  void DebugDraw();

 private:
  std::shared_ptr<legend::directx::BulletDebugDraw> debug_drawer_;
  std::shared_ptr<btDynamicsWorld> world_;
  std::vector<std::shared_ptr<Collider>> colliders_;
};

}  // namespace bullet
}  // namespace legend

#endif  //! LEGEND_BULLET_PHYSICS_FIELD_H_

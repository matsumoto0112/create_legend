#ifndef LEGEND_PHYSICS_COLLIDER_H_
#define LEGEND_PHYSICS_COLLIDER_H_

/**
 * @file collider.h
 */

#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/buffer/constant_buffer_structure.h"
#include "src/directx/buffer/index_buffer.h"
#include "src/directx/buffer/vertex_buffer.h"
#include "src/directx/device/command_list.h"
#include "src/directx/vertex.h"
#include "src/util/transform.h"

namespace legend {
namespace physics {
class Collider {
 public:
  Collider(const util::Transform& transform, const std::wstring& name);
  virtual ~Collider();
  virtual bool Init();
  virtual void Update();
  virtual void DebugDraw(directx::device::CommandList& command_list) {}

 protected:
  std::wstring name_;
  //! トランスフォーム
  util::Transform transform_;
};

}  // namespace physics
}  // namespace legend
#endif  //! LEGEND_PHYSICS_COLLIDER_H_

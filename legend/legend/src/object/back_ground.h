#ifndef LEGEND_OBJECT_BACK_GROUND_H
#define LEGEND_OBJECT_BACK_GROUND_H

/**
 * @file back_ground.h
 * @brief
 */

#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/buffer/constant_buffer_structure.h"

namespace legend {
namespace object {
class BackGround {
 public:
  BackGround();
  ~BackGround();
  bool Init();
  void Draw();

 private:
  directx::buffer::ConstantBuffer<
      directx::buffer::constant_buffer_structure::Transform>
      transform_cb_;
};

}  // namespace object
}  // namespace legend
#endif  //! LEGEND_OBJECT_BACK_GROUND_H

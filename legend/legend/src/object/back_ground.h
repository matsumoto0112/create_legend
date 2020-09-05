#ifndef LEGEND_OBJECT_BACK_GROUND_H
#define LEGEND_OBJECT_BACK_GROUND_H

/**
 * @file back_ground.h
 * @brief
 */

#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/buffer/constant_buffer_structure.h"
#include "src/util/resource/resource_names.h"

namespace legend {
namespace object {
namespace bg_type {
enum Enum {
  LEFT,
  RIGHT,
  FORWARD,
  BACK,
  FLOOR,
  // CEIL,
  MAX
};
}  // namespace bg_type
class BackGround {
 private:
 public:
  BackGround();
  ~BackGround();
  bool Init();
  void Draw(
      const std::wstring& pipeline_name =
          util::resource::resource_names::pipeline::DIFFERED_RENDERING_PRE);

 private:
  std::array<directx::buffer::ConstantBuffer<
                 directx::buffer::constant_buffer_structure::Transform>,
             bg_type::MAX>
      transform_cbs_;
};

}  // namespace object
}  // namespace legend
#endif  //! LEGEND_OBJECT_BACK_GROUND_H

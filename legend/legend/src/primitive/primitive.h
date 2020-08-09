#ifndef LEGEND_PRIMITIVE_PRIMITIVE_H_
#define LEGEND_PRIMITIVE_PRIMITIVE_H_

/**
 * @file primitive.h
 */

#include "src/directx/buffer/constant_buffer.h"
#include "src/directx/buffer/constant_buffer_structure.h"
#include "src/directx/buffer/index_buffer.h"
#include "src/directx/buffer/vertex_buffer.h"
#include "src/directx/device/command_list.h"
#include "src/directx/vertex.h"
#include "src/util/transform.h"

namespace legend {
namespace primitive {
class PrimitiveBase {
 protected:
  using TransformStruct = directx::buffer::constant_buffer_structure::Transform;

 public:
  PrimitiveBase(const std::wstring& name);
  virtual ~PrimitiveBase();
  virtual bool Init() = 0;
  virtual void Render(directx::device::CommandList& command_list);

 protected:
  virtual bool InitBuffer(const std::vector<directx::PhysicsVertex>& vertices,
                          const std::vector<u16>& indices);

 protected:
  std::wstring name_;
  directx::buffer::VertexBuffer vertex_buffer_;
  directx::buffer::IndexBuffer index_buffer_;
  directx::buffer::ConstantBuffer<TransformStruct> transform_cb_;
  util::Transform transform_;
};

}  // namespace primitive
}  // namespace legend
#endif  //! LEGEND_PRIMITIVE_PRIMITIVE_H_

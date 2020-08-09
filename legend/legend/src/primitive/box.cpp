#include "box.h"

#include "src/primitive/primitive.h"

namespace legend {
namespace primitive {

Box::Box() : PrimitiveBase(L"") {}

Box::~Box() {}

bool Box::Init() {
  const float length = 0.5f;

  const std::vector<directx::PhysicsVertex> vertices{
      {{-length, length, -length}},   // 0
      {{length, length, -length}},    // 1
      {{length, -length, -length}},   // 2
      {{-length, -length, -length}},  // 3
      {{-length, length, length}},    // 4
      {{length, length, length}},     // 5
      {{length, -length, length}},    // 6
      {{-length, -length, length}}    // 7
  };
  const std::vector<u16> indices = {
      0, 1, 1, 2, 2, 3, 3, 0, 4, 5, 5, 6, 6, 7, 7, 4, 0, 4, 1, 5, 2, 6, 3, 7,
  };
  return InitBuffer(vertices, indices);
}

}  // namespace primitive
}  // namespace legend
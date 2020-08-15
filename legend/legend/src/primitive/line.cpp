#include "src/primitive/line.h"

namespace legend {
namespace primitive {

//�R���X�g���N�^
Line::Line() : PrimitiveBase(L"Line") {}

//�f�X�g���N�^
Line::~Line() {}

//������
bool Line::Init(directx::descriptor_heap::heap_parameter::LocalHeapID heap_id) {
  const std::vector<directx::PhysicsVertex> vertices{
      {{0.0f, 0.0f, 0.0f}},
      {{0.0f, 0.0f, 1.0f}},
  };

  const std::vector<u16> indices{0, 1};
  return InitBuffer(vertices, indices, heap_id);
}

}  // namespace primitive
}  // namespace legend

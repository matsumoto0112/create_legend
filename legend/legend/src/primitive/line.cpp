#include "src/primitive/line.h"

namespace legend {
namespace primitive {

//�R���X�g���N�^
Line::Line() : PrimitiveBase(L"Line") {}

//�f�X�g���N�^
Line::~Line() {}

//������
bool Line::Init() {
  const std::vector<directx::PhysicsVertex> vertices{
      {{0.0f, 0.0f, 0.0f}},
      {{1.0f, 0.0f, 0.0f}},
  };

  const std::vector<u16> indices{0, 1};
  return InitBuffer(vertices, indices);
}

}  // namespace primitive
}  // namespace legend

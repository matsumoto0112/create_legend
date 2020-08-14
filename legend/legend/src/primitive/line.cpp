#include "src/primitive/line.h"

namespace legend {
namespace primitive {

//コンストラクタ
Line::Line() : PrimitiveBase(L"Line") {}

//デストラクタ
Line::~Line() {}

//初期化
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

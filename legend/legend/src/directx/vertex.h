#ifndef LEGEND_DIRECTX_VERTEX_H_
#define LEGEND_DIRECTX_VERTEX_H_

/**
 * @file vertex.h
 * @brief 頂点構造体定義
 */

#include "src/math/vector_2.h"
#include "src/math/vector_3.h"

namespace legend {
namespace directx {
struct Vertex {
  math::Vector3 position;
  math::Vector2 uv;

  Vertex(const math::Vector3& position, const math::Vector2& uv)
      : position(position), uv(uv) {}
};
}  // namespace directx
}  // namespace legend

#endif  // !LEGEND_DIRECTX_VERTEX_H_

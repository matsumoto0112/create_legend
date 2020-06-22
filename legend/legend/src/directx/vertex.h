#ifndef LEGEND_DIRECTX_VERTEX_H_
#define LEGEND_DIRECTX_VERTEX_H_

/**
 * @file vertex.h
 * @brief í∏ì_ç\ë¢ëÃíËã`
 */

#include "src/math/vector_2.h"
#include "src/math/vector_3.h"

namespace dx12_lib {
namespace directx {
struct Vertex {
  math::Vector3 position;

  Vertex(const math::Vector3& position) : position(position) {}
};
}  // namespace directx
}  // namespace dx12_lib

#endif  // !LEGEND_DIRECTX_VERTEX_H_

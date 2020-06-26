#ifndef LEGEND_DIRECTX_PRIMITIVE_TOPOLOGY_H_
#define LEGEND_DIRECTX_PRIMITIVE_TOPOLOGY_H_

/**
 * @file primitive_topology.h
 * @brief プリミティブの形状定義
 */

namespace legend {
namespace directx {

/**
 * @enum PrimitiveTopology
 * @brief プリミティブの形状
 */
enum class PrimitiveTopology : u32 {
  TriangleList,
  PointList,
};
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_PRIMITIVE_TOPOLOGY_H_

#ifndef LEGEND_DIRECTX_BUFFER_CONSTANT_BUFFER_STRUCTURE_H_
#define LEGEND_DIRECTX_BUFFER_CONSTANT_BUFFER_STRUCTURE_H_

/**
 * @file constant_buffer_structure.h
 * @brief コンスタントバッファで使用する構造体定義ファイル
 */

#include "src/math/matrix_4x4.h"

namespace legend {
namespace directx {
namespace buffer {
namespace constant_buffer_structure {

/**
 * @struct Transform
 * @brief ワールド行列データ構造体
 */
struct Transform {
  //! ワールド変換行列
  math::Matrix4x4 world;
};

/**
 * @struct WorldContext
 * @brief ワールド描画用データ構造体
 */
struct WorldContext {
  //! ビュー行列
  math::Matrix4x4 view;
  //! プロジェクション行列
  math::Matrix4x4 projection;
};

/**
 * @struct UVRect
 * @brief UV矩形
 */
struct UVRect {
  float left;
  float top;
  float width;
  float height;
};

}  // namespace constant_buffer_structure
}  // namespace buffer
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_BUFFER_CONSTANT_BUFFER_STRUCTURE_H_

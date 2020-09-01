#ifndef LEGEND_STAGE_GENERATE_CAMERA_GENERATE_INFO_H
#define LEGEND_STAGE_GENERATE_CAMERA_GENERATE_INFO_H

/**
 * @file camera_generate_info.h
 * @brief カメラの生成に必要な情報の構造体定義
 */

#include "src/math/vector_3.h"

namespace legend {
namespace stage_generate {
struct CameraGenerateInfo {
  math::Vector3 position;
};
}  // namespace stage_generate
}  // namespace legend
#endif  //! LEGEND_STAGE_GENERATE_CAMERA_GENERATE_INFO_H

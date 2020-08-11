#ifndef LEGEND_DIRECTX_DEVICE_DEVICE_OPTION_H_
#define LEGEND_DIRECTX_DEVICE_DEVICE_OPTION_H_

/**
 * @file device_option.h
 * @brief
 */

#include "src/util/enum_util.h"

namespace legend {
namespace directx {
namespace device {

/**
 * @brief デバイスのオプション
 */
enum class DeviceOptionFlags {
  NONE,
  TEARING,          //! テアリングサポート
  USE_WARP_DEVICE,  //! WARPデバイスを使用する
};

namespace defines {

//! 使用するフレーム数
constexpr u32 FRAME_COUNT = 3;

//! WARPデバイスを使用するか
//! 現在特定環境下においてDeviceの不具合があるためその時はtrueにする
constexpr bool USE_WARP_DEVICE = false;

//要求する最小機能レベル
//今回のプロジェクトで要求する最小レベル
constexpr D3D_FEATURE_LEVEL MIN_FEATURE_LEVEL =
    D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;
}  // namespace defines

}  // namespace device
}  // namespace directx
}  // namespace legend

namespace legend::util::enum_util {
template <>
struct provides_bitwise_operators<directx::device::DeviceOptionFlags>
    : std::true_type {};
}  // namespace legend::util::enum_util

#endif  //! LEGEND_DIRECTX_DEVICE_DEVICE_OPTION_H_
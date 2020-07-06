#ifndef LEGEND_DIRECTX_DEVICE_DEVICE_PARAMETERS_H_
#define LEGEND_DIRECTX_DEVICE_DEVICE_PARAMETERS_H_

/**
 * @file device_parameters.h
 * @brief デバイス用パラメータ定義
 */

namespace legend {
namespace directx {
namespace device {
namespace parameters {

//テアリングサポートが有効である
constexpr u32 ALLOW_TEARING = 0x1;

//テアリングサポートを要求する
constexpr u32 REQUIRE_TEARING_SUPPORT = 0x2;

//要求する最小機能レベル
//今回のプロジェクトで要求する最小レベル
constexpr D3D_FEATURE_LEVEL MIN_FEATURE_LEVEL =
    D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;

}  // namespace parameters
}  // namespace device
}  // namespace directx
}  // namespace legend
#endif  //! LEGEND_DIRECTX_DEVICE_DEVICE_PARAMETERS_H_

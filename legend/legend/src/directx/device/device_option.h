#ifndef LEGEND_DIRECTX_DEVICE_DEVICE_OPTION_H_
#define LEGEND_DIRECTX_DEVICE_DEVICE_OPTION_H_

#include "src/util/enum_util.h"

namespace legend {
namespace directx {
namespace device {
/**
 * @brief デバイスのオプション
 */
enum class DeviceOptionFlags {
  NONE,
  TEARING,  //! テアリングサポート
};

}  // namespace device
}  // namespace directx
}  // namespace legend

namespace legend::util::enum_util {
template <>
struct provides_bitwise_operators<directx::device::DeviceOptionFlags>
    : std::true_type {};
}  // namespace legend::util::enum_util

#endif  //! LEGEND_DIRECTX_DEVICE_DEVICE_OPTION_H_
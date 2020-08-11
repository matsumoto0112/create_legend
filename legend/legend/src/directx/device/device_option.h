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
 * @brief �f�o�C�X�̃I�v�V����
 */
enum class DeviceOptionFlags {
  NONE,
  TEARING,          //! �e�A�����O�T�|�[�g
  USE_WARP_DEVICE,  //! WARP�f�o�C�X���g�p����
};

namespace defines {

//! �g�p����t���[����
constexpr u32 FRAME_COUNT = 3;

//! WARP�f�o�C�X���g�p���邩
//! ���ݓ�������ɂ�����Device�̕s������邽�߂��̎���true�ɂ���
constexpr bool USE_WARP_DEVICE = false;

//�v������ŏ��@�\���x��
//����̃v���W�F�N�g�ŗv������ŏ����x��
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
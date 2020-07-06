#ifndef LEGEND_DIRECTX_DEVICE_DEVICE_PARAMETERS_H_
#define LEGEND_DIRECTX_DEVICE_DEVICE_PARAMETERS_H_

/**
 * @file device_parameters.h
 * @brief �f�o�C�X�p�p�����[�^��`
 */

namespace legend {
namespace directx {
namespace device {
namespace parameters {

//�e�A�����O�T�|�[�g���L���ł���
constexpr u32 ALLOW_TEARING = 0x1;

//�e�A�����O�T�|�[�g��v������
constexpr u32 REQUIRE_TEARING_SUPPORT = 0x2;

//�v������ŏ��@�\���x��
//����̃v���W�F�N�g�ŗv������ŏ����x��
constexpr D3D_FEATURE_LEVEL MIN_FEATURE_LEVEL =
    D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;

}  // namespace parameters
}  // namespace device
}  // namespace directx
}  // namespace legend
#endif  //! LEGEND_DIRECTX_DEVICE_DEVICE_PARAMETERS_H_

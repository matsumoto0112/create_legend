#ifndef LEGEND_DIRECTX_SHADER_ROOT_PARAMETER_INDEX_H_
#define LEGEND_DIRECTX_SHADER_ROOT_PARAMETER_INDEX_H_

/**
 * @file root_parameter_index.h
 * @brief ���[�g�V�O�l�`���̃p�����[�^�C���f�b�N�X��`
 */

namespace legend {
namespace directx {
namespace shader {

/**
 * @brief ���[�g�V�O�l�`���̃p�����[�^�C���f�b�N�X
 * @details �O�Ԃ�CBV�A�P�Ԃ�SRV�A�Q�Ԃ�UAV���g�p����
 */
namespace root_parameter_index {
enum Enum : u32 { CBV, SRV, UAV, MAX };

}  // namespace root_parameter_index
}  // namespace shader
}  // namespace directx
}  // namespace legend
#endif  //! LEGEND_DIRECTX_SHADER_ROOT_PARAMETER_INDEX_H_

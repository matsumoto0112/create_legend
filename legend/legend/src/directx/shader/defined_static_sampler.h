#ifndef LEGEND_DIRECTX_SHADER_DEFINED_STATIC_SAMPLER_H_
#define LEGEND_DIRECTX_SHADER_DEFINED_STATIC_SAMPLER_H_

/**
 * @file defined_static_sampler.h
 * @brief �悭�g��StaticSampler���`����t�@�C��
 */

namespace legend {
namespace directx {
namespace shader {
namespace defined_static_sampler {

/**
 * @brief Warp�T���v���[���擾����
 * @param register_id �V�F�[�_�[�̃��W�X�^�[�ԍ�
 */
D3D12_STATIC_SAMPLER_DESC Warp(u32 register_id);
/**
 * @brief Clamp�T���v���[���擾����
 * @param register_id �V�F�[�_�[�̃��W�X�^�[�ԍ�
 */
D3D12_STATIC_SAMPLER_DESC Clamp(u32 register_id);

}  // namespace defined_static_sampler
}  // namespace shader
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_SHADER_DEFINED_STATIC_SAMPLER_H_

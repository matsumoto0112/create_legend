#ifndef LEGEND_DIRECTX_SHADER_DEFINED_STATIC_SAMPLER_H_
#define LEGEND_DIRECTX_SHADER_DEFINED_STATIC_SAMPLER_H_

/**
 * @file defined_static_sampler.h
 * @brief �悭�g��StaticSampler���`����t�@�C��
 */

namespace legend {
namespace directx {
namespace shader {
namespace defined_sampler {

/**
 * @brief StaticSampler���쐬����
 * @param register_number �V�F�[�_�[�̃��W�X�^�[�ԍ�
 * @param filter �t�B���^�[
 * @param address_mode �e�N�X�`���͈̔͊O�Q�Ǝ��̏������@
 * @return ��L�̐ݒ�ɂ���č��ꂽ�T���v���[
 */
D3D12_STATIC_SAMPLER_DESC CreateStaticSampler(
    u32 register_number, D3D12_FILTER filter,
    D3D12_TEXTURE_ADDRESS_MODE address_mode);

}  // namespace defined_sampler
}  // namespace shader
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_SHADER_DEFINED_STATIC_SAMPLER_H_

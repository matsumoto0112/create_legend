#ifndef LEGEND_DIRECTX_SHADER_SHADER_REGISTER_ID_H_
#define LEGEND_DIRECTX_SHADER_SHADER_REGISTER_ID_H_

/**
 * @file shader_register_id.h
 * @brief �V�F�[�_�[�̃��W�X�^�[�ԍ���`
 */

namespace legend {
namespace directx {
namespace shader {
/**
 * @brief �R���X�^���g�o�b�t�@�̃��W�X�^�[�ԍ�
 */
namespace ConstantBufferRegisterID {
enum Enum : u32 {
  GLOBAL_DATA = 0,    //! �O���[�o�����
  TRANSFORM = 1,      //! �g�����X�t�H�[�����
  WORLD_CONTEXT = 2,  //! �r���[�A�v���W�F�N�V�������
  UV_RECT = 3,
};

}  // namespace ConstantBufferRegisterID

/**
 * @brief �e�N�X�`���̃��W�X�^�[�ԍ�
 */
namespace TextureRegisterID {
enum Enum : u32 {
  ALBEDO = 0,  //! �A���x�h���
};
}  // namespace TextureRegisterID

/**
 * @brief �T���v���[�̃��W�X�^�[�ԍ�
 */
namespace SamplerRegisterID {
enum Enum : u32 {
  WARP = 0,
  CLAMP = 1,
};
}  // namespace SamplerRegisterID

}  // namespace shader
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_SHADER_SHADER_REGISTER_ID_H_

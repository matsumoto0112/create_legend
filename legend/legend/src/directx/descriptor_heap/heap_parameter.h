#ifndef LEGEND_DIRECTX_DESCRIPTOR_HEAP_HEAP_PARAMETER_H_
#define LEGEND_DIRECTX_DESCRIPTOR_HEAP_HEAP_PARAMETER_H_

/**
 * @file heap_parameter.h
 * @brief �q�[�v�̃f�t�H���g�p�����[�^��`�t�@�C��
 * @details �R���p�C�����Ɍ���ł���A��{�I�ɕύX���Ȃ������`����
 */

namespace legend {
namespace directx {
namespace descriptor_heap {

/**
 * @brief /�q�[�v�̃f�t�H���g�p�����[�^
 */
namespace heap_parameter {

//! �A�v���P�[�V�����S�̂ł̃f�B�X�N���v�^�q�[�v�̍ő吔
//! CBV,SRV,UAV�ŋ��ʂ��銄�蓖�ĉ\���̏���l
constexpr u32 GLOBAL_HEAP_DESCRIPTOR_NUM = 500000;
//! RTV�Ŏg�p���銄�蓖�ĉ\��
constexpr u32 RTV_HEAP_DESCRIPTOR_NUM = 100;
//! DSV�Ŏg�p���銄�蓖�ĉ\��
constexpr u32 DSV_HEAP_DESCRIPTOR_NUM = 6;

/**
 * @brief ���[�J���q�[�v�̓o�^ID
 * @details
 * �q�[�v���m�ۂ��邽�тɗ̈悪�����Ă����̂�1�Q�[�����ƂɃ��Z�b�g���Ȃ��Ƃ��̂�������Ȃ��Ȃ�B
 * ���̂��߁A��ɑ��݂���q�[�v�i���f���̃e�N�X�`���f�[�^�Ȃǁj�ƁA1�Q�[�����Ƃ̃q�[�v�i�G�̃g�����X�t�H�[���R���X�^���g�o�b�t�@�Ȃǁj�ɕ�����K�v������
 */
enum class LocalHeapID : u32 {
  GLOBAL_ID = 0,  //! ��ɑ��݂���q�[�v
  ONE_PLAY = 1,   //! 1�Q�[�����ƂɃ��Z�b�g�����
};

namespace local {

//����`�̎��̃f�B�X�N���v�^���蓖�Đ�
constexpr u32 UNDEFINED_DESCRIPTOR_NUM = UINT_LEAST32_MAX;

/**
 * @brief ��`�ς݂̃��[�J��ID�ɑΉ������f�B�X�N���v�^�����擾����
 */
inline constexpr u32 GetDefinedLocalDescriptorNum(LocalHeapID id) {
  switch (id) {
    case LocalHeapID::GLOBAL_ID:
      return 20000;
    case LocalHeapID::ONE_PLAY:
      return 20000;
    default:
      MY_LOG(L"����`��ID���I������܂����B");
      break;
  }

  return UNDEFINED_DESCRIPTOR_NUM;
}
}  // namespace local
}  // namespace heap_parameter
}  // namespace descriptor_heap
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_DESCRIPTOR_HEAP_HEAP_PARAMETER_H_

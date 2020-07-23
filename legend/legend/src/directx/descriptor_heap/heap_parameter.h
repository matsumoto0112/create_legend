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
constexpr u32 GLOBAL_HEAP_DESCRIPTOR_NUM = 100000;

namespace local_heap_id {
enum Enum : u32 {
  //! ��ɑ��݂���q�[�v
  STATIC = 0,
  //! �}���`�����_�[�^�[�Q�b�g�e�X�g�V�[���p
  MULTI_RENDER_TARGET_TEST_SCENE,
};
}  // namespace local_heap_id

namespace local {
/**
 * @brief ���[�J���q�[�v�̓o�^ID
 * @details
 * �ł��邾���ׂ����������ق����e�q�[�v�̕K�v�ʂ��}�����邽�߁A�V�[�����ƁA�������̓V�`���G�[�V�������Ƃɕ�����̂�z��
 */
u32 GetDefinedLocalDescriptorNum(local_heap_id::Enum id);
}  // namespace local
}  // namespace heap_parameter
}  // namespace descriptor_heap
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_DESCRIPTOR_HEAP_HEAP_PARAMETER_H_

#ifndef LEGEND_DIRECTX_DESCRIPTOR_HEAP_HEAP_PARAMETER_H_
#define LEGEND_DIRECTX_DESCRIPTOR_HEAP_HEAP_PARAMETER_H_

/**
 * @file heap_parameter.h
 * @brief �q�[�v�̃f�t�H���g�p�����[�^��`�t�@�C��
 * @details �R���p�C�����Ɍ���ł���A��{�I�ɕύX���Ȃ������`����
 */

#include "src/util/stl_extend.h"

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

/**
 * @brief ���[�J���q�[�v�̓o�^ID
 * @details
 * �ł��邾���ׂ����������ق����e�q�[�v�̕K�v�ʂ��}�����邽�߁A�V�[�����ƁA�������̓V�`���G�[�V�������Ƃɕ�����̂�z��
 */
enum class LocalHeapID : u32 {
  //! ��ɑ��݂���q�[�v
  GLOBAL_ID = 0,
  GAME_OVER,
  TITLE,

  MODEL_VIEW_SCENE = 100,
  //! �}���`�����_�[�^�[�Q�b�g�e�X�g�V�[���p
  MULTI_RENDER_TARGET_TEST_SCENE,
  PERSPECTIVE_CAMERA_TEST_SCENE,
  PHYSICS_TEST,
  POST_PROCESS_VIEWER,
  SOUND_TEST,
  SPRITE_RENDER_TEST,
};

namespace local {
/**
 * @brief ��`�ς݂̃��[�J��ID�ɑΉ������f�B�X�N���v�^�����擾����
 */
inline constexpr u32 GetDefinedLocalDescriptorNum(LocalHeapID id) {
  switch (id) {
    case legend::directx::descriptor_heap::heap_parameter::LocalHeapID::
        GLOBAL_ID:
      return 20000;
    case legend::directx::descriptor_heap::heap_parameter::LocalHeapID::
        GAME_OVER:
      return 10000;
    case legend::directx::descriptor_heap::heap_parameter::LocalHeapID::TITLE:
      return 10000;
    case legend::directx::descriptor_heap::heap_parameter::LocalHeapID::
        MODEL_VIEW_SCENE:
      return 100;
    case legend::directx::descriptor_heap::heap_parameter::LocalHeapID::
        MULTI_RENDER_TARGET_TEST_SCENE:
      return 100;
    case legend::directx::descriptor_heap::heap_parameter::LocalHeapID::
        PERSPECTIVE_CAMERA_TEST_SCENE:
      return 100;
    case legend::directx::descriptor_heap::heap_parameter::LocalHeapID::
        PHYSICS_TEST:
      return 100;
    case legend::directx::descriptor_heap::heap_parameter::LocalHeapID::
        POST_PROCESS_VIEWER:
      return 100;
    case legend::directx::descriptor_heap::heap_parameter::LocalHeapID::
        SOUND_TEST:
      return 100;
    case legend::directx::descriptor_heap::heap_parameter::LocalHeapID::
        SPRITE_RENDER_TEST:
      return 100;
    default:
      MY_LOG(L"����`��ID���I������܂����B");
      break;
  }

  return 0;
}
}  // namespace local
}  // namespace heap_parameter
}  // namespace descriptor_heap
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_DESCRIPTOR_HEAP_HEAP_PARAMETER_H_

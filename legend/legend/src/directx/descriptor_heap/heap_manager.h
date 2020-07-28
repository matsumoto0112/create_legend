#ifndef LEGEND_DIRECTX_DESCRIPTOR_HEAP_HEAP_MANAGER_H_
#define LEGEND_DIRECTX_DESCRIPTOR_HEAP_HEAP_MANAGER_H_

/**
 * @file heap_manager.h
 * @brief �q�[�v�Ǘ��N���X��`
 * @details �f�B�X�N���v�^�q�[�v�Ǘ��N���X�̎����ɂ���
 �O���[�o���q�[�v�ƃ��[�J���q�[�v�̓��ނ̃q�[�v���g�������A��������Ă���B
 �O���[�o���q�[�v�͎��ۂ�GPU�ɓ]�������q�[�v�A���[�J���q�[�v��CPU���݂̂Ŏg�p�����B
 �ʏ�ASRV,CBV�Ȃǂ��쐬����Ƃ��ɂ�CPU�AGPU�n���h���Ƃ��ɕK�v�ŁA���̒i�K����n���h����ύX�ł��Ȃ����߁A�C�ӂ̏��A�C�ӂ̐��]������̂�����ɂȂ��Ă���
 ���̂��߁ASRV�����Ƃ��ɂ̓��[�J���q�[�v�̃n���h�����g�p���A�����K�v�ɂȂ莟��O���[�o���ɃR�s�[���Ďg���헪������͑I�������B
 */

#include "src/directx/descriptor_heap/counting_descriptor_heap.h"
#include "src/directx/descriptor_heap/descriptor_handle.h"
#include "src/directx/descriptor_heap/descriptor_heap.h"
#include "src/directx/descriptor_heap/heap_parameter.h"
#include "src/directx/directx_accessor.h"
#include "src/directx/resource_type.h"

namespace legend {
namespace directx {
namespace descriptor_heap {

/**
 * @brief �q�[�v�Ǘ��N���X
 */
class HeapManager {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  HeapManager();
  /**
   * @brief �f�X�g���N�^
   */
  ~HeapManager();
  /**
   * @brief ������
   * @param accessor DirectX12�A�N�Z�T
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init(IDirectXAccessor& accessor);
  /**
   * @brief �t���[���J�n���C�x���g
   */
  void BeginNewFrame();
  /**
   * @brief �R�}���h���X�g�ɃO���[�o���q�[�v���Z�b�g����
   * @param accessor DirectX12�A�N�Z�T
   */
  void SetHeapToCommandList(IDirectXAccessor& accessor) const;
  /**
   * @brief ���[�J���q�[�v�Ƀn���h�����Z�b�g����
   * @param register_num �g�p���郊�\�[�X�̃V�F�[�_�[�ɂ����郌�W�X�^�[�ԍ�
   * @param type �g�p���郊�\�[�X�̎��
   * @param handle �Z�b�g����CPU�n���h��
   */
  void SetHandleToLocalHeap(u32 register_num, ResourceType type,
                            D3D12_CPU_DESCRIPTOR_HANDLE handle);
  /**
   * @brief ���[�J������O���[�o���Ƀq�[�v���R�s�[���R�}���h���X�g�ɃZ�b�g����
   * @param accessor DirectX12�A�N�Z�T
   */
  void CopyHeapAndSetToGraphicsCommandList(IDirectXAccessor& accessor);
  /**
   * @brief ���[�J���q�[�v��ǉ�����
   * @param accessor DirectX12�A�N�Z�T
   * @param id �q�[�v�̎���ID
   * @return
   */
  bool AddLocalHeap(IDirectXAccessor& accessor, heap_parameter::LocalHeapID id);
  /**
   * @brief ID�ɑΉ��������[�J���q�[�v�̃J�E���^�[�����Z�b�g����
   */
  void ResetLocalHeapAllocateCounter(heap_parameter::LocalHeapID id);

 public:
  /**
   * @brief �����_�[�^�[�Q�b�g�q�[�v���擾����
   */
  CountingDescriptorHeap* GetRtvHeap() { return &rtv_heap_; }
  /**
   * @brief �f�v�X�E�X�e���V���q�[�v���擾����
   */
  CountingDescriptorHeap* GetDsvHeap() { return &dsv_heap_; }
  /**
   * @brief ID���烍�[�J���q�[�v���擾����
   * @param id ���[�J���q�[�v��ID
   * @return ID���L���Ȃ�Ή��������[�J���q�[�v��Ԃ�
   */
  CountingDescriptorHeap* GetLocalHeap(heap_parameter::LocalHeapID id);

 private:
  //! �O���[�o���q�[�v
  DescriptorHeap global_heap_;
  //! �O���[�o���q�[�v�̌��݃t���[���ɂ����銄�蓖�Đ�
  u32 global_heap_allocated_count_;
  //! �R���X�^���g�o�b�t�@�̃n���h��
  std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> cbv_handles_;
  //! �V�F�[�_�[���\�[�X�̃n���h��
  std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> srv_handles_;
  //! ���[�J���̃V�F�[�_�[���\�[�X�q�[�v
  std::unordered_map<heap_parameter::LocalHeapID, CountingDescriptorHeap>
      local_heaps_;
  //! �����_�[�^�[�Q�b�g�q�[�v
  CountingDescriptorHeap rtv_heap_;
  //! �f�v�X�E�X�e���V���q�[�v
  CountingDescriptorHeap dsv_heap_;
  //! �󔒂̊��蓖�Ă�u��������f�t�H���g�n���h��
  DescriptorHandle default_handle_;
};

}  // namespace descriptor_heap
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_DESCRIPTOR_HEAP_HEAP_MANAGER_H_
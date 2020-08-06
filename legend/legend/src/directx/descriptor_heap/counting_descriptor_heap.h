#ifndef LEGEND_DIRECTX_DESCRIPTOR_HEAP_COUNTING_DESCRIPTOR_HEAP_H_
#define LEGEND_DIRECTX_DESCRIPTOR_HEAP_COUNTING_DESCRIPTOR_HEAP_H_

/**
 * @file counting_descriptor_heap.h
 * @brief �����C���f�b�N�X���J�E���e�B���O���Ă����f�B�X�N���v�^�q�[�v
 * @details
 * ���蓖�Ă邽�тɃC���f�b�N�X���C���N�������g���Ă����̂ŁA�q�[�v�̊��蓖�ăC���f�b�N�X���O���ŊǗ�����K�v�̂Ȃ��q�[�v
 */

#include "src/directx/descriptor_heap/descriptor_heap.h"

namespace legend {
namespace directx {
namespace descriptor_heap {

/**
 * @brief �J�E���^�t���f�B�X�N���v�^�q�[�v
 */
class CountingDescriptorHeap {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  CountingDescriptorHeap();
  /**
   * @brief �f�X�g���N�^
   */
  ~CountingDescriptorHeap();
  /**
   * @brief ������
   * @param accessor DirectX12�A�N�Z�T
   * @param desc �q�[�v�쐬�f�B�X�N
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init(IDirectXAccessor& accessor, const DescriptorHeap::Desc& desc);
  /**
   * @brief CPU,GPU�n���h�����擾����
   * @param index �n���h���C���f�b�N�X
   * @return �L���ȃn���h����Ԃ�
   */
  DescriptorHandle GetHandle();
  /**
   * @brief �C���f�b�N�X���w�肵���n���h����Ԃ�
   * @param index �n���h���C���f�b�N�X
   * @return �L���ȃn���h����Ԃ�
   */
  DescriptorHandle GetForceHandle(u32 index) const;
  /**
   * @brief �A���P�[�g�J�E���^�[�����Z�b�g����
   */
  void ResetAllocateCounter();
  /**
   * @brief �q�[�v���擾����
   */
  ID3D12DescriptorHeap* GetHeap() const { return heap_.GetHeap(); };

 private:
  //! �f�B�X�N���v�^�q�[�v
  DescriptorHeap heap_;
  //! ���݊��蓖�ĂĂ���C���f�b�N�X
  u32 allocate_index_;
  //! �ő劄�蓖�ĉ\��
  u32 max_allocate_num_;
};

}  // namespace descriptor_heap
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_DESCRIPTOR_HEAP_COUNTING_DESCRIPTOR_HEAP_H_

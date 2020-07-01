#ifndef LEGEND_DIRECTX_DESCRIPTOR_HEAP_COUNTING_DESCRIPTOR_HEAP_H_
#define LEGEND_DIRECTX_DESCRIPTOR_HEAP_COUNTING_DESCRIPTOR_HEAP_H_

/**
 * @file counting_descriptor_heap.h
 * @brief �����C���f�b�N�X���J�E���e�B���O���Ă����f�B�X�N���v�^�q�[�v
 * @details
 * ���蓖�Ă邽�тɃC���f�b�N�X���C���N�������g���Ă����̂ŁA�q�[�v�̊��蓖�ăC���f�b�N�X���O���ŊǗ�����K�v�̂Ȃ��q�[�v
 */

#include "src/directx/descriptor_heap/descriptor_handle.h"
#include "src/directx/descriptor_heap/descriptor_heap.h"

namespace legend {
namespace directx {
/**
 * @brief �J�E���^�t���f�B�X�N���v�^�q�[�v
 */
class CountingDescriptorHeap {
 public:
  CountingDescriptorHeap();
  ~CountingDescriptorHeap();
  bool Init(IDirectXAccessor& accessor, const DescriptorHeap::Desc& desc);
  DescriptorHandle GetHandle();
  DescriptorHandle GetForceHandle(u32 index) const;
  ID3D12DescriptorHeap* GetHeap() const { return heap_.GetHeap(); };

 private:
  DescriptorHeap heap_;
  u32 allocate_index_;
  u32 max_allocate_num_;
};

}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_DESCRIPTOR_HEAP_COUNTING_DESCRIPTOR_HEAP_H_

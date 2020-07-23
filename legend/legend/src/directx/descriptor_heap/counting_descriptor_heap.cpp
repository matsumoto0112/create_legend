#include "counting_descriptor_heap.h"

namespace legend {
namespace directx {
namespace descriptor_heap {

//�R���X�g���N�^
CountingDescriptorHeap::CountingDescriptorHeap()
    : heap_(), allocate_index_(0), max_allocate_num_(0) {}

//�f�X�g���N�^
CountingDescriptorHeap::~CountingDescriptorHeap() {}

//������
bool CountingDescriptorHeap::Init(IDirectXAccessor& accessor,
                                  const DescriptorHeap::Desc& desc) {
  if (!this->heap_.Init(accessor, desc)) {
    return false;
  }

  this->max_allocate_num_ = desc.descriptor_num;
  this->allocate_index_ = 0;
  return true;
}

//�n���h�����擾����
DescriptorHandle CountingDescriptorHeap::GetHandle() {
  MY_ASSERTION(allocate_index_ < max_allocate_num_,
               L"���蓖�ĉ\������𒴂��Ă��܂��B");

  return heap_.GetHandle(allocate_index_++);
}

//�C���f�b�N�X�w��̃n���h�����擾����
DescriptorHandle CountingDescriptorHeap::GetForceHandle(u32 index) const {
  return heap_.GetHandle(index);
}

//�A���P�[�g�J�E���^�[�̃��Z�b�g
void CountingDescriptorHeap::ResetAllocateCounter() { allocate_index_ = 0; }

}  // namespace descriptor_heap
}  // namespace directx
}  // namespace legend
#include "counting_descriptor_heap.h"

#include "src/directx/descriptor_heap.h"

namespace legend {
namespace directx {

//�R���X�g���N�^
CountingDescriptorHeap::CountingDescriptorHeap()
    : heap_(), allocate_index_(0) {}

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

//�f�B�X�N���v�^�n���h��
DescriptorHandle CountingDescriptorHeap::GetHandle() {
  MY_ASSERTION(allocate_index_ < max_allocate_num_,
               L"���蓖�ĉ\������𒴂��Ă��܂��B");
  const DescriptorHandle handle = heap_.GetHandle(allocate_index_);
  allocate_index_++;
  return handle;
}

DescriptorHandle CountingDescriptorHeap::GetForceHandle(u32 index) const {
  return heap_.GetHandle(index);
}

}  // namespace directx
}  // namespace legend
#include "counting_descriptor_heap.h"

#include "src/directx/descriptor_heap.h"

namespace legend {
namespace directx {

//コンストラクタ
CountingDescriptorHeap::CountingDescriptorHeap()
    : heap_(), allocate_index_(0) {}

//デストラクタ
CountingDescriptorHeap::~CountingDescriptorHeap() {}

//初期化
bool CountingDescriptorHeap::Init(IDirectXAccessor& accessor,
                                  const DescriptorHeap::Desc& desc) {
  if (!this->heap_.Init(accessor, desc)) {
    return false;
  }

  this->max_allocate_num_ = desc.descriptor_num;
  this->allocate_index_ = 0;
  return true;
}

//ディスクリプタハンドル
DescriptorHandle CountingDescriptorHeap::GetHandle() {
  MY_ASSERTION(allocate_index_ < max_allocate_num_,
               L"割り当て可能上限数を超えています。");
  const DescriptorHandle handle = heap_.GetHandle(allocate_index_);
  allocate_index_++;
  return handle;
}

DescriptorHandle CountingDescriptorHeap::GetForceHandle(u32 index) const {
  return heap_.GetHandle(index);
}

}  // namespace directx
}  // namespace legend
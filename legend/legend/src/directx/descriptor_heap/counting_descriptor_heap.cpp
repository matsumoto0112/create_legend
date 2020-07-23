#include "counting_descriptor_heap.h"

namespace legend {
namespace directx {
namespace descriptor_heap {

//コンストラクタ
CountingDescriptorHeap::CountingDescriptorHeap()
    : heap_(), allocate_index_(0), max_allocate_num_(0) {}

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

//ハンドルを取得する
DescriptorHandle CountingDescriptorHeap::GetHandle() {
  MY_ASSERTION(allocate_index_ < max_allocate_num_,
               L"割り当て可能上限数を超えています。");

  return heap_.GetHandle(allocate_index_++);
}

//インデックス指定のハンドルを取得する
DescriptorHandle CountingDescriptorHeap::GetForceHandle(u32 index) const {
  return heap_.GetHandle(index);
}

//アロケートカウンターのリセット
void CountingDescriptorHeap::ResetAllocateCounter() { allocate_index_ = 0; }

}  // namespace descriptor_heap
}  // namespace directx
}  // namespace legend
#ifndef LEGEND_DIRECTX_DESCRIPTOR_HEAP_COUNTING_DESCRIPTOR_HEAP_H_
#define LEGEND_DIRECTX_DESCRIPTOR_HEAP_COUNTING_DESCRIPTOR_HEAP_H_

/**
 * @file counting_descriptor_heap.h
 * @brief 内部インデックスをカウンティングしていくディスクリプタヒープ
 * @details
 * 割り当てるたびにインデックスをインクリメントしていくので、ヒープの割り当てインデックスを外部で管理する必要のないヒープ
 */

#include "src/directx/descriptor_heap/descriptor_handle.h"
#include "src/directx/descriptor_heap/descriptor_heap.h"

namespace legend {
namespace directx {
/**
 * @brief カウンタ付きディスクリプタヒープ
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

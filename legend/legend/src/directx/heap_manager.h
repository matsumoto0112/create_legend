#ifndef LEGEND_DIRECTX_HEAP_MANAGER_H_
#define LEGEND_DIRECTX_HEAP_MANAGER_H_

/**
 * @file heap_manager.h
 * @brief ヒープ管理クラス定義
 */

#include "src/directx/descriptor_handle.h"
#include "src/directx/descriptor_heap.h"
#include "src/directx/directx_accessor.h"

namespace legend {
namespace directx {

enum class ResourceType {
  Cbv,
  Srv,
  Uav,
};

/**
 * @brief ヒープ管理クラス
 */
class HeapManager {
 public:
  /**
   * @brief コンストラクタ
   */
  HeapManager();
  ~HeapManager();
  bool Init(IDirectXAccessor& device);
  DescriptorHandle GetLocalHandle();
  void SetGraphicsCommandList(IDirectXAccessor& device);
  void StackLocalHeap(u32 register_num, ResourceType type,
                      D3D12_CPU_DESCRIPTOR_HANDLE handle);
  void PushToGlobalHeapAndSetCommandList(IDirectXAccessor& device);

 private:
  DescriptorHeap global_heap_;
  DescriptorHeap local_heap_;
  u32 local_heap_allocated_count_;
  u32 global_heap_index_;

  std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> cbv_handles;
  std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> srv_handles;
};

}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_HEAP_MANAGER_H_

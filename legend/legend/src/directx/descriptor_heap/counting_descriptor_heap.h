#ifndef LEGEND_DIRECTX_DESCRIPTOR_HEAP_COUNTING_DESCRIPTOR_HEAP_H_
#define LEGEND_DIRECTX_DESCRIPTOR_HEAP_COUNTING_DESCRIPTOR_HEAP_H_

/**
 * @file counting_descriptor_heap.h
 * @brief 内部インデックスをカウンティングしていくディスクリプタヒープ
 * @details
 * 割り当てるたびにインデックスをインクリメントしていくので、ヒープの割り当てインデックスを外部で管理する必要のないヒープ
 */

#include "src/directx/descriptor_heap/descriptor_heap.h"

namespace legend {
namespace directx {
namespace descriptor_heap {

/**
 * @brief カウンタ付きディスクリプタヒープ
 */
class CountingDescriptorHeap {
 public:
  /**
   * @brief コンストラクタ
   */
  CountingDescriptorHeap();
  /**
   * @brief デストラクタ
   */
  ~CountingDescriptorHeap();
  /**
   * @brief 初期化
   * @param accessor DirectX12アクセサ
   * @param desc ヒープ作成ディスク
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(IDirectXAccessor& accessor, const DescriptorHeap::Desc& desc);
  /**
   * @brief CPU,GPUハンドルを取得する
   * @param index ハンドルインデックス
   * @return 有効なハンドルを返す
   */
  DescriptorHandle GetHandle();
  /**
   * @brief インデックスを指定したハンドルを返す
   * @param index ハンドルインデックス
   * @return 有効なハンドルを返す
   */
  DescriptorHandle GetForceHandle(u32 index) const;
  /**
   * @brief アロケートカウンターをリセットする
   */
  void ResetAllocateCounter();
  /**
   * @brief ヒープを取得する
   */
  ID3D12DescriptorHeap* GetHeap() const { return heap_.GetHeap(); };

 private:
  //! ディスクリプタヒープ
  DescriptorHeap heap_;
  //! 現在割り当てているインデックス
  u32 allocate_index_;
  //! 最大割り当て可能数
  u32 max_allocate_num_;
};

}  // namespace descriptor_heap
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_DESCRIPTOR_HEAP_COUNTING_DESCRIPTOR_HEAP_H_

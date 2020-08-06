#ifndef LEGEND_DIRECTX_DESCRIPTOR_HEAP_DESCRIPTOR_HEAP_H_
#define LEGEND_DIRECTX_DESCRIPTOR_HEAP_DESCRIPTOR_HEAP_H_

/**
 * @file descriptor_heap.h
 * @brief ディスクリプタヒープ定義
 */

#include "src/directx/descriptor_heap/descriptor_handle.h"
#include "src/directx/descriptor_heap/descriptor_heap_type.h"
#include "src/directx/device/directx_accessor.h"

namespace legend {
namespace directx {
namespace descriptor_heap {

/**
 * @brief ディスクリプタヒープのラップクラス
 */
class DescriptorHeap {
 public:
  /**
   * @brief ディスクリプタヒープディスク
   */
  struct Desc final {
    //! ヒープ名
    std::wstring name;
    //! 使用するディスクリプタヒープの数
    u32 descriptor_num;
    //! ヒープの種類
    DescriptorHeapType type;
    //! ヒープフラグ
    DescriptorHeapFlag flag;
  };

 public:
  /**
   * @brief コンストラクタ
   */
  DescriptorHeap();
  /**
   * @brief デストラクタ
   */
  ~DescriptorHeap();
  /**
   * @brief 初期化
   * @param accessor DirectXデバイスアクセサ
   * @param desc 作成するヒープの設定
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(device::IDirectXAccessor& accessor, const Desc& desc);
  /**
   * @brief CPU,GPUハンドルを取得する
   * @param index ハンドルインデックス
   * @return 有効なハンドルを返す
   */
  DescriptorHandle GetHandle(u32 index) const;
  /**
   * @brief ヒープを取得する
   * @details
   * 複数のヒープを同時に使用するとき、まとめて渡す必要があるのでこのクラスでセットはしないためヒープを取得する関数を公開する
   */
  ID3D12DescriptorHeap* GetHeap() const { return heap_.Get(); }

 private:
  //! ヒープ
  ComPtr<ID3D12DescriptorHeap> heap_;
  //! ヒープの単位サイズ
  u32 heap_size_;
};

}  // namespace descriptor_heap
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_DESCRIPTOR_HEAP_DESCRIPTOR_HEAP_H_

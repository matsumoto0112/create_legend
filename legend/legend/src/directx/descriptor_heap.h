#ifndef LEGEND_DIRECTX_DESCRIPTOR_HEAP_H_
#define LEGEND_DIRECTX_DESCRIPTOR_HEAP_H_

/**
 * @file descriptor_heap.h
 * @brief ディスクリプタヒープ定義
 */

#include "src/directx/directx_accessor.h"

namespace legend {
namespace directx {
/**
 * @brief ヒープの種類
 */
enum class HeapType {
  CBV_SRV_UAV,
  Sampler,
  RTV,
  DSV,
};

/**
 * @brief ヒープのフラグ
 */
enum class HeapFlag {
  None,
  ShaderVisible,
};

/**
 * @brief ディスクリプタヒープのラップクラス
 */
class DescriptorHeap {
 public:
  /**
   * @brief ディスクリプタヒープディスク
   */
  struct Desc {
    //! 使用するディスクリプタヒープの数
    u32 descriptor_num;
    //! ヒープの種類
    HeapType type;
    //! ヒープフラグ
    HeapFlag flag;
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
   * @param device DirectX12デバイス
   * @param desc 作成するヒープの設定
   * @param name ヒープ名
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(IDirectXAccessor& device, const Desc& desc,
            const std::wstring& name);
  /**
   * @brief CPUハンドルを取得する
   * @param index ハンドルインデックス
   * @return 有効なハンドルを返す
   */
  D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(u32 index);
  /**
   * @brief GPUハンドルを取得する
   * @param index ハンドルインデックス
   * @return 有効なハンドルを返す
   */
  D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle(u32 index);
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

}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_DESCRIPTOR_HEAP_H_

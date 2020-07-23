#ifndef LEGEND_DIRECTX_DESCRIPTOR_HEAP_HEAP_MANAGER_H_
#define LEGEND_DIRECTX_DESCRIPTOR_HEAP_HEAP_MANAGER_H_

/**
 * @file heap_manager.h
 * @brief ヒープ管理クラス定義
 * @details ディスクリプタヒープ管理クラスの実装について
 グローバルヒープとローカルヒープの二種類のヒープを使い分け、実装されている。
 グローバルヒープは実際にGPUに転送されるヒープ、ローカルヒープはCPU内のみで使用される。
 通常、SRV,CBVなどを作成するときにはCPU、GPUハンドルともに必要で、その段階からハンドルを変更できないため、任意の順、任意の数転送するのが困難になっている
 そのため、SRVを作るときにはローカルヒープのハンドルを使用し、それを必要になり次第グローバルにコピーして使う戦略を今回は選択した。
 */

#include "src/directx/descriptor_heap/counting_descriptor_heap.h"
#include "src/directx/descriptor_heap/descriptor_handle.h"
#include "src/directx/descriptor_heap/descriptor_heap.h"
#include "src/directx/directx_accessor.h"
#include "src/directx/resource_type.h"

namespace legend {
namespace directx {
namespace descriptor_heap {

/**
 * @brief ヒープ管理クラス
 */
class HeapManager {
 public:
  /**
   * @brief コンストラクタ
   */
  HeapManager();
  /**
   * @brief デストラクタ
   */
  ~HeapManager();
  /**
   * @brief 初期化
   * @param accessor DirectX12アクセサ
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(IDirectXAccessor& accessor);
  /**
   * @brief フレーム開始時イベント
   */
  void BeginNewFrame();
  /**
   * @brief コマンドリストにグローバルヒープをセットする
   * @param accessor DirectX12アクセサ
   */
  void SetHeapToCommandList(IDirectXAccessor& accessor) const;
  /**
   * @brief ローカルヒープにハンドルをセットする
   * @param register_num 使用するリソースのシェーダーにおけるレジスター番号
   * @param type 使用するリソースの種類
   * @param handle セットするCPUハンドル
   */
  void SetHandleToLocalHeap(u32 register_num, ResourceType type,
                            D3D12_CPU_DESCRIPTOR_HANDLE handle);
  /**
   * @brief ローカルからグローバルにヒープをコピーしコマンドリストにセットする
   * @param accessor DirectX12アクセサ
   */
  void CopyHeapAndSetToGraphicsCommandList(IDirectXAccessor& accessor);
  /**
   * @brief ローカルヒープを追加する
   * @param accessor DirectX12アクセサ
   * @param id ヒープの識別ID
   * @param desc
   * @return
   */
  bool AddLocalHeap(IDirectXAccessor& accessor, u32 id,
                    const DescriptorHeap::Desc& desc);
  /**
   * @brief IDに対応したローカルヒープのカウンターをリセットする
   */
  void ResetLocalHeapAllocateCounter(u32 id);

 public:
  /**
   * @brief レンダーターゲットヒープを取得する
   */
  CountingDescriptorHeap* GetRtvHeap() { return &rtv_heap_; }
  /**
   * @brief デプス・ステンシルヒープを取得する
   */
  CountingDescriptorHeap* GetDsvHeap() { return &dsv_heap_; }
  /**
   * @brief IDからローカルヒープを取得する
   * @param id ローカルヒープのID
   * @return IDが有効なら対応したローカルヒープを返す
   */
  CountingDescriptorHeap* GetLocalHeap(u32 id = 0);

 private:
  //! グローバルヒープ
  DescriptorHeap global_heap_;
  //! グローバルヒープの現在フレームにおける割り当て数
  u32 global_heap_allocated_count_;
  //! コンスタントバッファのハンドル
  std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> cbv_handles_;
  //! シェーダーリソースのハンドル
  std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> srv_handles_;
  //! ローカルのシェーダーリソースヒープ
  std::unordered_map<u32, CountingDescriptorHeap> local_heaps_;
  //! レンダーターゲットヒープ
  CountingDescriptorHeap rtv_heap_;
  //! デプス・ステンシルヒープ
  CountingDescriptorHeap dsv_heap_;
  //! 空白の割り当てを置き換えるデフォルトハンドル
  DescriptorHandle default_handle_;
};

}  // namespace descriptor_heap
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_DESCRIPTOR_HEAP_HEAP_MANAGER_H_

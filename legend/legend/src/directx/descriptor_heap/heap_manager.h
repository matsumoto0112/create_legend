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
#include "src/directx/descriptor_heap/descriptor_heap.h"
#include "src/directx/descriptor_heap/heap_parameter.h"
#include "src/directx/device/command_list.h"
#include "src/directx/shader/shader_resource_type.h"

namespace legend {
namespace directx {
namespace descriptor_heap {

/**
 * @brief ヒープ管理クラス
 */
class HeapManager {
 private:
  using LocalHeapMap =
      std::unordered_map<heap_parameter::LocalHeapID, CountingDescriptorHeap>;

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
   * @param accessor DirectXデバイスアクセサ
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(device::IDirectXAccessor& accessor);
  /**
   * @brief フレーム開始時に呼ぶ
   */
  void BeginFrame();

  void SetGraphicsCommandList(device::CommandList& command_list) const;

  void RegisterHandle(u32 register_num, shader::ResourceType type,
                      DescriptorHandle handle);

  void UpdateGlobalHeap(ID3D12Device* device,
                        device::CommandList& command_list);
  bool AddLocalHeap(device::IDirectXAccessor& accessor,
                    heap_parameter::LocalHeapID heap_id);

  void RemoveLocalHeap(heap_parameter::LocalHeapID heap_id);

  DescriptorHandle GetLocalHeap(heap_parameter::LocalHeapID heap_id);

 public:
  /**
   * @brief レンダーターゲットヒープを取得する
   */
  CountingDescriptorHeap* GetRtvHeap() { return &rtv_heap_; }
  /**
   * @brief デプス・ステンシルヒープを取得する
   */
  CountingDescriptorHeap* GetDsvHeap() { return &dsv_heap_; }
  ///**
  // * @brief IDからローカルヒープを取得する
  // * @param id ローカルヒープのID
  // * @return IDが有効なら対応したローカルヒープを返す
  // */
  // CountingDescriptorHeap* GetLocalHeap(heap_parameter::LocalHeapID id);

 private:
  //! レンダーターゲットヒープ
  CountingDescriptorHeap rtv_heap_;
  //! デプス・ステンシルヒープ
  CountingDescriptorHeap dsv_heap_;
  //! グローバルヒープ
  DescriptorHeap global_heap_;
  //! グローバルヒープの現在フレームにおける割り当て数
  u32 global_heap_allocated_count_;
  //! ローカルのシェーダーリソースヒープ
  LocalHeapMap local_heaps_;

  struct LocalHandles final {
    //! コンスタントバッファのハンドル
    std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> cbv_handles_;
    //! シェーダーリソースのハンドル
    std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> srv_handles_;
  } current_local_handles_;

  /**
   * @brief 空白の割り当てを置き換えるデフォルトハンドル
   */
  struct DefaultHandle final {
    DescriptorHandle default_srv_handle_;
    DescriptorHandle default_cbv_handle_;
  } default_handle_;
};

}  // namespace descriptor_heap
}  // namespace directx
}  // namespace legend

#endif  //! LEGEND_DIRECTX_DESCRIPTOR_HEAP_HEAP_MANAGER_H_

#include "src/directx/descriptor_heap/heap_manager.h"

namespace {

//! グローバルヒープの作成するディスクリプタ数
constexpr legend::u32 GLOBAL_HEAP_DESCRIPTOR_NUM = 100000;
//! ローカルヒープの作成するディスクリプタ数
constexpr legend::u32 LOCAL_HEAP_DESCRIPTOR_NUM = 100000;
//! レンダーターゲットヒープの作成するディスクリプタ数
constexpr legend::u32 RTV_HEAP_DESCRIPTOR_NUM = 100;
//! デプス・ステンシルヒープの作成するディスクリプタ数
constexpr legend::u32 DSV_HEAP_DESCRIPTOR_NUM = 5;

legend::directx::descriptor_heap::DescriptorHandle default_cbv_;
}  // namespace

namespace legend {
namespace directx {
namespace descriptor_heap {

//コンストラクタ
HeapManager::HeapManager() {}

//デストラクタ
HeapManager::~HeapManager() {}

//初期化
bool HeapManager::Init(IDirectXAccessor& device) {
  DescriptorHeap::Desc global_desc(L"GlobalHeap", GLOBAL_HEAP_DESCRIPTOR_NUM,
                                   DescriptorHeapType::CBV_SRV_UAV,
                                   DescriptorHeapFlag::SHADER_VISIBLE);

  if (!global_heap_.Init(device, global_desc)) {
    return false;
  }

  DescriptorHeap::Desc local_desc(L"LocalHeap", LOCAL_HEAP_DESCRIPTOR_NUM,
                                  DescriptorHeapType::CBV_SRV_UAV,
                                  DescriptorHeapFlag::NONE);
  if (!cbv_srv_uav_heap_.Init(device, local_desc)) {
    return false;
  }

  DescriptorHeap::Desc rtv_desc(L"RTVHeap", RTV_HEAP_DESCRIPTOR_NUM,
                                DescriptorHeapType::RTV,
                                DescriptorHeapFlag::NONE);
  if (!rtv_heap_.Init(device, rtv_desc)) {
    return false;
  }

  DescriptorHeap::Desc dsv_desc(L"DSVHeap", DSV_HEAP_DESCRIPTOR_NUM,
                                DescriptorHeapType::DSV,
                                DescriptorHeapFlag::NONE);
  if (!dsv_heap_.Init(device, dsv_desc)) {
    return false;
  }

  this->global_heap_allocated_count_ = 0;
  default_cbv_ = GetLocalHandle();
  return true;
}

//ローカルのヒープハンドルを取得する
DescriptorHandle HeapManager::GetLocalHandle() {
  return cbv_srv_uav_heap_.GetHandle();
}

//フレーム開始時イベント
void HeapManager::BeginFrame() { global_heap_allocated_count_ = 0; }

//コマンドリストにグローバルヒープをセットする
void HeapManager::SetGraphicsCommandList(IDirectXAccessor& device) const {
  ID3D12DescriptorHeap* heaps[] = {global_heap_.GetHeap()};
  device.GetCommandList()->SetDescriptorHeaps(1, heaps);
}

//ローカルヒープにハンドルをセットする
void HeapManager::SetHandleToLocalHeap(u32 register_num, ResourceType type,
                                       D3D12_CPU_DESCRIPTOR_HANDLE handle) {
  //必要に応じて配列を拡張しつつ、ハンドルをセットする
  //レジスター番号に合わせてハンドルをセットする
  auto SetToHandlesAndAppendIfNeed =
      [](u32 register_num, std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>* handles,
         D3D12_CPU_DESCRIPTOR_HANDLE handle) {
        //レジスター番号より現在サイズ数が小さければ足りない分拡張する
        if (handles->size() <= register_num) {
          handles->resize(register_num + 1);
        }
        (*handles)[register_num] = handle;
      };

  //種類に応じて追加先を変える
  switch (type) {
    case legend::directx::ResourceType::Cbv:
      SetToHandlesAndAppendIfNeed(register_num, &cbv_handles_, handle);
      break;
    case legend::directx::ResourceType::Srv:
      SetToHandlesAndAppendIfNeed(register_num, &srv_handles_, handle);
      break;
    default:
      break;
  }
}

//ヒープをコピーしコマンドリストにセットする
void HeapManager::CopyHeapAndSetToGraphicsCommandList(
    IDirectXAccessor& device) {
  auto SetTo = [&](u32 index,
                   const std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>& handles) {
    //必要な数だけローカルからグローバルにコピーする
    u32 count = static_cast<u32>(handles.size());
    if (count == 0) return;
    DescriptorHandle global_handle =
        global_heap_.GetHandle(global_heap_allocated_count_);
    D3D12_CPU_DESCRIPTOR_HANDLE dst_handle = global_handle.cpu_handle_;
    device.GetDevice()->CopyDescriptors(
        1, &dst_handle, &count, count, handles.data(), nullptr,
        D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    device.GetCommandList()->SetGraphicsRootDescriptorTable(
        index, global_handle.gpu_handle_);
    global_heap_allocated_count_ += count;
  };

  for (auto i = 0; i < cbv_handles_.size(); i++) {
    if (cbv_handles_[i].ptr == 0) {
      cbv_handles_[i] = default_cbv_.cpu_handle_;
    }
  }
  SetTo(0, cbv_handles_);
  SetTo(1, srv_handles_);
}

}  // namespace descriptor_heap
}  // namespace directx
}  // namespace legend

#include "src/directx/heap_manager.h"

namespace {

//! グローバルヒープの作成するディスクリプタ数
constexpr legend::u32 GLOBAL_HEAP_DESCRIPTOR_NUM = 10000;
//! ローカルヒープの作成するディスクリプタ数
constexpr legend::u32 LOCAL_HEAP_DESCRIPTOR_NUM = 10000;
}  // namespace

namespace legend {
namespace directx {

//コンストラクタ
HeapManager::HeapManager() {}

//デストラクタ
HeapManager::~HeapManager() {}

//初期化
bool HeapManager::Init(IDirectXAccessor& device) {
  DescriptorHeap::Desc global_desc = {};
  global_desc.descriptor_num = GLOBAL_HEAP_DESCRIPTOR_NUM;
  global_desc.type = HeapType::CBV_SRV_UAV;
  global_desc.flag = HeapFlag::ShaderVisible;

  if (!global_heap_.Init(device, global_desc, L"GlobalHeap")) {
    return false;
  }

  DescriptorHeap::Desc local_desc = {};
  local_desc.descriptor_num = LOCAL_HEAP_DESCRIPTOR_NUM;
  local_desc.type = HeapType::CBV_SRV_UAV;
  local_desc.flag = HeapFlag::
      None;  //ローカルは実際にGPUに転送するわけではないのでフラグは必要なし
  if (!local_heap_.Init(device, local_desc, L"LocalHeap")) {
    return false;
  }

  this->local_heap_allocated_count_ = 0;
  this->global_heap_allocated_count_ = 0;
  return true;
}

//ローカルのヒープハンドルを取得する
DescriptorHandle HeapManager::GetLocalHandle() {
  const DescriptorHandle res{
      local_heap_.GetCPUHandle(local_heap_allocated_count_),
      local_heap_.GetGPUHandle(local_heap_allocated_count_)};
  local_heap_allocated_count_++;
  return res;
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
  static const auto SetToHandlesAndAppendIfNeed =
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
  auto next = [&](u32 index,
                  const std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>& handles) {
    u32 count = static_cast<u32>(handles.size());
    D3D12_CPU_DESCRIPTOR_HANDLE dst_handle =
        global_heap_.GetCPUHandle(global_heap_allocated_count_);
    device.GetDevice()->CopyDescriptors(
        1, &dst_handle, &count, count, handles.data(), nullptr,
        D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    device.GetCommandList()->SetGraphicsRootDescriptorTable(
        index, global_heap_.GetGPUHandle(global_heap_allocated_count_));
    global_heap_allocated_count_ += count;
  };

  next(0, cbv_handles_);
  next(1, srv_handles_);
}

}  // namespace directx
}  // namespace legend

#include "src/directx/descriptor_heap/heap_manager.h"

#include "src/directx/descriptor_heap/heap_parameter.h"
#include "src/directx/shader/root_parameter_index.h"
#include "src/util/stl_extend.h"

namespace legend {
namespace directx {
namespace descriptor_heap {

//コンストラクタ
HeapManager::HeapManager() {}

//デストラクタ
HeapManager::~HeapManager() {}

//初期化
bool HeapManager::Init(IDirectXAccessor& device) {
  const DescriptorHeap::Desc global_desc(
      L"GlobalHeap", heap_parameter::GLOBAL_HEAP_DESCRIPTOR_NUM,
      DescriptorHeapType::CBV_SRV_UAV, DescriptorHeapFlag::SHADER_VISIBLE);

  if (!global_heap_.Init(device, global_desc)) {
    return false;
  }

  const DescriptorHeap::Desc local_desc(
      L"LocalHeap",
      heap_parameter::local::GetDefinedLocalDescriptorNum(
          heap_parameter::LocalHeapID::GLOBAL_ID),
      DescriptorHeapType::CBV_SRV_UAV, DescriptorHeapFlag::NONE);
  if (!local_heaps_[heap_parameter::LocalHeapID::GLOBAL_ID].Init(device,
                                                                 local_desc)) {
    return false;
  }

  const DescriptorHeap::Desc rtv_desc(
      L"RTVHeap", heap_parameter::RTV_HEAP_DESCRIPTOR_NUM,
      DescriptorHeapType::RTV, DescriptorHeapFlag::NONE);
  if (!rtv_heap_.Init(device, rtv_desc)) {
    return false;
  }

  const DescriptorHeap::Desc dsv_desc(
      L"DSVHeap", heap_parameter::DSV_HEAP_DESCRIPTOR_NUM,
      DescriptorHeapType::DSV, DescriptorHeapFlag::NONE);
  if (!dsv_heap_.Init(device, dsv_desc)) {
    return false;
  }

  this->global_heap_allocated_count_ = 0;
  this->default_handle_ =
      local_heaps_[heap_parameter::LocalHeapID::GLOBAL_ID].GetHandle();
  return true;
}

//フレーム開始時イベント
void HeapManager::BeginNewFrame() { global_heap_allocated_count_ = 0; }

//コマンドリストにグローバルヒープをセットする
void HeapManager::SetHeapToCommandList(IDirectXAccessor& device) const {
  ID3D12DescriptorHeap* heaps[] = {global_heap_.GetHeap()};
  device.GetCommandList()->SetDescriptorHeaps(1, heaps);
}

//ローカルヒープにハンドルをセットする
void HeapManager::SetHandleToLocalHeap(u32 register_num, ResourceType type,
                                       D3D12_CPU_DESCRIPTOR_HANDLE handle) {
  //必要に応じて配列を拡張しつつ、ハンドルをセットする
  //レジスター番号に合わせてハンドルをセットする
  auto SetToHandlesAndAppendIfNeed =
      [&](u32 register_num, std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>* handles,
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
  auto PaddingNullHandle =
      [&](std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>* handle) {
        const u32 size = static_cast<u32>(handle->size());
        for (u32 i = 0; i < size; i++) {
          if ((*handle)[i].ptr == 0) (*handle)[i] = default_handle_.cpu_handle_;
        }
      };

  PaddingNullHandle(&cbv_handles_);
  PaddingNullHandle(&srv_handles_);

  auto CopyAndSetToCommandList =
      [&](u32 index, const std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>& handles) {
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

  CopyAndSetToCommandList(shader::root_parameter_index::CBV, cbv_handles_);
  CopyAndSetToCommandList(shader::root_parameter_index::SRV, srv_handles_);
}

//ローカルヒープを追加するs
bool HeapManager::AddLocalHeap(IDirectXAccessor& accessor,
                               heap_parameter::LocalHeapID id) {
  if (util::Exist(local_heaps_, id)) {
    MY_LOG(L"すでに追加済みのヒープIDが追加されようとしました。");
    return false;
  }

  std::wstringstream wss;
  wss << L"LocalHeap_" << static_cast<u32>(id);
  const DescriptorHeap::Desc desc{
      wss.str(), heap_parameter::local::GetDefinedLocalDescriptorNum(id)};
  return local_heaps_[id].Init(accessor, desc);
}

void HeapManager::ResetLocalHeapAllocateCounter(
    heap_parameter::LocalHeapID id) {
  MY_ASSERTION(util::Exist(local_heaps_, id), L"未登録のIDが送られました。");
  local_heaps_[id].ResetAllocateCounter();
}
// IDに対応したローカルヒープを返す
CountingDescriptorHeap* HeapManager::GetLocalHeap(
    heap_parameter::LocalHeapID id) {
  MY_ASSERTION(util::Exist(local_heaps_, id), L"未登録のIDが送られました。");

  return &local_heaps_.at(id);
}

}  // namespace descriptor_heap
}  // namespace directx
}  // namespace legend

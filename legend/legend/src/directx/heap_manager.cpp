#include "src/directx/heap_manager.h"

namespace legend {
namespace directx {

//コンストラクタ
HeapManager::HeapManager() {}

//デストラクタ
HeapManager::~HeapManager() {}

//初期化
bool HeapManager::Init(IDirectXAccessor& device) {
  DescriptorHeap::Desc global_desc = {};
  global_desc.descriptor_num = 10000;
  global_desc.type = HeapType::CBV_SRV_UAV;
  global_desc.flag = HeapFlag::ShaderVisible;

  if (!global_heap_.Init(device, global_desc, L"GlobalHeap")) {
    return false;
  }

  DescriptorHeap::Desc local_desc = {};
  local_desc.descriptor_num = 10000;
  local_desc.type = HeapType::CBV_SRV_UAV;
  local_desc.flag = HeapFlag::None;

  if (!local_heap_.Init(device, local_desc, L"LocalHeap")) {
    return false;
  }

  this->local_heap_allocated_count_ = 0;
  return true;
}

//
DescriptorHandle HeapManager::GetLocalHandle() {
  DescriptorHandle res{local_heap_.GetCPUHandle(local_heap_allocated_count_),
                       local_heap_.GetGPUHandle(local_heap_allocated_count_)};
  local_heap_allocated_count_++;
  return res;
}

void HeapManager::SetGraphicsCommandList(IDirectXAccessor& device) {
  ID3D12DescriptorHeap* heaps[] = {global_heap_.GetHeap()};
  device.GetCommandList()->SetDescriptorHeaps(1, heaps);
}

void HeapManager::StackLocalHeap(u32 register_num, ResourceType type,
                                 D3D12_CPU_DESCRIPTOR_HANDLE handle) {
  auto SetToHandlesAndAppendIfNeed =
      [](u32 register_num, std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>* handles,
         D3D12_CPU_DESCRIPTOR_HANDLE handle) {
        if (handles->size() <= register_num) {
          handles->resize(register_num + 1);
        }
        (*handles)[register_num] = handle;
      };

  switch (type) {
    case legend::directx::ResourceType::Cbv:
      SetToHandlesAndAppendIfNeed(register_num, &cbv_handles, handle);
      break;
    case legend::directx::ResourceType::Srv:
      SetToHandlesAndAppendIfNeed(register_num, &srv_handles, handle);
      break;
    default:
      break;
  }
}

void HeapManager::PushToGlobalHeapAndSetCommandList(IDirectXAccessor& device) {
  {
    u32 count = static_cast<u32>(cbv_handles.size());
    D3D12_CPU_DESCRIPTOR_HANDLE dst_handle =
        global_heap_.GetCPUHandle(global_heap_index_);
    device.GetDevice()->CopyDescriptors(
        1, &dst_handle, &count, count, cbv_handles.data(), nullptr,
        D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    device.GetCommandList()->SetGraphicsRootDescriptorTable(
        0, global_heap_.GetGPUHandle(global_heap_index_));
    global_heap_index_ += count;
  }
  {
    u32 count = static_cast<u32>(srv_handles.size());
    D3D12_CPU_DESCRIPTOR_HANDLE dst_handle =
        global_heap_.GetCPUHandle(global_heap_index_);
    device.GetDevice()->CopyDescriptors(
        1, &dst_handle, &count, count, srv_handles.data(), nullptr,
        D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    device.GetCommandList()->SetGraphicsRootDescriptorTable(
        1, global_heap_.GetGPUHandle(global_heap_index_));
    global_heap_index_ += count;
  }
}

}  // namespace directx
}  // namespace legend

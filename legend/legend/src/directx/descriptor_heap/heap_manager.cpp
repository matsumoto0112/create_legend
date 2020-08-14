#include "src/directx/descriptor_heap/heap_manager.h"

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
bool HeapManager::Init(device::IDirectXAccessor& accessor) {
  //グローバルヒープ
  {
    const DescriptorHeap::Desc global_desc{
        L"GlobalHeap", heap_parameter::GLOBAL_HEAP_DESCRIPTOR_NUM,
        DescriptorHeapType::CBV_SRV_UAV, DescriptorHeapFlag::SHADER_VISIBLE};
    if (!global_heap_.Init(accessor, global_desc)) {
      return false;
    }
  }  // namespace descriptor_heap

  //グローバルで使用するローカルヒープ
  {
    auto CreateLocalHeap = [&](heap_parameter::LocalHeapID id,
                               const std::wstring& name) {
      const u32 descriptor_num =
          heap_parameter::local::GetDefinedLocalDescriptorNum(id);
      const DescriptorHeap::Desc local_desc{name, descriptor_num,
                                            DescriptorHeapType::CBV_SRV_UAV,
                                            DescriptorHeapFlag::NONE};
      if (!local_heaps_[id].Init(accessor, local_desc)) {
        return false;
      }
      return true;
    };
    if (!CreateLocalHeap(heap_parameter::LocalHeapID::GLOBAL_ID,
                         L"GlobalHeap")) {
      return false;
    }
    if (!CreateLocalHeap(heap_parameter::LocalHeapID::ONE_PLAY,
                         L"OnePlayHeap")) {
      return false;
    }
  }  // namespace directx

  // RTVヒープ
  {
    const DescriptorHeap::Desc rtv_desc{
        L"RTVHeap", heap_parameter::RTV_HEAP_DESCRIPTOR_NUM,
        DescriptorHeapType::RTV, DescriptorHeapFlag::NONE};
    if (!rtv_heap_.Init(accessor, rtv_desc)) {
      return false;
    }
  }

  // DSVヒープ
  {
    const DescriptorHeap::Desc dsv_desc{
        L"DSVHeap", heap_parameter::DSV_HEAP_DESCRIPTOR_NUM,
        DescriptorHeapType::DSV, DescriptorHeapFlag::NONE};
    if (!dsv_heap_.Init(accessor, dsv_desc)) {
      return false;
    }
  }

  this->global_heap_allocated_count_ = 0;

  //デフォルトのコンスタントバッファ用ハンドルを作成
  this->default_handle_.default_cbv_handle_ =
      local_heaps_[heap_parameter::LocalHeapID::GLOBAL_ID].GetHandle();
  accessor.GetDevice()->CreateConstantBufferView(
      nullptr, default_handle_.default_cbv_handle_.cpu_handle_);

  this->default_handle_.default_srv_handle_ =
      local_heaps_[heap_parameter::LocalHeapID::GLOBAL_ID].GetHandle();

  D3D12_SHADER_RESOURCE_VIEW_DESC null_srv_desc = {};
  null_srv_desc.ViewDimension =
      D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2D;
  null_srv_desc.Shader4ComponentMapping =
      D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
  null_srv_desc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
  null_srv_desc.Texture2D.MipLevels = 1;
  null_srv_desc.Texture2D.MostDetailedMip = 0;
  null_srv_desc.Texture2D.ResourceMinLODClamp = 0.0f;

  accessor.GetDevice()->CreateShaderResourceView(
      nullptr, &null_srv_desc, default_handle_.default_srv_handle_.cpu_handle_);

  this->default_handle_.default_uav_handle_ =
      local_heaps_[heap_parameter::LocalHeapID::GLOBAL_ID].GetHandle();
  D3D12_UNORDERED_ACCESS_VIEW_DESC null_uav_desc = {};
  null_uav_desc.ViewDimension =
      D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_TEXTURE2D;
  null_uav_desc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
  accessor.GetDevice()->CreateUnorderedAccessView(
      nullptr, nullptr, &null_uav_desc,
      this->default_handle_.default_uav_handle_.cpu_handle_);

  return true;
}  // namespace legend

void HeapManager::BeginFrame() { global_heap_allocated_count_ = 0; }

void HeapManager::SetCommandList(device::CommandList& command_list) const {
  ID3D12DescriptorHeap* const heaps[] = {global_heap_.GetHeap()};
  command_list.GetCommandList()->SetDescriptorHeaps(_countof(heaps), heaps);
}

void HeapManager::RegisterHandle(u32 register_num, shader::ResourceType type,
                                 DescriptorHandle handle) {
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
    case shader::ResourceType::CBV:
      SetToHandlesAndAppendIfNeed(register_num,
                                  &current_local_handles_.cbv_handles_,
                                  handle.cpu_handle_);
      break;
    case shader::ResourceType::SRV:
      SetToHandlesAndAppendIfNeed(register_num,
                                  &current_local_handles_.srv_handles_,
                                  handle.cpu_handle_);
      break;
    case shader::ResourceType::UAV:
      SetToHandlesAndAppendIfNeed(register_num,
                                  &current_local_handles_.uav_handles_,
                                  handle.cpu_handle_);
    default:
      break;
  }
}

void HeapManager::SetHeapTableToGraphicsCommandList(
    device::IDirectXAccessor& accessor, device::CommandList& command_list) {
  auto PaddingNullHandle = [&](std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>* handle,
                               D3D12_CPU_DESCRIPTOR_HANDLE default_handle) {
    const u32 size = static_cast<u32>(handle->size());
    for (u32 i = 0; i < size; i++) {
      if ((*handle)[i].ptr == 0) (*handle)[i] = default_handle;
    }
  };

  PaddingNullHandle(&current_local_handles_.cbv_handles_,
                    default_handle_.default_cbv_handle_.cpu_handle_);
  PaddingNullHandle(&current_local_handles_.srv_handles_,
                    default_handle_.default_srv_handle_.cpu_handle_);
  PaddingNullHandle(&current_local_handles_.uav_handles_,
                    default_handle_.default_uav_handle_.cpu_handle_);

  auto CopyAndSetToCommandList =
      [&](u32 index, const std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>& handles) {
        //必要な数だけローカルからグローバルにコピーする
        const u32 count = static_cast<u32>(handles.size());
        if (count == 0) return;
        const DescriptorHandle global_handle =
            global_heap_.GetHandle(global_heap_allocated_count_);
        D3D12_CPU_DESCRIPTOR_HANDLE dst_handle = global_handle.cpu_handle_;
        accessor.GetDevice()->CopyDescriptors(
            1, &dst_handle, &count, count, handles.data(), nullptr,
            D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

        command_list.GetCommandList()->SetGraphicsRootDescriptorTable(
            index, global_handle.gpu_handle_);
        global_heap_allocated_count_ += count;
      };

  CopyAndSetToCommandList(shader::root_parameter_index::CBV,
                          current_local_handles_.cbv_handles_);
  CopyAndSetToCommandList(shader::root_parameter_index::SRV,
                          current_local_handles_.srv_handles_);
  CopyAndSetToCommandList(shader::root_parameter_index::UAV,
                          current_local_handles_.uav_handles_);
}

void HeapManager::SetHeapTableToComputeCommandList(
    device::IDirectXAccessor& accessor, device::CommandList& command_list) {
  auto PaddingNullHandle = [&](std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>* handle,
                               D3D12_CPU_DESCRIPTOR_HANDLE default_handle) {
    const u32 size = static_cast<u32>(handle->size());
    for (u32 i = 0; i < size; i++) {
      if ((*handle)[i].ptr == 0) (*handle)[i] = default_handle;
    }
  };

  PaddingNullHandle(&current_local_handles_.cbv_handles_,
                    default_handle_.default_cbv_handle_.cpu_handle_);
  PaddingNullHandle(&current_local_handles_.srv_handles_,
                    default_handle_.default_srv_handle_.cpu_handle_);
  PaddingNullHandle(&current_local_handles_.uav_handles_,
                    default_handle_.default_uav_handle_.cpu_handle_);

  auto CopyAndSetToCommandList =
      [&](u32 index, const std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>& handles) {
        //必要な数だけローカルからグローバルにコピーする
        const u32 count = static_cast<u32>(handles.size());
        if (count == 0) return;
        const DescriptorHandle global_handle =
            global_heap_.GetHandle(global_heap_allocated_count_);
        D3D12_CPU_DESCRIPTOR_HANDLE dst_handle = global_handle.cpu_handle_;
        accessor.GetDevice()->CopyDescriptors(
            1, &dst_handle, &count, count, handles.data(), nullptr,
            D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

        command_list.GetCommandList()->SetComputeRootDescriptorTable(
            index, global_handle.gpu_handle_);
        global_heap_allocated_count_ += count;
      };

  CopyAndSetToCommandList(shader::root_parameter_index::CBV,
                          current_local_handles_.cbv_handles_);
  CopyAndSetToCommandList(shader::root_parameter_index::SRV,
                          current_local_handles_.srv_handles_);
  CopyAndSetToCommandList(shader::root_parameter_index::UAV,
                          current_local_handles_.uav_handles_);
}

bool HeapManager::AddLocalHeap(device::IDirectXAccessor& accessor,
                               heap_parameter::LocalHeapID heap_id) {
  if (util::Exist(local_heaps_, heap_id)) {
    MY_LOG(L"すでに追加済みのヒープIDが追加されようとしました。 %d",
           static_cast<u32>(heap_id));
    return false;
  }

  std::wstringstream wss;
  wss << L"LocalHeap_" << static_cast<u32>(heap_id);
  const u32 descriptor_num =
      heap_parameter::local::GetDefinedLocalDescriptorNum(heap_id);
  if (descriptor_num == heap_parameter::local::UNDEFINED_DESCRIPTOR_NUM) {
    MY_LOG(L"割り当てディスクリプタ数が不正です。");
    return false;
  }

  const DescriptorHeap::Desc desc{wss.str(), descriptor_num,
                                  DescriptorHeapType::CBV_SRV_UAV,
                                  DescriptorHeapFlag::NONE};
  return local_heaps_[heap_id].Init(accessor, desc);
}

void HeapManager::RemoveLocalHeap(heap_parameter::LocalHeapID heap_id) {
  if (!util::Exist(local_heaps_, heap_id)) {
    MY_LOG(L"未割当のヒープIDが選択されました。 %d", static_cast<u32>(heap_id));
    return;
  }

  local_heaps_.erase(heap_id);
  MY_LOG(L"%d", local_heaps_.size());
}

void HeapManager::ResetLocalHeap(heap_parameter::LocalHeapID heap_id) {
  MY_ASSERTION(util::Exist(local_heaps_, heap_id), L"heap_idが無効です。");
  local_heaps_.at(heap_id).ResetAllocateCounter();
}

DescriptorHandle HeapManager::GetLocalHeap(
    heap_parameter::LocalHeapID heap_id) {
  MY_ASSERTION(util::Exist(local_heaps_, heap_id), L"heap_idが無効です。");
  return local_heaps_.at(heap_id).GetHandle();
}

}  // namespace descriptor_heap
}  // namespace directx
}  // namespace legend

#include "src/directx/descriptor_heap/heap_manager.h"

#include "src/directx/shader/root_parameter_index.h"

namespace legend {
namespace directx {
namespace descriptor_heap {

//�R���X�g���N�^
HeapManager::HeapManager() {}

//�f�X�g���N�^
HeapManager::~HeapManager() {}

//������
bool HeapManager::Init(IDirectXAccessor& device) {
  //�O���[�o���q�[�v
  {
    const DescriptorHeap::Desc global_desc{
        L"GlobalHeap", heap_parameter::GLOBAL_HEAP_DESCRIPTOR_NUM,
        DescriptorHeapType::CBV_SRV_UAV, DescriptorHeapFlag::SHADER_VISIBLE};
    if (!global_heap_.Init(device, global_desc)) {
      return false;
    }
  }

  //�O���[�o���Ŏg�p���郍�[�J���q�[�v
  {
    const u32 descriptor_num =
        heap_parameter::local::GetDefinedLocalDescriptorNum(
            heap_parameter::LocalHeapID::GLOBAL_ID);
    const DescriptorHeap::Desc local_desc{L"LocalHeap", descriptor_num,
                                          DescriptorHeapType::CBV_SRV_UAV,
                                          DescriptorHeapFlag::NONE};
    if (!local_heaps_[heap_parameter::LocalHeapID::GLOBAL_ID].Init(
            device, local_desc)) {
      return false;
    }
  }

  // RTV�q�[�v
  {
    const DescriptorHeap::Desc rtv_desc{
        L"RTVHeap", heap_parameter::RTV_HEAP_DESCRIPTOR_NUM,
        DescriptorHeapType::RTV, DescriptorHeapFlag::NONE};
    if (!rtv_heap_.Init(device, rtv_desc)) {
      return false;
    }
  }

  // DSV�q�[�v
  {
    const DescriptorHeap::Desc dsv_desc{
        L"DSVHeap", heap_parameter::DSV_HEAP_DESCRIPTOR_NUM,
        DescriptorHeapType::DSV, DescriptorHeapFlag::NONE};
    if (!dsv_heap_.Init(device, dsv_desc)) {
      return false;
    }
  }

  this->global_heap_allocated_count_ = 0;

  //�f�t�H���g�̃R���X�^���g�o�b�t�@�p�n���h�����쐬
  this->default_handle_.default_cbv_handle_ =
      local_heaps_[heap_parameter::LocalHeapID::GLOBAL_ID].GetHandle();
  device.GetDevice()->CreateConstantBufferView(
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

  device.GetDevice()->CreateShaderResourceView(
      nullptr, &null_srv_desc, default_handle_.default_srv_handle_.cpu_handle_);
  return true;
}

void HeapManager::BeginFrame() { global_heap_allocated_count_ = 0; }

void HeapManager::SetGraphicsCommandList(
    device::CommandList& command_list) const {
  ID3D12DescriptorHeap* const heaps[] = {global_heap_.GetHeap()};
  command_list.GetCommandList()->SetDescriptorHeaps(_countof(heaps), heaps);
}

void HeapManager::SetHandleToLocalHeap(u32 register_num, ResourceType type,
                                       DescriptorHandle handle) {
  //�K�v�ɉ����Ĕz����g�����A�n���h�����Z�b�g����
  //���W�X�^�[�ԍ��ɍ��킹�ăn���h�����Z�b�g����
  auto SetToHandlesAndAppendIfNeed =
      [&](u32 register_num, std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>* handles,
          D3D12_CPU_DESCRIPTOR_HANDLE handle) {
        //���W�X�^�[�ԍ���茻�݃T�C�Y������������Α���Ȃ����g������
        if (handles->size() <= register_num) {
          handles->resize(register_num + 1);
        }
        (*handles)[register_num] = handle;
      };

  //��ނɉ����Ēǉ����ς���
  switch (type) {
    case ResourceType::Cbv:
      SetToHandlesAndAppendIfNeed(register_num,
                                  &current_local_handles_.cbv_handles_,
                                  handle.cpu_handle_);
      break;
    case ResourceType::Srv:
      SetToHandlesAndAppendIfNeed(register_num,
                                  &current_local_handles_.srv_handles_,
                                  handle.cpu_handle_);
      break;
    default:
      break;
  }
}

void HeapManager::UpdateGlobalHeap(ID3D12Device* device,
                                   device::CommandList& command_list) {
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

  auto CopyAndSetToCommandList =
      [&](u32 index, const std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>& handles) {
        //�K�v�Ȑ��������[�J������O���[�o���ɃR�s�[����
        const u32 count = static_cast<u32>(handles.size());
        if (count == 0) return;
        const DescriptorHandle global_handle =
            global_heap_.GetHandle(global_heap_allocated_count_);
        D3D12_CPU_DESCRIPTOR_HANDLE dst_handle = global_handle.cpu_handle_;
        device->CopyDescriptors(
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
}

bool HeapManager::AddLocalHeap(IDirectXAccessor& accessor,
                               heap_parameter::LocalHeapID heap_id) {
  if (util::Exist(local_heaps_, heap_id)) {
    MY_LOG(L"���łɒǉ��ς݂̃q�[�vID���ǉ�����悤�Ƃ��܂����B %d",
           static_cast<u32>(heap_id));
    return false;
  }

  std::wstringstream wss;
  wss << L"LocalHeap_" << static_cast<u32>(heap_id);
  const u32 descriptor_num =
      heap_parameter::local::GetDefinedLocalDescriptorNum(heap_id);
  if (descriptor_num == heap_parameter::local::UNDEFINED_DESCRIPTOR_NUM) {
    MY_LOG(L"���蓖�ăf�B�X�N���v�^�����s���ł��B");
    return false;
  }

  const DescriptorHeap::Desc desc{wss.str(), descriptor_num,
                                  DescriptorHeapType::CBV_SRV_UAV,
                                  DescriptorHeapFlag::NONE};
  return local_heaps_[heap_id].Init(accessor, desc);
}

void HeapManager::RemoveLocalHeap(heap_parameter::LocalHeapID heap_id) {
  if (!util::Exist(local_heaps_, heap_id)) {
    MY_LOG(L"�������̃q�[�vID���I������܂����B %d", static_cast<u32>(heap_id));
    return;
  }

  local_heaps_.erase(heap_id);
}

////�q�[�v���R�s�[���R�}���h���X�g�ɃZ�b�g����
// void HeapManager::CopyHeapAndSetToGraphicsCommandList(
//    IDirectXAccessor& device) {
//  auto PaddingNullHandle =
//      [&](std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>* handle) {
//        const u32 size = static_cast<u32>(handle->size());
//        for (u32 i = 0; i < size; i++) {
//          if ((*handle)[i].ptr == 0) (*handle)[i] =
//          default_handle_.cpu_handle_;
//        }
//      };
//
//  PaddingNullHandle(&cbv_handles_);
//  PaddingNullHandle(&srv_handles_);
//
//  auto CopyAndSetToCommandList =
//      [&](u32 index, const std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>& handles)
//      {
//        //�K�v�Ȑ��������[�J������O���[�o���ɃR�s�[����
//        u32 count = static_cast<u32>(handles.size());
//        if (count == 0) return;
//        DescriptorHandle global_handle =
//            global_heap_.GetHandle(global_heap_allocated_count_);
//        D3D12_CPU_DESCRIPTOR_HANDLE dst_handle = global_handle.cpu_handle_;
//        device.GetDevice()->CopyDescriptors(
//            1, &dst_handle, &count, count, handles.data(), nullptr,
//            D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
//
//        device.GetCommandList()->SetGraphicsRootDescriptorTable(
//            index, global_handle.gpu_handle_);
//        global_heap_allocated_count_ += count;
//      };
//
//  CopyAndSetToCommandList(shader::root_parameter_index::CBV, cbv_handles_);
//  CopyAndSetToCommandList(shader::root_parameter_index::SRV, srv_handles_);
//}
//
////���[�J���q�[�v��ǉ�����s
// bool HeapManager::AddLocalHeap(IDirectXAccessor& accessor,
//                               heap_parameter::LocalHeapID id) {
//  if (util::Exist(local_heaps_, id)) {
//    MY_LOG(L"���łɒǉ��ς݂̃q�[�vID���ǉ�����悤�Ƃ��܂����B");
//    return false;
//  }
//
//  std::wstringstream wss;
//  wss << L"LocalHeap_" << static_cast<u32>(id);
//  const DescriptorHeap::Desc desc{
//      wss.str(), heap_parameter::local::GetDefinedLocalDescriptorNum(id)};
//
//  if (desc.descriptor_num == heap_parameter::local::UNDEFINED_DESCRIPTOR_NUM)
//  {
//    return false;
//  }
//
//  return local_heaps_[id].Init(accessor, desc);
//}
//
// void HeapManager::ResetLocalHeapAllocateCounter(
//    heap_parameter::LocalHeapID id) {
//  MY_ASSERTION(util::Exist(local_heaps_, id), L"���o�^��ID�������܂����B");
//  local_heaps_[id].ResetAllocateCounter();
//}
//// ID�ɑΉ��������[�J���q�[�v��Ԃ�
// CountingDescriptorHeap* HeapManager::GetLocalHeap(
//    heap_parameter::LocalHeapID id) {
//  MY_ASSERTION(util::Exist(local_heaps_, id), L"���o�^��ID�������܂����B");
//
//  return &local_heaps_.at(id);
//}

}  // namespace descriptor_heap
}  // namespace directx
}  // namespace legend

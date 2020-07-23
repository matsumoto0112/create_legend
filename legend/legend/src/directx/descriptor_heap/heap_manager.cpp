#include "src/directx/descriptor_heap/heap_manager.h"

#include "src/directx/descriptor_heap/heap_parameter.h"
#include "src/directx/shader/root_parameter_index.h"
#include "src/util/stl_extend.h"

namespace legend {
namespace directx {
namespace descriptor_heap {

//�R���X�g���N�^
HeapManager::HeapManager() {}

//�f�X�g���N�^
HeapManager::~HeapManager() {}

//������
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

//�t���[���J�n���C�x���g
void HeapManager::BeginNewFrame() { global_heap_allocated_count_ = 0; }

//�R�}���h���X�g�ɃO���[�o���q�[�v���Z�b�g����
void HeapManager::SetHeapToCommandList(IDirectXAccessor& device) const {
  ID3D12DescriptorHeap* heaps[] = {global_heap_.GetHeap()};
  device.GetCommandList()->SetDescriptorHeaps(1, heaps);
}

//���[�J���q�[�v�Ƀn���h�����Z�b�g����
void HeapManager::SetHandleToLocalHeap(u32 register_num, ResourceType type,
                                       D3D12_CPU_DESCRIPTOR_HANDLE handle) {
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

//�q�[�v���R�s�[���R�}���h���X�g�ɃZ�b�g����
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
        //�K�v�Ȑ��������[�J������O���[�o���ɃR�s�[����
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

//���[�J���q�[�v��ǉ�����s
bool HeapManager::AddLocalHeap(IDirectXAccessor& accessor,
                               heap_parameter::LocalHeapID id) {
  if (util::Exist(local_heaps_, id)) {
    MY_LOG(L"���łɒǉ��ς݂̃q�[�vID���ǉ�����悤�Ƃ��܂����B");
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
  MY_ASSERTION(util::Exist(local_heaps_, id), L"���o�^��ID�������܂����B");
  local_heaps_[id].ResetAllocateCounter();
}
// ID�ɑΉ��������[�J���q�[�v��Ԃ�
CountingDescriptorHeap* HeapManager::GetLocalHeap(
    heap_parameter::LocalHeapID id) {
  MY_ASSERTION(util::Exist(local_heaps_, id), L"���o�^��ID�������܂����B");

  return &local_heaps_.at(id);
}

}  // namespace descriptor_heap
}  // namespace directx
}  // namespace legend

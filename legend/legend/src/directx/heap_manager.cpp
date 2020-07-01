#include "src/directx/heap_manager.h"

namespace {

//! �O���[�o���q�[�v�̍쐬����f�B�X�N���v�^��
constexpr legend::u32 GLOBAL_HEAP_DESCRIPTOR_NUM = 10000;
//! ���[�J���q�[�v�̍쐬����f�B�X�N���v�^��
constexpr legend::u32 LOCAL_HEAP_DESCRIPTOR_NUM = 10000;
}  // namespace

namespace legend {
namespace directx {

//�R���X�g���N�^
HeapManager::HeapManager() {}

//�f�X�g���N�^
HeapManager::~HeapManager() {}

//������
bool HeapManager::Init(IDirectXAccessor& device) {
  DescriptorHeap::Desc global_desc(L"GlobalHeap", GLOBAL_HEAP_DESCRIPTOR_NUM,
                                   HeapType::CBV_SRV_UAV,
                                   HeapFlag::ShaderVisible);

  if (!global_heap_.Init(device, global_desc)) {
    return false;
  }

  DescriptorHeap::Desc local_desc(L"LocalHeap", LOCAL_HEAP_DESCRIPTOR_NUM,
                                  HeapType::CBV_SRV_UAV, HeapFlag::None);
  if (!cbv_srv_uav_heap_.Init(device, local_desc)) {
    return false;
  }

  DescriptorHeap::Desc rtv_desc(L"RTVHeap", 100, HeapType::RTV, HeapFlag::None);
  if (!rtv_heap_.Init(device, rtv_desc)) {
    return false;
  }

  DescriptorHeap::Desc dsv_desc(L"DSVHeap", 5, HeapType::DSV, HeapFlag::None);
  if (!dsv_heap_.Init(device, dsv_desc)) {
    return false;
  }

  this->global_heap_allocated_count_ = 0;
  return true;
}

//���[�J���̃q�[�v�n���h�����擾����
DescriptorHandle HeapManager::GetLocalHandle() {
  return cbv_srv_uav_heap_.GetHandle();
}

//�t���[���J�n���C�x���g
void HeapManager::BeginFrame() { global_heap_allocated_count_ = 0; }

//�R�}���h���X�g�ɃO���[�o���q�[�v���Z�b�g����
void HeapManager::SetGraphicsCommandList(IDirectXAccessor& device) const {
  ID3D12DescriptorHeap* heaps[] = {global_heap_.GetHeap()};
  device.GetCommandList()->SetDescriptorHeaps(1, heaps);
}

//���[�J���q�[�v�Ƀn���h�����Z�b�g����
void HeapManager::SetHandleToLocalHeap(u32 register_num, ResourceType type,
                                       D3D12_CPU_DESCRIPTOR_HANDLE handle) {
  //�K�v�ɉ����Ĕz����g�����A�n���h�����Z�b�g����
  //���W�X�^�[�ԍ��ɍ��킹�ăn���h�����Z�b�g����
  static const auto SetToHandlesAndAppendIfNeed =
      [](u32 register_num, std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>* handles,
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
  auto SetTo = [&](u32 index,
                   const std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>& handles) {
    //�K�v�Ȑ��������[�J������O���[�o���ɃR�s�[����
    u32 count = static_cast<u32>(handles.size());
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

  SetTo(0, cbv_handles_);
  SetTo(1, srv_handles_);
}

}  // namespace directx
}  // namespace legend

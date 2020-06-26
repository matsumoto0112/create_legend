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
      None;  //���[�J���͎��ۂ�GPU�ɓ]������킯�ł͂Ȃ��̂Ńt���O�͕K�v�Ȃ�
  if (!local_heap_.Init(device, local_desc, L"LocalHeap")) {
    return false;
  }

  this->local_heap_allocated_count_ = 0;
  this->global_heap_allocated_count_ = 0;
  return true;
}

//���[�J���̃q�[�v�n���h�����擾����
DescriptorHandle HeapManager::GetLocalHandle() {
  const DescriptorHandle res{
      local_heap_.GetCPUHandle(local_heap_allocated_count_),
      local_heap_.GetGPUHandle(local_heap_allocated_count_)};
  local_heap_allocated_count_++;
  return res;
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

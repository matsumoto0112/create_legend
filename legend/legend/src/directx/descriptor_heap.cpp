#include "src/directx/descriptor_heap.h"

namespace {
//! �q�[�v�̎�ރe�[�u��
static const std::unordered_map<legend::directx::HeapType,
                                D3D12_DESCRIPTOR_HEAP_TYPE>
    HEAP_TYPES{
        {legend::directx::HeapType::CBV_SRV_UAV,
         D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV},
        {legend::directx::HeapType::Sampler,
         D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER},
        {legend::directx::HeapType::RTV,
         D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV},
        {legend::directx::HeapType::DSV,
         D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV},
    };

//! �q�[�v�̃t���O�e�[�u��
static const std::unordered_map<legend::directx::HeapFlag,
                                D3D12_DESCRIPTOR_HEAP_FLAGS>
    HEAP_FLAGS{
        {legend::directx::HeapFlag::None,
         D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE},
        {legend::directx::HeapFlag::ShaderVisible,
         D3D12_DESCRIPTOR_HEAP_FLAGS::
             D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE},
    };
}  // namespace

namespace legend {
namespace directx {

// �R���X�g���N�^
DescriptorHeap::DescriptorHeap() : heap_(nullptr), heap_size_(0) {}

// �f�X�g���N�^
DescriptorHeap::~DescriptorHeap() {}

// ������
bool DescriptorHeap::Init(DirectX12Device& device, const Desc& desc,
                          const std::wstring& name) {
  this->heap_.Reset();
  this->heap_size_ = 0;

  MY_ASSERTION(HEAP_TYPES.find(desc.type) != HEAP_TYPES.end(),
               L"���o�^��HeapType���I������܂����B");
  MY_ASSERTION(HEAP_FLAGS.find(desc.flag) != HEAP_FLAGS.end(),
               L"���o�^��HeapFlag���I������܂����B");

  D3D12_DESCRIPTOR_HEAP_DESC heap_desc = {};
  heap_desc.NumDescriptors = desc.descriptor_num;
  heap_desc.Type = HEAP_TYPES.at(desc.type);
  heap_desc.Flags = HEAP_FLAGS.at(desc.flag);
  heap_desc.NodeMask = 0;
  if (FAILED(device.GetDevice()->CreateDescriptorHeap(&heap_desc,
                                                      IID_PPV_ARGS(&heap_)))) {
    MY_LOG(L"CreateDescriptorHeap %s failed.", name.c_str());
    return false;
  }

  //�f�o�b�O�p�̖��O��t����
  if (FAILED(heap_->SetName(name.c_str()))) {
    return false;
  }

  this->heap_size_ = device.GetDevice()->GetDescriptorHandleIncrementSize(
      HEAP_TYPES.at(desc.type));
  return true;
}

// CPU�n���h��
D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeap::GetCPUHandle(u32 index) {
  return CD3DX12_CPU_DESCRIPTOR_HANDLE(
      heap_->GetCPUDescriptorHandleForHeapStart(), index, heap_size_);
}

// GPU�n���h��
D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeap::GetGPUHandle(u32 index) {
  return CD3DX12_GPU_DESCRIPTOR_HANDLE(
      heap_->GetGPUDescriptorHandleForHeapStart(), index, heap_size_);
}
}  // namespace directx
}  // namespace legend

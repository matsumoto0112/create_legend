#include "src/directx/descriptor_heap/descriptor_heap.h"

#include "src/directx/directx_helper.h"

namespace {

using legend::directx::descriptor_heap::DescriptorHeapFlag;
using legend::directx::descriptor_heap::DescriptorHeapType;

//! �q�[�v�̎�ރe�[�u��
static const std::unordered_map<DescriptorHeapType, D3D12_DESCRIPTOR_HEAP_TYPE>
    HEAP_TYPES{
        {DescriptorHeapType::CBV_SRV_UAV,
         D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV},
        {DescriptorHeapType::SAMPLER,
         D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER},
        {DescriptorHeapType::RTV,
         D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV},
        {DescriptorHeapType::DSV,
         D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV},
    };

//! �q�[�v�̃t���O�e�[�u��
static const std::unordered_map<DescriptorHeapFlag, D3D12_DESCRIPTOR_HEAP_FLAGS>
    HEAP_FLAGS{
        {DescriptorHeapFlag::NONE,
         D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE},
        {DescriptorHeapFlag::SHADER_VISIBLE,
         D3D12_DESCRIPTOR_HEAP_FLAGS::
             D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE},
    };
}  // namespace

namespace legend {
namespace directx {
namespace descriptor_heap {
using directx_helper::Failed;

// �R���X�g���N�^
DescriptorHeap::DescriptorHeap() : heap_(nullptr), heap_size_(0) {}

// �f�X�g���N�^
DescriptorHeap::~DescriptorHeap() {}

//������
bool DescriptorHeap::Init(IDirectXAccessor& device, const Desc& desc) {
  this->heap_.Reset();
  this->heap_size_ = 0;
  MY_ASSERTION(util::Exist(HEAP_TYPES, desc.type),
               L"���o�^��HeapType���I������܂����B");
  MY_ASSERTION(util::Exist(HEAP_FLAGS, desc.flag),
               L"���o�^��HeapFlag���I������܂����B");

  const D3D12_DESCRIPTOR_HEAP_TYPE heap_type = HEAP_TYPES.at(desc.type);
  const D3D12_DESCRIPTOR_HEAP_FLAGS heap_flags = HEAP_FLAGS.at(desc.flag);

  D3D12_DESCRIPTOR_HEAP_DESC heap_desc = {};
  heap_desc.NumDescriptors = desc.descriptor_num;
  heap_desc.Type = heap_type;
  heap_desc.Flags = heap_flags;
  heap_desc.NodeMask = 0;
  if (Failed(device.GetDevice()->CreateDescriptorHeap(&heap_desc,
                                                      IID_PPV_ARGS(&heap_)))) {
    return false;
  }
  heap_->SetName(desc.name.c_str());

  this->heap_size_ =
      device.GetDevice()->GetDescriptorHandleIncrementSize(heap_type);
  return true;
}

//�n���h����Ԃ�
DescriptorHandle DescriptorHeap::GetHandle(u32 index) const {
  return DescriptorHandle{
      CD3DX12_CPU_DESCRIPTOR_HANDLE(heap_->GetCPUDescriptorHandleForHeapStart(),
                                    index, heap_size_),
      CD3DX12_GPU_DESCRIPTOR_HANDLE(heap_->GetGPUDescriptorHandleForHeapStart(),
                                    index, heap_size_)};
}

}  // namespace descriptor_heap
}  // namespace directx
}  // namespace legend

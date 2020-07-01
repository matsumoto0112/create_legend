#include "src/directx/descriptor_heap/descriptor_heap.h"

namespace {
//! ヒープの種類テーブル
static const std::unordered_map<legend::directx::DescriptorHeapType,
                                D3D12_DESCRIPTOR_HEAP_TYPE>
    HEAP_TYPES{
        {legend::directx::DescriptorHeapType::CBV_SRV_UAV,
         D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV},
        {legend::directx::DescriptorHeapType::SAMPLER,
         D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER},
        {legend::directx::DescriptorHeapType::RTV,
         D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV},
        {legend::directx::DescriptorHeapType::DSV,
         D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV},
    };

//! ヒープのフラグテーブル
static const std::unordered_map<legend::directx::DescriptorHeapFlag,
                                D3D12_DESCRIPTOR_HEAP_FLAGS>
    HEAP_FLAGS{
        {legend::directx::DescriptorHeapFlag::NONE,
         D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE},
        {legend::directx::DescriptorHeapFlag::SHADER_VISIBLE,
         D3D12_DESCRIPTOR_HEAP_FLAGS::
             D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE},
    };
}  // namespace

namespace legend {
namespace directx {

// コンストラクタ
DescriptorHeap::DescriptorHeap() : heap_(nullptr), heap_size_(0) {}

// デストラクタ
DescriptorHeap::~DescriptorHeap() {}

bool DescriptorHeap::Init(IDirectXAccessor& device, const Desc& desc) {
  this->heap_.Reset();
  this->heap_size_ = 0;

  MY_ASSERTION(HEAP_TYPES.find(desc.type) != HEAP_TYPES.end(),
               L"未登録のHeapTypeが選択されました。");
  MY_ASSERTION(HEAP_FLAGS.find(desc.flag) != HEAP_FLAGS.end(),
               L"未登録のHeapFlagが選択されました。");

  D3D12_DESCRIPTOR_HEAP_DESC heap_desc = {};
  heap_desc.NumDescriptors = desc.descriptor_num;
  heap_desc.Type = HEAP_TYPES.at(desc.type);
  heap_desc.Flags = HEAP_FLAGS.at(desc.flag);
  heap_desc.NodeMask = 0;
  if (FAILED(device.GetDevice()->CreateDescriptorHeap(&heap_desc,
                                                      IID_PPV_ARGS(&heap_)))) {
    MY_LOG(L"CreateDescriptorHeap %s failed.", desc.name.c_str());
    return false;
  }

  //デバッグ用の名前を付ける
  if (FAILED(heap_->SetName(desc.name.c_str()))) {
    return false;
  }

  this->heap_size_ = device.GetDevice()->GetDescriptorHandleIncrementSize(
      HEAP_TYPES.at(desc.type));
  return true;
}

//ハンドルを返す
DescriptorHandle DescriptorHeap::GetHandle(u32 index) const {
  return DescriptorHandle{
      CD3DX12_CPU_DESCRIPTOR_HANDLE(heap_->GetCPUDescriptorHandleForHeapStart(),
                                    index, heap_size_),
      CD3DX12_GPU_DESCRIPTOR_HANDLE(heap_->GetGPUDescriptorHandleForHeapStart(),
                                    index, heap_size_)};
}

}  // namespace directx
}  // namespace legend

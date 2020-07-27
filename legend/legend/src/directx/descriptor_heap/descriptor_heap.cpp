#include "src/directx/descriptor_heap/descriptor_heap.h"

#include "src/directx/directx_helper.h"

namespace {
//! ヒープの種類テーブル
static const std::unordered_map<
    legend::directx::descriptor_heap::DescriptorHeapType,
    D3D12_DESCRIPTOR_HEAP_TYPE>
    HEAP_TYPES{
        {legend::directx::descriptor_heap::DescriptorHeapType::CBV_SRV_UAV,
         D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV},
        {legend::directx::descriptor_heap::DescriptorHeapType::SAMPLER,
         D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER},
        {legend::directx::descriptor_heap::DescriptorHeapType::RTV,
         D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV},
        {legend::directx::descriptor_heap::DescriptorHeapType::DSV,
         D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV},
    };

//! ヒープのフラグテーブル
static const std::unordered_map<
    legend::directx::descriptor_heap::DescriptorHeapFlag,
    D3D12_DESCRIPTOR_HEAP_FLAGS>
    HEAP_FLAGS{
        {legend::directx::descriptor_heap::DescriptorHeapFlag::NONE,
         D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE},
        {legend::directx::descriptor_heap::DescriptorHeapFlag::SHADER_VISIBLE,
         D3D12_DESCRIPTOR_HEAP_FLAGS::
             D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE},
    };
}  // namespace

namespace legend {
namespace directx {
namespace descriptor_heap {

// コンストラクタ
DescriptorHeap::DescriptorHeap() : heap_(nullptr), heap_size_(0) {}

// デストラクタ
DescriptorHeap::~DescriptorHeap() {}

//初期化
bool DescriptorHeap::Init(IDirectXAccessor& device, const Desc& desc) {
  this->heap_.Reset();
  this->heap_size_ = 0;

  MY_ASSERTION(util::Exist(HEAP_TYPES, desc.type),
               L"未登録のHeapTypeが選択されました。");
  MY_ASSERTION(util::Exist(HEAP_FLAGS, desc.flag),
               L"未登録のHeapFlagが選択されました。");

  D3D12_DESCRIPTOR_HEAP_DESC heap_desc = {};
  heap_desc.NumDescriptors = desc.descriptor_num;
  heap_desc.Type = HEAP_TYPES.at(desc.type);
  heap_desc.Flags = HEAP_FLAGS.at(desc.flag);
  heap_desc.NodeMask = 0;
  if (HRESULT hr = device.GetDevice()->CreateDescriptorHeap(
          &heap_desc, IID_PPV_ARGS(&heap_));
      FAILED(hr)) {
    MY_LOG(L"CreateDescriptorHeap %s failed.\n Reason: %s", desc.name.c_str(),
           directx_helper::HrToWString(hr));
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

}  // namespace descriptor_heap
}  // namespace directx
}  // namespace legend

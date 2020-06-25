#include "src/directx/buffer/committed_resource.h"

namespace legend {
namespace directx {
namespace buffer {

//コンストラクタ
CommittedResource::CommittedResource() {}

//デストラクタ
CommittedResource::~CommittedResource() {}

//バッファとして初期化する
bool CommittedResource::InitAsBuffer(DirectX12Device& device, u32 buffer_size,
                                     const std::wstring& name) {
  this->buffer_size_ = 0;

  if (FAILED(device.GetDevice()->CreateCommittedResource(
          &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD),
          D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
          &CD3DX12_RESOURCE_DESC::Buffer(buffer_size),
          D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
          IID_PPV_ARGS(&resource_)))) {
    MY_LOG(L"CreateCommittedResource %s failed", name.c_str());
    return false;
  }

  if (FAILED(resource_->SetName(name.c_str()))) {
    return false;
  }

  this->buffer_size_ = buffer_size;
  this->current_state_ =
      D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ;
  return true;
}

//リソースにデータを書き込む
bool CommittedResource::WriteResource(const void* data) {
  void* begin;
  if (FAILED(resource_->Map(0, nullptr, &begin))) {
    MY_LOG(L"Resource Map failed");
    return false;
  }
  memcpy_s(begin, buffer_size_, data, buffer_size_);
  resource_->Unmap(0, nullptr);

  return true;
}

}  // namespace buffer
}  // namespace directx
}  // namespace legend

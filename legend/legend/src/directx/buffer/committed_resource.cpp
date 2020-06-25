#include "src/directx/buffer/committed_resource.h"

#include "src/directx/directx_helper.h"

namespace legend {
namespace directx {
namespace buffer {

//コンストラクタ
CommittedResource::CommittedResource() {}

//デストラクタ
CommittedResource::~CommittedResource() {}

//バッファとして初期化する
bool CommittedResource::InitAsBuffer(DirectX12Device& device, u64 buffer_size,
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

bool CommittedResource::InitAsTex2D(DirectX12Device& device, DXGI_FORMAT format,
                                    u32 width, u32 height,
                                    const std::wstring& name) {
  if (FAILED(device.GetDevice()->CreateCommittedResource(
          &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT),
          D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
          &CD3DX12_RESOURCE_DESC::Tex2D(format, width, height),
          D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
          IID_PPV_ARGS(&resource_)))) {
    return false;
  }
  if (FAILED(resource_->SetName(name.c_str()))) {
    return false;
  }

  this->buffer_size_ = width * height * util::CalcPixelSizeFromFormat(format);
  this->current_state_ =
      D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ;
  return true;
}

void CommittedResource::Transition(DirectX12Device& device,
                                   D3D12_RESOURCE_STATES next_state) {
  if (current_state_ == next_state) return;

  device.GetCommandList()->ResourceBarrier(
      1, &CD3DX12_RESOURCE_BARRIER::Transition(resource_.Get(), current_state_,
                                               next_state));
  this->current_state_ = next_state;
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

void CommittedResource::UpdateSubresource(DirectX12Device& device,
                                          CommittedResource* dest_resource,
                                          CommittedResource* immediate_resource,
                                          const void* data, u64 row,
                                          u64 slice) {
  D3D12_SUBRESOURCE_DATA subresource_data = {};
  subresource_data.pData = data;
  subresource_data.RowPitch = row;
  subresource_data.SlicePitch = slice;
  UpdateSubresources(device.GetCommandList(), dest_resource->GetResource(),
                     immediate_resource->GetResource(), 0, 0, 1,
                     &subresource_data);
}

}  // namespace buffer
}  // namespace directx
}  // namespace legend

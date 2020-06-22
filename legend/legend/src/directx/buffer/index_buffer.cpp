#include "src/directx/buffer/index_buffer.h"

namespace dx12_lib {
namespace directx {
namespace buffer {
IndexBuffer::IndexBuffer()
    : index_buffer_resource_(nullptr), index_buffer_view_{}, index_num_(0) {}

IndexBuffer::~IndexBuffer() {}

bool IndexBuffer::Init(DirectX12Device& device, u32 index_num) {
  index_buffer_resource_.Reset();
  index_buffer_view_ = {};
  this->index_num_ = index_num;

  constexpr DXGI_FORMAT index_format = DXGI_FORMAT::DXGI_FORMAT_R16_UINT;
  const u32 index_buffer_size = index_num * sizeof(Index);

  if (FAILED(device.GetDevice()->CreateCommittedResource(
          &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD),
          D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
          &CD3DX12_RESOURCE_DESC::Buffer(index_buffer_size),
          D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
          IID_PPV_ARGS(&index_buffer_resource_)))) {
    util::debug::Log(L"CreateCommittedResource IndexBuffer failed");
    return false;
  }

  index_buffer_view_.BufferLocation =
      index_buffer_resource_->GetGPUVirtualAddress();
  index_buffer_view_.Format = index_format;
  index_buffer_view_.SizeInBytes = index_buffer_size;

  return true;
}

bool IndexBuffer::WriteBufferResource(const std::vector<u16>& indices) {
  void* index_data_begin;
  if (FAILED(index_buffer_resource_->Map(0, nullptr, &index_data_begin))) {
    util::debug::Log(L"IndexBuffer Map failed");
    return false;
  }
  memcpy_s(index_data_begin, index_buffer_view_.SizeInBytes, indices.data(),
           index_buffer_view_.SizeInBytes);
  index_buffer_resource_->Unmap(0, nullptr);

  return true;
}

void IndexBuffer::SetGraphicsCommandList(DirectX12Device& device) {
  device.GetCommandList()->IASetIndexBuffer(&index_buffer_view_);
}

void IndexBuffer::Draw(DirectX12Device& device) {
  device.GetCommandList()->DrawIndexedInstanced(index_num_, 1, 0, 0, 0);
}

}  // namespace buffer
}  // namespace directx
}  // namespace dx12_lib
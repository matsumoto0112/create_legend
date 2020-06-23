#include "src/directx/buffer/vertex_buffer.h"

namespace legend {
namespace directx {
namespace buffer {

//コンストラクタ
VertexBuffer::VertexBuffer()
    : vertex_buffer_resource_(nullptr), vertex_buffer_view_{} {}

//デストラクタ
VertexBuffer::~VertexBuffer() {}

//初期化
bool VertexBuffer::Init(DirectX12Device& device, u32 vertex_size,
                        u32 vertex_num) {
  vertex_buffer_resource_.Reset();
  vertex_buffer_view_ = {};

  const u32 buffer_size = vertex_size * vertex_num;
  if (FAILED(device.GetDevice()->CreateCommittedResource(
          &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD),
          D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
          &CD3DX12_RESOURCE_DESC::Buffer(buffer_size),
          D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
          IID_PPV_ARGS(&vertex_buffer_resource_)))) {
    MY_LOG(L"CreateCommittedResource failed.");
    return false;
  }

  vertex_buffer_view_.BufferLocation =
      vertex_buffer_resource_->GetGPUVirtualAddress();
  vertex_buffer_view_.SizeInBytes = buffer_size;
  vertex_buffer_view_.StrideInBytes = vertex_size;
  return true;
}

//コマンドリストにセットする
void VertexBuffer::SetGraphicsCommandList(DirectX12Device& device) {
  device.GetCommandList()->IASetVertexBuffers(0, 1, &vertex_buffer_view_);
}

}  // namespace buffer
}  // namespace directx
}  // namespace legend

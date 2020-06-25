#include "src/directx/buffer/vertex_buffer.h"

namespace legend {
namespace directx {
namespace buffer {

//�R���X�g���N�^
VertexBuffer::VertexBuffer() : resource_(), vertex_buffer_view_{} {}

//�f�X�g���N�^
VertexBuffer::~VertexBuffer() {}

//������
bool VertexBuffer::Init(DirectX12Device& device, u32 vertex_size,
                        u32 vertex_num, const std::wstring& name) {
  vertex_buffer_view_ = {};

  const u32 buffer_size = vertex_size * vertex_num;
  if (!resource_.InitAsBuffer(device, buffer_size, name)) {
    return false;
  }

  vertex_buffer_view_.BufferLocation =
      resource_.GetResource()->GetGPUVirtualAddress();
  vertex_buffer_view_.SizeInBytes = buffer_size;
  vertex_buffer_view_.StrideInBytes = vertex_size;
  return true;
}

//�R�}���h���X�g�ɃZ�b�g����
void VertexBuffer::SetGraphicsCommandList(DirectX12Device& device) {
  device.GetCommandList()->IASetVertexBuffers(0, 1, &vertex_buffer_view_);
}

}  // namespace buffer
}  // namespace directx
}  // namespace legend

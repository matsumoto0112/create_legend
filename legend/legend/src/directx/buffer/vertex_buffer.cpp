#include "src/directx/buffer/vertex_buffer.h"

namespace legend {
namespace directx {
namespace buffer {

//�R���X�g���N�^
VertexBuffer::VertexBuffer() : resource_{}, vertex_buffer_view_{} {}

//�f�X�g���N�^
VertexBuffer::~VertexBuffer() {}

//������
bool VertexBuffer::Init(device::IDirectXAccessor& accessor, u32 vertex_size,
                        u32 vertex_num, const std::wstring& name) {
  vertex_buffer_view_ = {};

  const u32 buffer_size = vertex_size * vertex_num;
  const CommittedResource::BufferDesc desc{
      name, buffer_size,
      D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ};
  if (!resource_.InitAsBuffer(accessor, desc)) {
    return false;
  }

  vertex_buffer_view_.BufferLocation =
      resource_.GetResource()->GetGPUVirtualAddress();
  vertex_buffer_view_.SizeInBytes = buffer_size;
  vertex_buffer_view_.StrideInBytes = vertex_size;

  return true;
}

//���\�[�X�̏�������
bool VertexBuffer::WriteBufferResource(const void* data) {
  return resource_.WriteResource(data);
}

//�R�}���h���X�g�ɃZ�b�g����
void VertexBuffer::SetGraphicsCommandList(device::CommandList& command_list) {
  command_list.GetCommandList()->IASetVertexBuffers(0, 1, &vertex_buffer_view_);
}

}  // namespace buffer
}  // namespace directx
}  // namespace legend

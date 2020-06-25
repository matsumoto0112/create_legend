#include "src/directx/buffer/index_buffer.h"

namespace {
D3D12_PRIMITIVE_TOPOLOGY convert(legend::directx::PrimitiveTopology topology) {
  using legend::directx::PrimitiveTopology;
  static const std::unordered_map<PrimitiveTopology, D3D12_PRIMITIVE_TOPOLOGY>
      dict{{PrimitiveTopology::TriangleList,
            D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST},
           {PrimitiveTopology::PointList,
            D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_POINTLIST}};

  MY_ASSERTION(dict.find(topology) != dict.end(),
               L"未定義のトポロジーが選択されました。");
  return dict.at(topology);
}
}  // namespace

namespace legend {
namespace directx {
namespace buffer {
IndexBuffer::IndexBuffer() : resource_(), index_buffer_view_{}, index_num_(0) {}

IndexBuffer::~IndexBuffer() {}

bool IndexBuffer::Init(DirectX12Device& device, u32 index_num,
                       PrimitiveTopology topology) {
  index_buffer_view_ = {};
  this->index_num_ = index_num;
  this->primitive_toporogy_ = convert(topology);

  constexpr DXGI_FORMAT index_format = DXGI_FORMAT::DXGI_FORMAT_R16_UINT;
  const u32 index_buffer_size = index_num * sizeof(Index);

  if (!resource_.InitAsBuffer(device, index_buffer_size,
                              L"IndexBufferResource")) {
    return false;
  }

  index_buffer_view_.BufferLocation =
      resource_.GetResource()->GetGPUVirtualAddress();
  index_buffer_view_.Format = index_format;
  index_buffer_view_.SizeInBytes = index_buffer_size;

  return true;
}

bool IndexBuffer::WriteBufferResource(const std::vector<u16>& indices) {
  return resource_.WriteResource(indices.data());
}

void IndexBuffer::SetGraphicsCommandList(DirectX12Device& device) {
  device.GetCommandList()->IASetIndexBuffer(&index_buffer_view_);
  device.GetCommandList()->IASetPrimitiveTopology(primitive_toporogy_);
}

void IndexBuffer::Draw(DirectX12Device& device) {
  device.GetCommandList()->DrawIndexedInstanced(index_num_, 1, 0, 0, 0);
}

}  // namespace buffer
}  // namespace directx
}  // namespace legend

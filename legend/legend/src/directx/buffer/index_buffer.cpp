#include "src/directx/buffer/index_buffer.h"

namespace {

/**
 * @brief PrimitiveTopologyからD3D12_PRIMITIVE_TOPOLOGYに変換する
 * @param topology 変換元
 * @return 変換先
 */
D3D12_PRIMITIVE_TOPOLOGY Convert(legend::directx::PrimitiveTopology topology) {
  using legend::directx::PrimitiveTopology;

  static const std::unordered_map<PrimitiveTopology, D3D12_PRIMITIVE_TOPOLOGY>
      dict{
          {PrimitiveTopology::TriangleList,
           D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST},
          {PrimitiveTopology::PointList,
           D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_POINTLIST},
          {PrimitiveTopology::LineList,
           D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_LINELIST},
      };

  MY_ASSERTION(dict.find(topology) != dict.end(),
               L"未定義のトポロジーが選択されました。");
  return dict.at(topology);
}

/**
 * @brief インデックスの大きさからフォーマットを計算する
 * @param index_size
 * @return
 */
DXGI_FORMAT CalcFormatFromIndexSize(legend::u32 index_size) {
  switch (index_size) {
    case sizeof(legend::u16):
      return DXGI_FORMAT::DXGI_FORMAT_R16_UINT;
    case sizeof(legend::u32):
      return DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
    default:
      return DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
      break;
  }
}
}  // namespace

namespace legend {
namespace directx {
namespace buffer {

//コンストラクタ
IndexBuffer::IndexBuffer()
    : resource_(),
      index_buffer_view_{},
      index_num_(0),
      primitive_toporogy_(
          D3D12_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED) {}

//デストラクタ
IndexBuffer::~IndexBuffer() {}

//初期化する
bool IndexBuffer::Init(device::IDirectXAccessor& accessor, u32 index_size,
                       u32 index_num, PrimitiveTopology topology,
                       const std::wstring& name) {
  index_buffer_view_ = {};
  index_num_ = index_num;
  primitive_toporogy_ = Convert(topology);

  const DXGI_FORMAT index_format = CalcFormatFromIndexSize(index_size);
  const u32 index_buffer_size = index_num * index_size;

  const CommittedResource::BufferDesc desc{
      name, index_buffer_size,
      D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ};
  if (!resource_.InitAsBuffer(accessor, desc)) {
    return false;
  }

  index_buffer_view_.BufferLocation =
      resource_.GetResource()->GetGPUVirtualAddress();
  index_buffer_view_.Format = index_format;
  index_buffer_view_.SizeInBytes = index_buffer_size;
  return true;
}

//リソースを書き込む
bool IndexBuffer::WriteBufferResource(const void* data) {
  return resource_.WriteResource(data);
}

//コマンドリストにセットする
void IndexBuffer::SetGraphicsCommandList(device::CommandList& command_list) {
  command_list.GetCommandList()->IASetIndexBuffer(&index_buffer_view_);
  command_list.GetCommandList()->IASetPrimitiveTopology(primitive_toporogy_);
}

//描画
void IndexBuffer::Draw(device::CommandList& command_list) {
  command_list.GetCommandList()->DrawIndexedInstanced(index_num_, 1, 0, 0, 0);
}

}  // namespace buffer
}  // namespace directx
}  // namespace legend

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

//初期化
bool IndexBuffer::Init(IDirectXAccessor& accessor, u32 index_num,
                       PrimitiveTopology topology, const std::wstring& name) {
  this->index_buffer_view_ = {};
  this->index_num_ = index_num;
  this->primitive_toporogy_ = Convert(topology);

  constexpr DXGI_FORMAT index_format = DXGI_FORMAT::DXGI_FORMAT_R16_UINT;
  const u32 index_buffer_size = index_num * sizeof(Index);

  if (!resource_.InitAsBuffer(accessor, index_buffer_size, name)) {
    return false;
  }

  index_buffer_view_.BufferLocation =
      resource_.GetResource()->GetGPUVirtualAddress();
  index_buffer_view_.Format = index_format;
  index_buffer_view_.SizeInBytes = index_buffer_size;

  return true;
}

//初期化と書き込み
bool IndexBuffer::InitAndWrite(IDirectXAccessor& accessor,
                               const std::vector<Index>& indices,
                               PrimitiveTopology topology,
                               const std::wstring& name) {
  if (!Init(accessor, static_cast<u32>(indices.size()), topology, name))
    return false;

  return WriteBufferResource(indices);
}

//バッファにリソース書き込み
bool IndexBuffer::WriteBufferResource(const std::vector<u16>& indices) {
  return resource_.WriteResource(indices.data());
}

//コマンドリストにセットする
void IndexBuffer::SetGraphicsCommandList(IDirectXAccessor& accessor) {
  accessor.GetCommandList()->IASetIndexBuffer(&index_buffer_view_);
  accessor.GetCommandList()->IASetPrimitiveTopology(primitive_toporogy_);
}

//描画指令を出す
void IndexBuffer::Draw(IDirectXAccessor& accessor) {
  accessor.GetCommandList()->DrawIndexedInstanced(index_num_, 1, 0, 0, 0);
}

}  // namespace buffer
}  // namespace directx
}  // namespace legend

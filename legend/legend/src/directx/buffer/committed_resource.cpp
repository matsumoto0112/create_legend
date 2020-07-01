#include "src/directx/buffer/committed_resource.h"

#include "src/directx/directx_helper.h"

namespace {
/**
 * @brief バッファの初期化値が必要かどうか判定する
 * @param flags リソースの用途
 * @return 用途に対して初期化値が必要ならtrueを返す
 */
bool NeedClearValue(D3D12_RESOURCE_FLAGS flags) {
  return (
      flags & D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET ||
      flags & D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
}
}  // namespace

namespace legend {
namespace directx {
namespace buffer {

//コンストラクタ
CommittedResource::CommittedResource()
    : resource_(nullptr),
      buffer_size_(0),
      current_state_(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COMMON) {}

//デストラクタ
CommittedResource::~CommittedResource() { Reset(); }

void CommittedResource::Reset() {
  resource_.Reset();
  buffer_size_ = 0;
  current_state_ = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COMMON;
}

//バッファとして初期化する
bool CommittedResource::InitAsBuffer(IDirectXAccessor& accessor,
                                     u64 buffer_size,
                                     const std::wstring& name) {
  Reset();

  if (FAILED(accessor.GetDevice()->CreateCommittedResource(
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

// 2Dテクスチャとして初期化する
bool CommittedResource::InitAsTex2D(IDirectXAccessor& accessor,
                                    const TextureBufferDesc& desc) {
  Reset();

  CD3DX12_RESOURCE_DESC resource_desc =
      CD3DX12_RESOURCE_DESC::Tex2D(desc.format, desc.width, desc.height);
  resource_desc.Flags |= desc.flags;
  const D3D12_CLEAR_VALUE* clear_value_ptr =
      NeedClearValue(resource_desc.Flags) ? &desc.clear_value : nullptr;

  if (FAILED(accessor.GetDevice()->CreateCommittedResource(
          &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT),
          D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &resource_desc,
          D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ,
          clear_value_ptr, IID_PPV_ARGS(&resource_)))) {
    MY_LOG(L"CreateCommittedResource %s failed.", desc.name.c_str());
    return false;
  }

  if (FAILED(resource_->SetName(desc.name.c_str()))) {
    return false;
  }

  this->buffer_size_ = desc.width * desc.height *
                       directx_helper::CalcPixelSizeFromFormat(desc.format);
  this->current_state_ =
      D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ;
  return true;
}

//バッファをコピーする
bool CommittedResource::InitFromBuffer(IDirectXAccessor& accessor,
                                       ComPtr<ID3D12Resource> buffer) {
  resource_ = buffer;
  this->current_state_ = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COMMON;
  this->buffer_size_ = 0;

  return true;
}

//状態を遷移させる
void CommittedResource::Transition(IDirectXAccessor& accessor,
                                   D3D12_RESOURCE_STATES next_state) {
  //前と同じならスルー
  if (current_state_ == next_state) return;

  accessor.GetCommandList()->ResourceBarrier(
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

//テクスチャなどに利用する2つのバッファを利用した書き込み
void CommittedResource::UpdateSubresource(IDirectXAccessor& accessor,
                                          CommittedResource* dest_resource,
                                          CommittedResource* immediate_resource,
                                          const void* data, u64 row,
                                          u64 slice) {
  D3D12_SUBRESOURCE_DATA subresource_data = {};
  subresource_data.pData = data;
  subresource_data.RowPitch = row;
  subresource_data.SlicePitch = slice;
  UpdateSubresources(accessor.GetCommandList(), dest_resource->GetResource(),
                     immediate_resource->GetResource(), 0, 0, 1,
                     &subresource_data);
}

}  // namespace buffer
}  // namespace directx
}  // namespace legend

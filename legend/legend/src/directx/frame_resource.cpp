#include "src/directx/frame_resource.h"

namespace legend {
namespace directx {

//コンストラクタ
FrameResource::FrameResource() {}

//デストラクタ
FrameResource::~FrameResource() {}

//初期化
bool FrameResource::Init(device::IDirectXAccessor& accessor,
                         D3D12_COMMAND_LIST_TYPE command_list_type) {
  if (!command_lists_.Init(accessor, command_list_type)) {
    return false;
  }

  if (!command_lists_.Close()) {
    return false;
  }
  fence_value_ = 0;
  return true;
}

//リソース準備
bool FrameResource::Ready() {
  if (!command_lists_.Reset()) {
    return false;
  }

  return true;
}

//破棄
void FrameResource::Destroy() {}

}  // namespace directx
}  // namespace legend

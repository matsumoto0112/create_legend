#include "src/directx/frame_resource.h"

namespace legend {
namespace directx {

bool FrameResource::Init(device::IDirectXAccessor& accessor) {
  if (!command_lists_.Init(
          accessor, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT)) {
    return false;
  }

  command_lists_.Close();

  return true;
}

bool FrameResource::Ready() {
  if (!command_lists_.Reset()) {
    return false;
  }

  return true;
}

void FrameResource::Destroy() {}

}  // namespace directx
}  // namespace legend

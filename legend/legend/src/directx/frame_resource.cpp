#include "src/directx/frame_resource.h"

namespace legend {
namespace directx {

bool FrameResource::Init(ID3D12Device* device) { return true; }

bool FrameResource::AddCommandList(ID3D12Device* device) {
  auto& cmd_list = command_lists_.emplace_back();
  if (!cmd_list.Init(device,
                     D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT)) {
    return false;
  }

  cmd_list.Close();
  batch_submit_.emplace_back(cmd_list.GetCommandList());
  return true;
}

bool FrameResource::Ready() {
  for (auto&& cmd_list : command_lists_) {
    if (!cmd_list.Reset()) {
      return false;
    }
  }

  return true;
}

void FrameResource::Destroy() {}

}  // namespace directx
}  // namespace legend

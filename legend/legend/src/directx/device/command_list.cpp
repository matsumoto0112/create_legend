#include "src/directx/device/command_list.h"

#include "src/directx/directx_helper.h"

namespace legend {
namespace directx {
namespace device {

using directx_helper::Failed;

//�R���X�g���N�^
CommandList::CommandList() {}

//�f�X�g���N�^
CommandList::~CommandList() {}

//������
bool CommandList::Init(IDirectXAccessor& accessor,
                       D3D12_COMMAND_LIST_TYPE type) {
  if (Failed(accessor.GetDevice()->CreateCommandAllocator(
          type, IID_PPV_ARGS(&command_allocator_)))) {
    return false;
  }

  if (Failed(accessor.GetDevice()->CreateCommandList(
          0, type, command_allocator_.Get(), nullptr,
          IID_PPV_ARGS(&command_list_)))) {
    return false;
  }

  return true;
}

//�R�}���h���X�g�����
bool CommandList::Close() {
  if (Failed(command_list_->Close())) {
    return false;
  }

  return true;
}
bool CommandList::Reset() {
  if (Failed(command_allocator_->Reset())) {
    return false;
  }

  if (Failed(command_list_->Reset(command_allocator_.Get(), nullptr))) {
    return false;
  }

  return true;
}
}  // namespace device
}  // namespace directx
}  // namespace legend

#include "src/directx/device/CommandList.h"

#include "src/directx/directx_helper.h"

namespace legend {
namespace directx {
namespace device {
CommandList::CommandList() {}
CommandList::~CommandList() {}
bool CommandList::Init(ID3D12Device* device,
                       ID3D12CommandQueue* command_queue) {
  MY_ASSERTION(device, L"�f�o�C�X��nullptr�ł��B");
  MY_ASSERTION(command_queue, L"CommandQueue��nullptr�ł��B");

  const D3D12_COMMAND_LIST_TYPE command_list_type =
      command_queue->GetDesc().Type;

  if (HRESULT hr = device->CreateCommandAllocator(
          command_list_type, IID_PPV_ARGS(&command_allocator_));
      FAILED(hr)) {
    MY_LOG(L"CreateCommandAllocator failed.\nReason: %s",
           directx_helper::HrToWString(hr).c_str());
    return false;
  }

  if (HRESULT hr = device->CreateCommandList(0, command_list_type,
                                             command_allocator_.Get(), nullptr,
                                             IID_PPV_ARGS(&command_list_));
      FAILED(hr)) {
    MY_LOG(L"CreateCommandList failed.\nReason: %s",
           directx_helper::HrToWString(hr).c_str());
    return false;
  }

  return true;
}
}  // namespace device
}  // namespace directx
}  // namespace legend

#include "src/directx/device/directx_device.h"

#include "src/directx/directx_helper.h"

namespace legend {
namespace directx {
namespace device {
using namespace directx::directx_helper;

//�R���X�g���N�^
DirectXDevice::DirectXDevice() {}

//�f�X�g���N�^
DirectXDevice::~DirectXDevice() {}

//������
bool DirectXDevice::Init(u32 width, u32 height, HWND hwnd) {
  //�f�o�C�X�ɐݒ肷��I�v�V�������`����
  const DeviceOptionFlags flags = [](bool use_warp_device) {
    DeviceOptionFlags flags = DeviceOptionFlags::TEARING;
    if (use_warp_device) {
      flags |= DeviceOptionFlags::USE_WARP_DEVICE;
    }
    return flags;
  }(USE_WARP_DEVICE);

  //�f�o�C�X���̏��������܂����
  if (!adapter_.Init(flags)) {
    return false;
  }

  if (Failed(D3D12CreateDevice(adapter_.GetAdapter(),
                               directx::device::defines::MIN_FEATURE_LEVEL,
                               IID_PPV_ARGS(&device_)))) {
    return false;
  }

  heap_manager_ = std::make_unique<descriptor_heap::HeapManager>();
  if (!heap_manager_->Init(*this)) {
    return false;
  }

  //�R�}���h�L���[���쐬����
  D3D12_COMMAND_QUEUE_DESC queue_desc = {};
  queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;
  queue_desc.Type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT;
  if (Failed(device_->CreateCommandQueue(&queue_desc,
                                         IID_PPV_ARGS(&command_queue_)))) {
    return false;
  }
  command_queue_->SetName(L"CommandQueue");

  //�`�惊�\�[�X�Ǘ��I�u�W�F�N�g���쐬����
  if (!render_resource_manager_.Init(*this, adapter_, FRAME_COUNT, width,
                                     height,
                                     DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
                                     hwnd, command_queue_.Get())) {
    return false;
  }
  //�t���[���C���f�b�N�X���X���b�v�`�F�C������擾����
  //����A���`���ɍX�V���Ă���
  frame_index_ = render_resource_manager_.GetCurrentBackBufferIndex();

  for (u32 i = 0; i < FRAME_COUNT; i++) {
    FrameResource& resource = resources_[i];
    if (!resource.Init(*this)) {
      return false;
    }
  }

  if (Failed(device_->CreateFence(fence_value_,
                                  D3D12_FENCE_FLAGS::D3D12_FENCE_FLAG_NONE,
                                  IID_PPV_ARGS(&fence_)))) {
    return false;
  }

  fence_value_++;
  return true;
}

//�`�揀��
bool DirectXDevice::Prepare() {
  heap_manager_->BeginFrame();

  const UINT64 last_completed_fence = fence_->GetCompletedValue();
  current_resource_ = &resources_[frame_index_];

  if (current_resource_->fence_value_ > last_completed_fence) {
    HANDLE event_handle = CreateEvent(nullptr, false, false, nullptr);
    if (!event_handle) {
      return false;
    }
    if (Failed(fence_->SetEventOnCompletion(current_resource_->fence_value_,
                                            event_handle))) {
      return false;
    }
    WaitForSingleObject(event_handle, INFINITE);
    CloseHandle(event_handle);
  }

  current_resource_->Ready();

  render_resource_manager_.SetRenderTargets(
      current_resource_->GetCommandList(),
      directx::render_target::RenderTargetID::BACK_BUFFER, true,
      directx::render_target::DepthStencilTargetID::NONE, false);

  return true;
}

//�`�揈��
bool DirectXDevice::Present() {
  render_resource_manager_.DrawEnd(current_resource_->GetCommandList());

  if (Failed(current_resource_->GetCommandList().Close())) {
    return false;
  }

  ID3D12CommandList* const command_list[] = {
      current_resource_->GetCommandList().GetCommandList()};
  command_queue_->ExecuteCommandLists(_countof(command_list), command_list);
  if (!render_resource_manager_.Present()) {
    return false;
  }
  frame_index_ = render_resource_manager_.GetCurrentBackBufferIndex();

  //�`��R�}���h�𑗐M����
  current_resource_->fence_value_ = fence_value_;
  if (Failed(command_queue_->Signal(fence_.Get(), fence_value_))) {
    return false;
  }
  fence_value_++;

  return true;
}

//�f�o�C�X�̔j��
void DirectXDevice::Destroy() {
  const UINT64 fence = fence_value_;
  const UINT64 last_completed_fence = fence_->GetCompletedValue();

  //�R�}���h�L���[�̎��s
  if (Failed(command_queue_->Signal(fence_.Get(), fence_value_))) {
    return;
  }
  fence_value_++;

  //�R�}���h�̎��s��ҋ@����
  HANDLE fence_event = CreateEvent(nullptr, false, false, nullptr);
  if (last_completed_fence < fence) {
    if (Failed(fence_->SetEventOnCompletion(fence, fence_event))) {
      return;
    }
    WaitForSingleObject(fence_event, INFINITE);
  }
  CloseHandle(fence_event);

  for (UINT i = 0; i < FRAME_COUNT; i++) {
    resources_[i].Destroy();
  }
}

//�����_�[�^�[�Q�b�g�n���h�����擾����
descriptor_heap::DescriptorHandle DirectXDevice::GetRTVHandle() {
  return heap_manager_->GetRtvHeap()->GetHandle();
}

//�f�v�X�E�X�e���V���n���h�����擾����
descriptor_heap::DescriptorHandle DirectXDevice::GetDSVHandle() {
  return heap_manager_->GetDsvHeap()->GetHandle();
}

//�n���h����o�^����
void DirectXDevice::RegisterHandle(u32 register_num, shader::ResourceType type,
                                   descriptor_heap::DescriptorHandle handle) {
  heap_manager_->RegisterHandle(register_num, type, handle);
}

//���[�J���n���h�����擾����
descriptor_heap::DescriptorHandle DirectXDevice::GetLocalHandle(
    descriptor_heap::heap_parameter::LocalHeapID heap_id) {
  return heap_manager_->GetLocalHeap(heap_id);
}

//�R�}���h�̎��s
void DirectXDevice::ExecuteCommandList(
    const std::vector<CommandList>& command_lists) {
  const u32 num = static_cast<u32>(command_lists.size());
  std::vector<ID3D12CommandList*> cmd_lists(num);
  for (u32 i = 0; i < num; i++) {
    cmd_lists[i] = command_lists[i].GetCommandList();
  }

  command_queue_->ExecuteCommandLists(num, cmd_lists.data());
}

//�R�}���h�̎��s��ҋ@����
void DirectXDevice::WaitExecute() {
  HANDLE fence_event = CreateEvent(nullptr, false, false, nullptr);
  if (!fence_event) {
    return;
  }

  const UINT64 fence_to_wait_for = fence_value_;
  if (FAILED(command_queue_->Signal(fence_.Get(), fence_to_wait_for))) {
    return;
  }
  fence_value_++;

  if (FAILED(fence_->SetEventOnCompletion(fence_to_wait_for, fence_event))) {
    return;
  }
  WaitForSingleObject(fence_event, INFINITE);
  CloseHandle(fence_event);
}

}  // namespace device
}  // namespace directx
}  // namespace legend
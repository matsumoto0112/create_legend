#include "src/directx/render_target/render_resource_manager.h"

#include "src/util/stl_extend.h"

namespace legend {
namespace directx {
namespace render_target {

//�R���X�g���N�^
RenderResourceManager::RenderResourceManager() {}

//�f�X�g���N�^
RenderResourceManager::~RenderResourceManager() {}

//������
bool RenderResourceManager::Init(device::IDirectXAccessor& accessor,
                                 device::DXGIAdapter& adapter, u32 frame_count,
                                 u32 width, u32 height, DXGI_FORMAT format,
                                 HWND hwnd, ID3D12CommandQueue* command_queue) {
  this->frame_count_ = frame_count;
  if (!swap_chain_.Init(accessor, adapter, frame_count, width, height, format,
                        hwnd, command_queue)) {
    return false;
  }

  render_targets_.emplace(
      RenderTargetID::BACK_BUFFER,
      MultiFrameMultiRenderTargetTexture(frame_count, swap_chain_));
  auto& rtt = render_targets_.at(RenderTargetID::BACK_BUFFER);

  for (u32 i = 0; i < frame_count; i++) {
    std::wstringstream wss;
    wss << L"BackBuffer " << i;
    if (!rtt.Get(i).InitFromBuffer(accessor, swap_chain_.GetBuffer(i),
                                   util::Color4(0.2f, 0.2f, 0.2f, 1.0f),
                                   wss.str())) {
      return false;
    }
  }

  this->depth_stencil_targets_.clear();
  return true;
}

//�����_�[�^�[�Q�b�g���Z�b�g����
void RenderResourceManager::SetRenderTargets(
    device::CommandList& command_list, RenderTargetID render_target_id,
    bool clear_render_target, DepthStencilTargetID depth_stencil_target_id,
    bool clear_depth_stencil_target) {
  //�����_�[�^�[�Q�b�g���g�p���Ȃ��ݒ�
  if (render_target_id == RenderTargetID::NONE) {
    //�f�v�X�E�X�e���V�����g�p���Ȃ��Ȃ�S��nullptr�ɂ���
    if (depth_stencil_target_id == DepthStencilTargetID::NONE) {
      command_list.GetCommandList()->OMSetRenderTargets(0, nullptr, FALSE,
                                                        nullptr);
      return;
    }

    MY_ASSERTION(util::Exist(depth_stencil_targets_, depth_stencil_target_id),
                 L"���o�^��DepthStencilID���I������܂����B");
    //�f�v�X�E�X�e���V�����g�p�\��Ԃɂ���
    DepthStencil& depth_stencil_target =
        depth_stencil_targets_.at(depth_stencil_target_id).Get();
    depth_stencil_target.Transition(
        command_list, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_DEPTH_WRITE);
    const D3D12_CPU_DESCRIPTOR_HANDLE dsv_handle =
        depth_stencil_target.GetHandle().cpu_handle_;
    command_list.GetCommandList()->OMSetRenderTargets(0, nullptr, FALSE,
                                                      &dsv_handle);
    //�f�v�X�E�X�e���V���̃N���A���K�v�Ȃ珈��������
    if (clear_depth_stencil_target) {
      depth_stencil_target.ClearDepthStencil(command_list);
    }
    return;
  }

  MY_ASSERTION(util::Exist(render_targets_, render_target_id),
               L"���o�^��RenderTargetID���I������܂����B");

  //�����_�[�^�[�Q�b�g���g�p�\��Ԃɂ���
  MultiRenderTargetTexture& render_target =
      render_targets_.at(render_target_id).Get();
  render_target.Transition(
      command_list, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET);

  const std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> rtv_handle =
      render_target.GetRTVHandles();
  const u32 size = static_cast<u32>(rtv_handle.size());

  //�f�v�X�E�X�e���V��������ID�Ȃ�Z�b�g���Ȃ�
  if (depth_stencil_target_id == DepthStencilTargetID::NONE) {
    command_list.GetCommandList()->OMSetRenderTargets(size, rtv_handle.data(),
                                                      FALSE, nullptr);
  } else {
    MY_ASSERTION(util::Exist(depth_stencil_targets_, depth_stencil_target_id),
                 L"���o�^��DepthStencilID���I������܂����B");

    //�f�v�X�E�X�e���V�����g�p�\��Ԃɂ���
    DepthStencil& depth_stencil_target =
        depth_stencil_targets_.at(depth_stencil_target_id).Get();
    depth_stencil_target.Transition(
        command_list, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_DEPTH_WRITE);
    const D3D12_CPU_DESCRIPTOR_HANDLE dsv_handle =
        depth_stencil_target.GetHandle().cpu_handle_;
    command_list.GetCommandList()->OMSetRenderTargets(size, rtv_handle.data(),
                                                      FALSE, &dsv_handle);
    //�f�v�X�E�X�e���V���̃N���A���K�v�Ȃ珈��������
    if (clear_depth_stencil_target) {
      depth_stencil_target.ClearDepthStencil(command_list);
    }
  }

  //�����_�[�^�[�Q�b�g�̃N���A���K�v�Ȃ珈��������
  if (clear_render_target) {
    render_target.ClearRenderTarget(command_list);
  }

  render_target.SetScissorRect(command_list);
  render_target.SetViewport(command_list);
}

//���݂̃o�b�N�o�b�t�@�̃C���f�b�N�X���擾����
u32 RenderResourceManager::GetCurrentBackBufferIndex() const {
  return swap_chain_.GetCurrentBackBufferIndex();
}

//�`��I��
void RenderResourceManager::DrawEnd(device::CommandList& command_list) {
  render_targets_.at(RenderTargetID::BACK_BUFFER)
      .Get()
      .Transition(command_list,
                  D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT);
}

//�`����e�\��
bool RenderResourceManager::Present() { return swap_chain_.Present(); }

//�����_�[�^�[�Q�b�g��ǉ�����
bool RenderResourceManager::AddRenderTarget(
    RenderTargetID id, device::IDirectXAccessor& accessor,
    const std::vector<MultiRenderTargetTexture::Info>& infos) {
  MY_ASSERTION(!util::Exist(render_targets_, id),
               L"�o�^�ς݂�ID���I������܂����B");

  render_targets_.emplace(
      id, MultiFrameMultiRenderTargetTexture(frame_count_, swap_chain_));
  auto& rtt = render_targets_.at(id);

  for (u32 i = 0; i < frame_count_; i++) {
    if (!rtt.Get(i).Init(
            accessor, descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID,
            infos)) {
      return false;
    }
  }

  return true;
}

//�f�v�X�E�X�e���V����ǉ�����
bool RenderResourceManager::AddDepthStencil(
    DepthStencilTargetID id, device::IDirectXAccessor& accessor,
    const DepthStencil::DepthStencilDesc& desc) {
  MY_ASSERTION(!util::Exist(depth_stencil_targets_, id),
               L"�o�^�ς݂�ID���I������܂����B");

  depth_stencil_targets_.emplace(
      id, MultiFrameDepthStencil(frame_count_, swap_chain_));
  auto& ds = depth_stencil_targets_.at(id);

  for (u32 i = 0; i < frame_count_; i++) {
    if (!ds.Get(i).Init(accessor, desc)) {
      return false;
    }
  }
  return true;
}

//�V�F�[�_�[���\�[�X�Ƃ��Ďg�p����
void RenderResourceManager::UseAsSRV(device::IDirectXAccessor& accessor,
                                     RenderTargetID id,
                                     u32 render_target_number) {
  MY_ASSERTION(util::Exist(render_targets_, id),
               L"���o�^��ID���I������܂����B");

  render_targets_.at(id).Get().UseAsSRV(accessor, render_target_number);
}

void RenderResourceManager::UseAsSRV(device::IDirectXAccessor& accessor,
                                     device::CommandList& command_list,
                                     DepthStencilTargetID id,
                                     u32 register_num) {
  MY_ASSERTION(util::Exist(depth_stencil_targets_, id),
               L"���o�^��ID���I������܂����B");

  depth_stencil_targets_.at(id).Get().UseAsSRV(accessor, command_list,
                                               register_num);
}

//�o�^�ς݂�����
bool RenderResourceManager::IsRegisteredRenderTarget(RenderTargetID id) const {
  return util::Exist(render_targets_, id);
}

//�o�^�ς݂�����
bool RenderResourceManager::IsRegisteredDepthStencilTarget(
    DepthStencilTargetID id) const {
  return util::Exist(depth_stencil_targets_, id);
}

MultiRenderTargetTexture RenderResourceManager::GetRenderTarget(
    RenderTargetID id) const {
  MY_ASSERTION(IsRegisteredRenderTarget(id), L"");
  return render_targets_.at(id).Get();
}

DepthStencil RenderResourceManager::GetDepthStencilTarget(
    DepthStencilTargetID id) const {
  MY_ASSERTION(IsRegisteredDepthStencilTarget(id), L"");
  return depth_stencil_targets_.at(id).Get();
}

}  // namespace render_target
}  // namespace directx
}  // namespace legend

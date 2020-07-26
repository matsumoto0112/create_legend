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
bool RenderResourceManager::Init(IDirectXAccessor& accessor,
                                 device::DXGIAdapter& adapter,
                                 window::Window& target_window, u32 frame_count,
                                 DXGI_FORMAT format,
                                 ID3D12CommandQueue* command_queue) {
  if (!swap_chain_.Init(accessor, adapter, target_window, command_queue,
                        format)) {
    return false;
  }

  this->current_render_target_id_ = RenderTargetID::BACK_BUFFER;
  this->current_depth_stencil_target_id_ = UINT_LEAST32_MAX;
  return true;
}

//�����_�[�^�[�Q�b�g�̍쐬
bool RenderResourceManager::CreateRenderTarget(IDirectXAccessor& accessor,
                                               RenderTargetID unique_id,
                                               DXGI_FORMAT format, u32 width,
                                               u32 height,
                                               const util::Color4& clear_color,
                                               const std::wstring& name) {
  MY_ASSERTION(!util::Exist(created_render_targets_, unique_id),
               L"unique_id���d�����Ă��܂��B");

  MultiRenderTargetTexture render_target;
  const MultiRenderTargetTexture::Info info{0,      format,      width,
                                            height, clear_color, L""};
  if (!render_target.Init(
          accessor, descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID,
          info)) {
    return false;
  }

  created_render_targets_[unique_id] = render_target;
  return true;
}

//�}���`�����_�[�^�[�Q�b�g�̍쐬
bool RenderResourceManager::CreateRenderTargets(IDirectXAccessor& accessor,
                                                RenderTargetID unique_id,
                                                const std::vector<Info>& info) {
  MY_ASSERTION(!util::Exist(created_render_targets_, unique_id),
               L"unique_id���d�����Ă��܂��B");

  MultiRenderTargetTexture render_target;
  if (!render_target.Init(
          accessor, descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID,
          info)) {
    return false;
  }

  created_render_targets_[unique_id] = render_target;
  return true;
}

//�f�v�X�E�X�e���V�����쐬����
bool RenderResourceManager::CreateDepthStencil(
    IDirectXAccessor& accessor, u32 unique_id, DXGI_FORMAT format, u32 width,
    u32 height, float depth_clear_value, u8 stencil_clear_value,
    const std::wstring& name) {
  MY_ASSERTION(!util::Exist(created_depth_stencil_targets_, unique_id),
               L"unique_id���d�����Ă��܂��B");

  DepthStencil depth_stencil;
  if (!depth_stencil.Init(accessor, format, width, height,
                          {depth_clear_value, stencil_clear_value}, name)) {
    return false;
  }

  created_depth_stencil_targets_[unique_id] = depth_stencil;
  return true;
}

//�����_�[�^�[�Q�b�g���Z�b�g����
void RenderResourceManager::SetRenderTarget(RenderTargetID unique_id) {
  //���ۂɂ�ID���X�V���邾��
  this->current_render_target_id_ = unique_id;
}

//�f�v�X�E�X�e���V�����Z�b�g����
void RenderResourceManager::SetDepthStencilTarget(u32 unique_id) {
  //���ۂɂ�ID���X�V���邾��
  this->current_depth_stencil_target_id_ = unique_id;
}

//�����_�[�^�[�Q�b�g�̏����p�C�v���C���f�B�X�N�ɏ�������
void RenderResourceManager::WriteRenderTargetInfoToPipelineDesc(
    IDirectXAccessor& accessor, RenderTargetID unique_id,
    shader::GraphicsPipelineState& pipeline) {
  if (unique_id == RenderTargetID::BACK_BUFFER) {
    swap_chain_.GetRenderTarget().WriteInfoToPipelineStateDesc(pipeline);
  } else {
    MY_ASSERTION(util::Exist(created_render_targets_, unique_id),
                 L"���o�^�̃����_�[�^�[�Q�b�g���I������܂����B");
    created_render_targets_[unique_id].WriteInfoToPipelineDesc(&pipeline);
  }
}

//�����_�[�^�[�Q�b�g���V�F�[�_�[���\�[�X�Ƃ��ė��p����
void RenderResourceManager::UseRenderTargetToShaderResource(
    IDirectXAccessor& accessor, RenderTargetID unique_id,
    u32 render_target_number) {
  MY_ASSERTION(
      unique_id != RenderTargetID::BACK_BUFFER,
      L"SwapChain�̃����_�[�^�[�Q�b�g�̓��\�[�X�Ƃ��Ďg�p�ł��܂���B");

  created_render_targets_[unique_id].SetToGlobalHeap(accessor,
                                                     render_target_number);
}

//���݃Z�b�g����Ă��郌���_�[�^�[�Q�b�g���N���A����
void RenderResourceManager::ClearCurrentRenderTarget(
    IDirectXAccessor& accessor) {
  if (current_render_target_id_ == RenderTargetID::BACK_BUFFER) {
    swap_chain_.ClearBackBuffer(accessor);
  } else {
    created_render_targets_[current_render_target_id_].ClearRenderTarget(
        accessor);
  }
}

//���݃Z�b�g����Ă���f�v�X�E�X�e���V�����N���A����
void RenderResourceManager::ClearCurrentDepthStencilTarget(
    IDirectXAccessor& accessor) {
  created_depth_stencil_targets_[current_depth_stencil_target_id_]
      .ClearDepthStencil(accessor);
}

//�����_�[�^�[�Q�b�g���R�}���h���X�g�ɃZ�b�g����
void RenderResourceManager::SetRenderTargetsToCommandList(
    IDirectXAccessor& accessor) {
  if (current_render_target_id_ == RenderTargetID::BACK_BUFFER) {
    SetBackBufferToCommandList(accessor);
    return;
  }
  MY_ASSERTION(util::Exist(created_render_targets_, current_render_target_id_),
               L"���o�^�̃����_�[�^�[�Q�b�gID���I������܂����B");
  MY_ASSERTION(current_depth_stencil_target_id_ == UINT_LEAST32_MAX ||
                   util::Exist(created_depth_stencil_targets_,
                               current_depth_stencil_target_id_),
               L"���o�^�̃f�v�X�E�X�e���V���^�[�Q�b�gID���I������܂����B");

  created_render_targets_[current_render_target_id_].PrepareToUseRenderTarget(
      accessor);

  //�����_�[�^�[�Q�b�g�̃n���h�����擾����
  //�^�[�Q�b�g���X���b�v�`�F�C���Ȃ�o�b�N�o�b�t�@�̃n���h�����擾����
  const std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> rtv_handles =
      created_render_targets_[current_render_target_id_].GetRTVHandles();
  const u32 render_target_num = static_cast<u32>(rtv_handles.size());

  if (current_depth_stencil_target_id_ == UINT_LEAST32_MAX) {
    accessor.GetCommandList()->OMSetRenderTargets(
        render_target_num, rtv_handles.data(), FALSE, nullptr);
    created_render_targets_[current_render_target_id_].SetViewport(accessor);
    created_render_targets_[current_render_target_id_].SetScissorRect(accessor);
  } else {
    D3D12_CPU_DESCRIPTOR_HANDLE dsv_handle =
        created_depth_stencil_targets_[current_depth_stencil_target_id_]
            .GetCPUHandle();
    accessor.GetCommandList()->OMSetRenderTargets(
        render_target_num, rtv_handles.data(), TRUE, &dsv_handle);
    created_render_targets_[current_render_target_id_].SetViewport(accessor);
    created_render_targets_[current_render_target_id_].SetScissorRect(accessor);
  }
}
void RenderResourceManager::DrawEnd(IDirectXAccessor& accessor) {
  if (current_render_target_id_ == RenderTargetID::BACK_BUFFER) {
    swap_chain_.DrawEnd(accessor);
  } else {
    created_render_targets_[current_render_target_id_].DrawEnd(accessor);
  }
}

bool RenderResourceManager::IsRegisteredRenderTargetID(
    RenderTargetID unique_id) const {
  return util::Exist(created_render_targets_, unique_id);
}

bool RenderResourceManager::Present() { return swap_chain_.Present(); }

u32 RenderResourceManager::GetCurrentFrameIndex() {
  swap_chain_.UpdateCurrentFrameIndex();
  return swap_chain_.GetCurrentFrameIndex();
}

void RenderResourceManager::SetBackBufferToCommandList(
    IDirectXAccessor& accessor) {
  swap_chain_.DrawBegin(accessor);

  D3D12_CPU_DESCRIPTOR_HANDLE rtv_handle[1] = {
      swap_chain_.GetRenderTarget().GetHandle().cpu_handle_};
  //�f�v�X�E�X�e���V�����g�p���Ȃ��Ƃ���null���Z�b�g����
  if (current_depth_stencil_target_id_ == UINT_LEAST32_MAX) {
    accessor.GetCommandList()->OMSetRenderTargets(1, rtv_handle, FALSE,
                                                  nullptr);
  } else {
    MY_ASSERTION(util::Exist(created_depth_stencil_targets_,
                             current_depth_stencil_target_id_),
                 L"���o�^�̃f�v�X�E�X�e���V�����I������܂����B");
    D3D12_CPU_DESCRIPTOR_HANDLE dsv_handle =
        created_depth_stencil_targets_[current_depth_stencil_target_id_]
            .GetCPUHandle();
    accessor.GetCommandList()->OMSetRenderTargets(1, rtv_handle, TRUE,
                                                  &dsv_handle);
  }
}

}  // namespace render_target
}  // namespace directx
}  // namespace legend

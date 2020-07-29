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
  if (!swap_chain_.Init(accessor, adapter, target_window, format, frame_count,
                        command_queue)) {
    return false;
  }

  this->current_render_target_id_ = RenderTargetID::BACK_BUFFER;
  this->current_depth_stencil_target_id_ = DepthStencilTargetID::None;
  return true;
}

//�����_�[�^�[�Q�b�g�̍쐬
bool RenderResourceManager::CreateRenderTarget(IDirectXAccessor& accessor,
                                               RenderTargetID unique_id,
                                               DXGI_FORMAT format, u32 width,
                                               u32 height,
                                               const util::Color4& clear_color,
                                               const std::wstring& name) {
  MY_ASSERTION(!util::Exist(render_targets_, unique_id),
               L"unique_id���d�����Ă��܂��B");

  MultiRenderTargetTexture render_target;
  const MultiRenderTargetTexture::Info info{0,      format,      width,
                                            height, clear_color, L""};
  if (!render_target.Init(
          accessor, descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID,
          info)) {
    return false;
  }

  render_targets_.emplace(unique_id, render_target);
  return true;
}

//�}���`�����_�[�^�[�Q�b�g�̍쐬
bool RenderResourceManager::CreateRenderTargets(IDirectXAccessor& accessor,
                                                RenderTargetID unique_id,
                                                const std::vector<Info>& info) {
  MY_ASSERTION(!util::Exist(render_targets_, unique_id),
               L"unique_id���d�����Ă��܂��B");

  MultiRenderTargetTexture render_target;
  if (!render_target.Init(
          accessor, descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID,
          info)) {
    return false;
  }

  render_targets_.emplace(unique_id, render_target);
  return true;
}

//�f�v�X�E�X�e���V�����쐬����
bool RenderResourceManager::CreateDepthStencil(
    IDirectXAccessor& accessor, DepthStencilTargetID unique_id,
    DXGI_FORMAT format, u32 width, u32 height, float depth_clear_value,
    u8 stencil_clear_value, const std::wstring& name) {
  MY_ASSERTION(!util::Exist(depth_stencil_targets_, unique_id),
               L"unique_id���d�����Ă��܂��B");

  DepthStencil depth_stencil;
  if (!depth_stencil.Init(accessor, format, width, height,
                          {depth_clear_value, stencil_clear_value}, name)) {
    return false;
  }

  depth_stencil_targets_.emplace(unique_id, depth_stencil);
  return true;
}

//�����_�[�^�[�Q�b�g���Z�b�g����
void RenderResourceManager::SetRenderTargetID(RenderTargetID unique_id) {
  this->current_render_target_id_ = unique_id;
}

//�f�v�X�E�X�e���V�����Z�b�g����
void RenderResourceManager::SetDepthStencilTargetID(
    DepthStencilTargetID unique_id) {
  this->current_depth_stencil_target_id_ = unique_id;
}

//�����_�[�^�[�Q�b�g�̏����p�C�v���C���ɏ�������
void RenderResourceManager::WriteRenderTargetInfoToPipeline(
    IDirectXAccessor& accessor, RenderTargetID unique_id,
    shader::GraphicsPipelineState* pipeline) {
  if (unique_id == RenderTargetID::BACK_BUFFER) {
    swap_chain_.GetRenderTarget().WriteInfoToPipelineState(pipeline);
  } else {
    MY_ASSERTION(util::Exist(render_targets_, unique_id),
                 L"���o�^�̃����_�[�^�[�Q�b�g���I������܂����B");

    render_targets_.at(unique_id).WriteInfoToPipelineState(pipeline);
  }
}

//�f�v�X�E�X�e���V���̏����p�C�v���C���ɏ�������
void RenderResourceManager::WriteDepthStencilTargetInfoToPipeline(
    IDirectXAccessor& accessor, DepthStencilTargetID unique_id,
    shader::GraphicsPipelineState* pipeline) {
  if (unique_id == DepthStencilTargetID::None) {
    pipeline->SetDSVFormat(DXGI_FORMAT::DXGI_FORMAT_UNKNOWN);
    CD3DX12_DEPTH_STENCIL_DESC sd(D3D12_DEFAULT);
    sd.DepthEnable = false;
    sd.StencilEnable = false;
    pipeline->SetDepthStencilState(sd);
  } else {
    MY_ASSERTION(util::Exist(depth_stencil_targets_, unique_id),
                 L"���o�^�̃f�v�X�E�X�e���V�����I������܂����B");

    depth_stencil_targets_.at(unique_id).WriteInfoToPipelineState(pipeline);
  }
}

//�����_�[�^�[�Q�b�g���V�F�[�_�[���\�[�X�Ƃ��ė��p����
void RenderResourceManager::UseRenderTargetToShaderResource(
    IDirectXAccessor& accessor, RenderTargetID unique_id,
    u32 render_target_number) {
  MY_ASSERTION(
      unique_id != RenderTargetID::BACK_BUFFER,
      L"SwapChain�̃����_�[�^�[�Q�b�g�̓��\�[�X�Ƃ��Ďg�p�ł��܂���B");
  MY_ASSERTION(util::Exist(render_targets_, unique_id),
               L"���o�^�̃����_�[�^�[�Q�b�g���I������܂����B");

  render_targets_.at(unique_id).SetToGlobalHeap(accessor, render_target_number);
}

//���݃Z�b�g����Ă��郌���_�[�^�[�Q�b�g���N���A����
void RenderResourceManager::ClearCurrentRenderTarget(
    IDirectXAccessor& accessor) {
  if (current_render_target_id_ == RenderTargetID::BACK_BUFFER) {
    swap_chain_.ClearBackBuffer(accessor);
  } else {
    MY_ASSERTION(util::Exist(render_targets_, current_render_target_id_),
                 L"���o�^�̃����_�[�^�[�Q�b�g���I������܂����B");

    render_targets_.at(current_render_target_id_).ClearRenderTarget(accessor);
  }
}

//���݃Z�b�g����Ă���f�v�X�E�X�e���V�����N���A����
void RenderResourceManager::ClearCurrentDepthStencilTarget(
    IDirectXAccessor& accessor) {
  if (current_depth_stencil_target_id_ == DepthStencilTargetID::None) return;

  MY_ASSERTION(
      util::Exist(depth_stencil_targets_, current_depth_stencil_target_id_),
      L"���o�^�̃f�v�X�E�X�e���V�����I������܂����B");

  depth_stencil_targets_.at(current_depth_stencil_target_id_)
      .ClearDepthStencil(accessor);
}

//�����_�[�^�[�Q�b�g���R�}���h���X�g�ɃZ�b�g����
void RenderResourceManager::SetRenderTargetsToCommandList(
    IDirectXAccessor& accessor) {
  MY_ASSERTION(current_render_target_id_ == RenderTargetID::BACK_BUFFER ||
                   util::Exist(render_targets_, current_render_target_id_),
               L"���o�^�̃����_�[�^�[�Q�b�gID���I������܂����B");

  const bool use_depth_stencil =
      current_depth_stencil_target_id_ != DepthStencilTargetID::None;

  if (current_render_target_id_ == RenderTargetID::BACK_BUFFER) {
    swap_chain_.DrawBegin(accessor);
  } else {
    render_targets_.at(current_render_target_id_).SetViewport(accessor);
    render_targets_.at(current_render_target_id_).SetScissorRect(accessor);
    render_targets_.at(current_render_target_id_)
        .PrepareToUseRenderTarget(accessor);
  }

  //�����_�[�^�[�Q�b�g�̃n���h�����擾����
  //�^�[�Q�b�g���X���b�v�`�F�C���Ȃ�o�b�N�o�b�t�@�̃n���h�����擾����
  const std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> rtv_handles = [&]() {
    if (current_render_target_id_ == RenderTargetID::BACK_BUFFER) {
      return std::vector{swap_chain_.GetRenderTarget().GetHandle().cpu_handle_};
    }
    return render_targets_.at(current_render_target_id_).GetRTVHandles();
  }();

  const u32 render_target_num = static_cast<u32>(rtv_handles.size());

  //�f�v�X�E�X�e���V�����g�p����Ƃ�
  if (use_depth_stencil) {
    MY_ASSERTION(
        util::Exist(depth_stencil_targets_, current_depth_stencil_target_id_),
        L"���o�^�̃f�v�X�E�X�e���V���^�[�Q�b�gID���I������܂����B");

    depth_stencil_targets_.at(current_depth_stencil_target_id_)
        .PrepareToSetCommandList(accessor);
    D3D12_CPU_DESCRIPTOR_HANDLE dsv_handle =
        depth_stencil_targets_.at(current_depth_stencil_target_id_)
            .GetCPUHandle();
    accessor.GetCommandList()->OMSetRenderTargets(
        render_target_num, rtv_handles.data(), TRUE, &dsv_handle);
  } else {
    accessor.GetCommandList()->OMSetRenderTargets(
        render_target_num, rtv_handles.data(), FALSE, nullptr);
  }
}

//�`��I��
void RenderResourceManager::DrawEnd(IDirectXAccessor& accessor) {
  if (current_render_target_id_ == RenderTargetID::BACK_BUFFER) {
    swap_chain_.DrawEnd(accessor);
  } else {
    render_targets_.at(current_render_target_id_).DrawEnd(accessor);
  }
}

//�����_�[�^�[�Q�b�gID���o�^����Ă��邩
bool RenderResourceManager::IsRegisteredRenderTargetID(
    RenderTargetID unique_id) const {
  return util::Exist(render_targets_, unique_id);
}

//�f�v�X�E�X�e���V��ID���o�^����Ă��邩
bool RenderResourceManager::IsRegisterdDepthStencilTargetID(
    DepthStencilTargetID unique_id) const {
  return util::Exist(depth_stencil_targets_, unique_id);
}

//�o�b�N�o�b�t�@��\������
bool RenderResourceManager::Present() { return swap_chain_.Present(); }

//�t���[���̃C���f�b�N�X���X�V����
void RenderResourceManager::UpdateCurrentFrameIndex() {
  swap_chain_.UpdateCurrentFrameIndex();
}

//���݂̃t���[���C���f�b�N�X���擾����
u32 RenderResourceManager::GetCurrentFrameIndex() const {
  return swap_chain_.GetCurrentFrameIndex();
}

}  // namespace render_target
}  // namespace directx
}  // namespace legend

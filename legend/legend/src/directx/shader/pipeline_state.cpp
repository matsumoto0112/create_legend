#include "src/directx/shader/pipeline_state.h"

#include "src/directx/directx_helper.h"

namespace legend {
namespace directx {
namespace shader {

using directx_helper::Failed;

//�R���X�g���N�^
PipelineState::PipelineState() : pipeline_state_(nullptr) {}

//�f�X�g���N�^
PipelineState::~PipelineState() {}

//������
bool PipelineState::Init(device::IDirectXAccessor& accessor,
                         const GraphicsPipelineStateDesc& desc) {
  if (Failed(accessor.GetDevice()->CreateGraphicsPipelineState(
          &desc, IID_PPV_ARGS(&pipeline_state_)))) {
    return false;
  }

  return true;
}

//������
bool PipelineState::Init(device::IDirectXAccessor& accessor,
                         const ComputePipelineStateDesc& desc) {
  if (Failed(accessor.GetDevice()->CreateComputePipelineState(
          &desc, IID_PPV_ARGS(&pipeline_state_)))) {
    return false;
  }

  return true;
}

//�R�}���h���X�g�ɃZ�b�g����
void PipelineState::SetCommandList(device::CommandList& command_list) {
  command_list.GetCommandList()->SetPipelineState(pipeline_state_.Get());
}

}  // namespace shader
}  // namespace directx
}  // namespace legend

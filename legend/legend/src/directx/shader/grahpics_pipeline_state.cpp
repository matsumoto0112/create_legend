#include "src/directx/directx_helper.h"
#include "src/directx/shader/graphics_pipeline_state.h"

namespace legend {
namespace directx {
namespace shader {

using directx_helper::Failed;

//�R���X�g���N�^
GraphicsPipelineState::GraphicsPipelineState() : pipeline_state_(nullptr) {}

//�f�X�g���N�^
GraphicsPipelineState::~GraphicsPipelineState() {}

//������
bool GraphicsPipelineState::Init(device::IDirectXAccessor& accessor,
                                 const PSODesc& desc) {
  if (Failed(accessor.GetDevice()->CreateGraphicsPipelineState(
          &desc, IID_PPV_ARGS(&pipeline_state_)))) {
    return false;
  }

  return true;
}

//�R�}���h���X�g�ɃZ�b�g����
void GraphicsPipelineState::SetGraphicsCommandList(
    device::CommandList& command_list) {
  command_list.GetCommandList()->SetPipelineState(pipeline_state_.Get());
}

}  // namespace shader
}  // namespace directx
}  // namespace legend

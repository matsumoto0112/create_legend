#include "src/directx/shader/pipeline_state.h"

#include "src/directx/directx_helper.h"

namespace legend {
namespace directx {
namespace shader {

using directx_helper::Failed;

//コンストラクタ
PipelineState::PipelineState() : pipeline_state_(nullptr) {}

//デストラクタ
PipelineState::~PipelineState() {}

//初期化
bool PipelineState::Init(device::IDirectXAccessor& accessor,
                         const GraphicsPipelineStateDesc& desc) {
  if (Failed(accessor.GetDevice()->CreateGraphicsPipelineState(
          &desc, IID_PPV_ARGS(&pipeline_state_)))) {
    return false;
  }

  return true;
}

//初期化
bool PipelineState::Init(device::IDirectXAccessor& accessor,
                         const ComputePipelineStateDesc& desc) {
  if (Failed(accessor.GetDevice()->CreateComputePipelineState(
          &desc, IID_PPV_ARGS(&pipeline_state_)))) {
    return false;
  }

  return true;
}

//コマンドリストにセットする
void PipelineState::SetCommandList(device::CommandList& command_list) {
  command_list.GetCommandList()->SetPipelineState(pipeline_state_.Get());
}

}  // namespace shader
}  // namespace directx
}  // namespace legend

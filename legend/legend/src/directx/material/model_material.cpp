#include "src/directx/material/model_material.h"

#include "src/directx/shader/alpha_blend_desc.h"
#include "src/game/game_device.h"

namespace legend {
namespace directx {
namespace material {

// コンストラクタ
ModelMaterial::ModelMaterial() {}

//デストラクタ
ModelMaterial::~ModelMaterial() {}

//初期化
bool ModelMaterial::Init() {
  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();
  if (!pipeline_state_.Init(device)) {
    return false;
  }

  util::resource::Resource& resource =
      game::GameDevice::GetInstance()->GetResource();
  pipeline_state_.SetVertexShader(resource.GetVertexShader().Get(
      util::resource::VertexShaderID::MODEL_VIEW));
  pipeline_state_.SetPixelShader(
      resource.GetPixelShader().Get(util::resource::PixelShaderID::MODEL_VIEW));
  pipeline_state_.SetBlendDesc(
      directx::shader::alpha_blend_desc::BLEND_DESC_ALIGNMENT, 0);
  device.GetRenderResourceManager().WriteDepthStencilTargetInfoToPipeline(
      device, render_target::DepthStencilTargetID::Depth, &pipeline_state_);
  device.GetRenderResourceManager().WriteRenderTargetInfoToPipeline(
      device, render_target::RenderTargetID::BACK_BUFFER, &pipeline_state_);
  pipeline_state_.SetPrimitiveTopology(
      D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
  pipeline_state_.SetRootSignature(device.GetDefaultRootSignature());

  return pipeline_state_.CreatePipelineState(device);
}

//コマンドリストにセットする
void ModelMaterial::SetToGraphicsCommandList() {
  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();
  pipeline_state_.SetGraphicsCommandList(device);
}

}  // namespace material
}  // namespace directx
}  // namespace legend
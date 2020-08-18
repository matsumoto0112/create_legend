#include "src/util/pipeline_initializer.h"

#include "src/directx/shader/alpha_blend_desc.h"
#include "src/directx/shader/graphics_pipeline_state_desc.h"
#include "src/directx/shader/pipeline_state.h"
#include "src/game/game_device.h"

namespace {
enum PipelineFormat {
  NAME,
  VS,
  PS,
  RT_ID,
  DS_ID,
  ALPHA_BLEND,
  PRIM_TYPE,
};
enum BlendStateType {
  NO_BLEND,
  ALIGNMENT,
  ADD,
};

}  // namespace

namespace legend {
namespace util {

bool PipelineInitializer::Init(const std::filesystem::path& filepath) {
  //ƒpƒCƒvƒ‰ƒCƒ“‚Ì“o˜^
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& render_resource_manager = device.GetRenderResourceManager();
  auto& resource = game::GameDevice::GetInstance()->GetResource();
  std::vector<u8> data = resource.GetArchiveLoader().Load(filepath);
  std::string str(data.begin(), data.end());
  std::stringstream ss(str);
  std::string line;
  while (std::getline(ss, line)) {
    if (line.back() == '\r') line = line.substr(0, line.size() - 1);
    directx::shader::GraphicsPipelineStateDesc pso_desc = {};
    auto splited = util::string_util::StringSplit(line, ',');

    const std::wstring vertex_shader_name =
        util::string_util::String_2_WString(splited[VS]) + L".cso";
    pso_desc.SetVertexShader(
        resource.GetVertexShader().Get(vertex_shader_name).get());

    const std::wstring pixel_shader_name =
        util::string_util::String_2_WString(splited[PS]) + L".cso";
    pso_desc.SetPixelShader(
        resource.GetPixelShader().Get(pixel_shader_name).get());

    const int render_target_id = std::stoi(splited[RT_ID]);
    pso_desc.SetRenderTargets(render_resource_manager.GetRenderTarget(
        static_cast<directx::render_target::RenderTargetID>(render_target_id)));

    const int depth_stencil_target_id = std::stoi(splited[DS_ID]);
    if (depth_stencil_target_id == 0) {
      pso_desc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC1(D3D12_DEFAULT);
      pso_desc.DepthStencilState.DepthEnable = false;
    } else {
      pso_desc.SetDepthStencilTarget(
          render_resource_manager.GetDepthStencilTarget(
              static_cast<directx::render_target::DepthStencilTargetID>(
                  depth_stencil_target_id)));
    }

    const int primitive_type = std::stoi(splited[PRIM_TYPE]);
    pso_desc.PrimitiveTopologyType =
        static_cast<D3D12_PRIMITIVE_TOPOLOGY_TYPE>(primitive_type);

    enum BlendStateType {
      NO_BLEND,
      ALIGNMENT,
      ADD,
    };
    const BlendStateType blend_state_type =
        static_cast<BlendStateType>(std::stoi(splited[ALPHA_BLEND]));
    switch (blend_state_type) {
      case BlendStateType::NO_BLEND:
        pso_desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
        break;
      default:
      case BlendStateType::ALIGNMENT:
        pso_desc.BlendState.RenderTarget[0] =
            directx::shader::alpha_blend_desc::BLEND_DESC_ALIGNMENT;
        break;
      case BlendStateType::ADD:
        pso_desc.BlendState.RenderTarget[0] =
            directx::shader::alpha_blend_desc::BLEND_DESC_ADD;
        break;
    }

    pso_desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    pso_desc.SampleDesc.Count = 1;
    pso_desc.SampleMask = UINT_MAX;
    pso_desc.SetRootSignature(device.GetDefaultRootSignature());

    auto pipeline = std::make_shared<directx::shader::PipelineState>();
    if (!pipeline->Init(device, pso_desc)) {
      return false;
    }

    const std::wstring pipeline_name =
        util::string_util::String_2_WString(splited[NAME]);
    resource.GetPipeline().Register(pipeline_name, pipeline);
  }
  return true;
}

}  // namespace util
}  // namespace legend
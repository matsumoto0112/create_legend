#include "src/scenes/debugscene/model_view.h"

#include "src/directx/directx_helper.h"
#include "src/directx/shader/alpha_blend_desc.h"
#include "src/directx/shader/pixel_shader.h"
#include "src/directx/shader/shader_register_id.h"
#include "src/directx/shader/vertex_shader.h"
#include "src/directx/vertex.h"
#include "src/game/game_device.h"
#include "src/util/path.h"

namespace {
constexpr legend::u32 OBJ_NUM = 1;
}  // namespace

namespace legend {
namespace scenes {
namespace debugscene {

//コンストラクタ
ModelView::ModelView(ISceneChange* scene_change) : Scene(scene_change) {}

ModelView::~ModelView() {}

//初期化
bool ModelView::Initialize() {
  if (!Scene::Initialize()) {
    return false;
  }

  auto& device = game::GameDevice::GetInstance()->GetDevice();

  if (!device.GetHeapManager().AddLocalHeap(
          device, directx::descriptor_heap::heap_parameter::LocalHeapID::
                      MODEL_VIEW_SCENE)) {
    return false;
  }

  directx::device::CommandList command_list;
  if (!command_list.Init(
          device, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT)) {
    return false;
  }

  const std::filesystem::path model_path =
      util::Path::GetInstance()->model() / "desk.glb";
  if (!model_.Init(model_path, command_list)) {
    return false;
  }

  {
    const std::filesystem::path shader = util::Path::GetInstance()->shader();
    directx::shader::VertexShader vs;
    if (!vs.Init(device, shader / "modelview" / "model_view_vs.cso")) {
      return false;
    }
    directx::shader::PixelShader ps;
    if (!ps.Init(device, shader / "modelview" / "model_view_ps.cso")) {
      return false;
    }

    D3D12_GRAPHICS_PIPELINE_STATE_DESC pso_desc = {};
    pso_desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    pso_desc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC1(D3D12_DEFAULT);
    pso_desc.DepthStencilState.DepthEnable = false;
    pso_desc.DSVFormat = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
    pso_desc.InputLayout = vs.GetInputLayout();
    pso_desc.NumRenderTargets = 1;
    pso_desc.PrimitiveTopologyType =
        D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    pso_desc.pRootSignature =
        device.GetDefaultRootSignature()->GetRootSignature();
    pso_desc.PS = ps.GetShaderBytecode();
    pso_desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    pso_desc.RTVFormats[0] = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
    pso_desc.SampleDesc.Count = 1;
    pso_desc.SampleMask = UINT_MAX;
    pso_desc.VS = vs.GetShaderBytecode();
    if (!pipeline_.Init(device, pso_desc)) {
      return false;
    }
  }

  {
    directx::render_target::DepthStencil::DepthStencilDesc desc = {
        L"DepthOnly", DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT, 1280, 720, 1.0f, 0};
    if (!device.GetRenderResourceManager().AddDepthStencil(
            directx::render_target::DepthStencilTargetID::DEPTH_ONLY, device,
            desc)) {
      return false;
    }
  }

  transform_cb_.resize(OBJ_NUM);
  transforms_.resize(OBJ_NUM);
  for (u32 i = 0; i < OBJ_NUM; i++) {
    if (!transform_cb_[i].Init(
            device, directx::shader::ConstantBufferRegisterID::TRANSFORM,
            device.GetLocalHandle(directx::descriptor_heap::heap_parameter::
                                      LocalHeapID::GLOBAL_ID),
            L"Transform")) {
      return false;
    }
    transforms_[i].SetPosition(math::Vector3(i * 0.6f, 0.0f, i * 0.6f));
    transform_cb_[i].GetStagingRef().world = transforms_[i].CreateWorldMatrix();
    transform_cb_[i].UpdateStaging();
  }
  {
    const math::Vector3 pos = math::Vector3(0.0f, 0.3f, -0.5f);
    const math::Quaternion rot =
        math::Quaternion::FromEular(20.0f * math::util::DEG_2_RAD, 0.0f, 0.0f);
    const float fov = 50.0f * math::util::DEG_2_RAD;
    const float aspect = 1280.0f / 720.0f;
    if (!camera_.Init(L"MainCamera", pos, rot, fov, aspect)) {
      return false;
    }
  }

  if (!command_list.Close()) {
    return false;
  }

  device.ExecuteCommandList({command_list});
  device.WaitExecute();

  return true;
}

//更新
bool ModelView::Update() {
  if (!Scene::Update()) {
    return false;
  }

  transforms_[0].SetRotation(transforms_[0].GetRotation() *
                             math::Quaternion::FromEular(0.0f, 0.001f, 0.0f));
  transform_cb_[0].GetStagingRef().world = transforms_[0].CreateWorldMatrix();
  transform_cb_[0].UpdateStaging();
  return true;
}

//描画
void ModelView::Draw() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();

  auto& render_resource_manager = device.GetRenderResourceManager();
  directx::device::CommandList& command_list =
      device.GetCurrentFrameResource()->GetCommandList();
  render_resource_manager.SetRenderTargets(
      command_list, directx::render_target::RenderTargetID::BACK_BUFFER, true,
      directx::render_target::DepthStencilTargetID::DEPTH_ONLY, true);

  pipeline_.SetGraphicsCommandList(command_list);
  device.GetHeapManager().SetGraphicsCommandList(command_list);
  camera_.RenderStart();

  for (i32 i = OBJ_NUM - 1; i >= 0; i--) {
    transform_cb_[i].SetToHeap(device);
    model_.Draw(command_list);
  }
}

void ModelView::Finalize() {}

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend

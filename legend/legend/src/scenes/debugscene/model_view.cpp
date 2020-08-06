#include "src/scenes/debugscene/model_view.h"

#include "src/directx/directx_helper.h"
#include "src/directx/shader/pixel_shader.h"
#include "src/directx/shader/shader_register_id.h"
#include "src/directx/shader/vertex_shader.h"
#include "src/directx/vertex.h"
#include "src/game/game_device.h"
#include "src/util/path.h"

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
  directx::device::CommandList command_list;
  if (!command_list.Init(
          device.GetDevice(),
          D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT)) {
    return false;
  }

  const std::filesystem::path model_path =
      util::Path::GetInstance()->model() / "1000cmObject.glb";
  if (!model_.Init(model_path, command_list)) {
    return false;
  }

  if (!root_signature_.InitByDefault(device, L"DefaultRootSignature")) {
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
    pso_desc.DSVFormat = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
    pso_desc.Flags = D3D12_PIPELINE_STATE_FLAGS::D3D12_PIPELINE_STATE_FLAG_NONE;
    pso_desc.InputLayout = vs.GetInputLayout();
    pso_desc.NodeMask = 0;
    pso_desc.NumRenderTargets = 1;
    pso_desc.PrimitiveTopologyType =
        D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    pso_desc.pRootSignature = root_signature_.GetRootSignature();
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

  if (!transform_cb_.Init(
          device, directx::shader::ConstantBufferRegisterID::TRANSFORM,
          device.GetLocalHandle(
              directx::descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID),
          L"Transform")) {
    return false;
  }
  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();

  {
    const math::Vector3 pos = math::Vector3(0.0f, 10.0f, -10.0f);
    const math::Quaternion rot =
        math::Quaternion::FromEular(45.0f * math::util::DEG_2_RAD, 0.0f, 0.0f);
    const float fov = 50.0f * math::util::DEG_2_RAD;
    const float aspect = 1280.0f / 720.0f;
    if (!camera_.Init(L"MainCamera", pos, rot, fov, aspect)) {
      return false;
    }
  }

  if (!command_list.Close()) {
    return false;
  }

  ID3D12CommandList* pp_command_list[] = {command_list.GetCommandList()};
  device.command_queue_->ExecuteCommandLists(_countof(pp_command_list),
                                             pp_command_list);

  HANDLE fence_event = CreateEvent(nullptr, false, false, nullptr);
  if (!fence_event) {
    return false;
  }

  const UINT64 fence_to_wait_for = device.fence_value_;
  if (FAILED(device.command_queue_->Signal(device.fence_.Get(),
                                           fence_to_wait_for))) {
    return false;
  }
  device.fence_value_++;

  if (FAILED(device.fence_->SetEventOnCompletion(fence_to_wait_for,
                                                 fence_event))) {
    return false;
  }
  WaitForSingleObject(fence_event, INFINITE);

  return true;
}

//更新
bool ModelView::Update() {
  if (!Scene::Update()) {
    return false;
  }

  return true;
}

//描画
void ModelView::Draw() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();

  directx::device::CommandList& command_list =
      device.current_resource_->command_lists_[device.MID_COMMAND_LIST_ID];
  root_signature_.SetGraphicsCommandList(command_list);
  pipeline_.SetGraphicsCommandList(command_list);
  device.heap_manager_.SetGraphicsCommandList(command_list);
  camera_.RenderStart();
  transform_cb_.SetToHeap(device);
  model_.Draw(command_list);
}

void ModelView::Finalize() {}

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend

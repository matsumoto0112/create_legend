#include "src/scenes/debugscene/enemy_move_viewer.h"

#include "src/directx/shader/alpha_blend_desc.h"
#include "src/directx/shader/shader_register_id.h"
#include "src/game/game_device.h"
#include "src/util/path.h"

namespace legend {
namespace scenes {
namespace debugscene {

//コンストラクタ
EnemyMoveViewer::EnemyMoveViewer(ISceneChange* scene_change)
    : Scene(scene_change) {}

EnemyMoveViewer::~EnemyMoveViewer() {}

//初期化
bool EnemyMoveViewer::Initialize() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& resource = game::GameDevice::GetInstance()->GetResource();

  directx::device::CommandList command_list;
  if (!command_list.Init(
          device, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT)) {
    return false;
  }

  device.GetHeapManager().AddLocalHeap(
      device,
      directx::descriptor_heap::heap_parameter::LocalHeapID::ENEMY_MOVE_TEST);

  //このシーンで使用するシェーダーを事前に読み込んでおく
  if (!resource.GetVertexShader().Load(
          util::resource::id::VertexShader::MODEL_VIEW,
          util::Path::GetInstance()->shader() / "modelview" /
              "model_view_vs.cso")) {
    return false;
  }
  if (!resource.GetPixelShader().Load(
          util::resource::id::PixelShader::MODEL_VIEW,
          util::Path::GetInstance()->shader() / "modelview" /
              "model_view_ps.cso")) {
    return false;
  }
  //モデルデータを読み込む
  const std::filesystem::path model_path =
      util::Path::GetInstance()->model() / "eraser_01.glb";
  if (!resource.GetModel().Load(util::resource::id::Model::ERASER, model_path,
                                command_list)) {
    return false;
  }

  auto gps = std::make_shared<directx::shader::GraphicsPipelineState>();
  D3D12_GRAPHICS_PIPELINE_STATE_DESC pso_desc = {};
  pso_desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
  pso_desc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC1(D3D12_DEFAULT);
  pso_desc.DSVFormat = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
  pso_desc.InputLayout = resource.GetVertexShader()
                             .Get(util::resource::id::VertexShader::MODEL_VIEW)
                             ->GetInputLayout();
  pso_desc.NumRenderTargets = 1;
  pso_desc.PrimitiveTopologyType =
      D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
  pso_desc.pRootSignature =
      device.GetDefaultRootSignature()->GetRootSignature();
  pso_desc.PS = resource.GetPixelShader()
                    .Get(util::resource::id::PixelShader::MODEL_VIEW)
                    ->GetShaderBytecode();
  pso_desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
  pso_desc.RTVFormats[0] = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
  pso_desc.SampleDesc.Count = 1;
  pso_desc.SampleMask = UINT_MAX;
  pso_desc.VS = resource.GetVertexShader()
                    .Get(util::resource::id::VertexShader::MODEL_VIEW)
                    ->GetShaderBytecode();
  if (!gps->Init(device, pso_desc)) {
    return false;
  }
  resource.GetPipeline().Register(util::resource::id::Pipeline::MODEL_VIEW,
                                  gps);

  if (!enemy_manager_.Initilaize()) {
    return false;
  }

  //カメラの初期化
  {
    const math::Vector3 camera_position = math::Vector3(0, 2, -2);
    const math::Quaternion camera_rotation =
        math::Quaternion::FromEular(math::util::DEG_2_RAD * 45.0f, 0.0f, 0.0f);
    const math::IntVector2 screen_size =
        game::GameDevice::GetInstance()->GetWindow().GetScreenSize();
    const float aspect_ratio = screen_size.x * 1.0f / screen_size.y;
    if (!camera_.Init(L"MainCamera", camera_position, camera_rotation,
                      math::util::DEG_2_RAD * 50.0f, aspect_ratio)) {
      return false;
    }
  }

  command_list.Close();
  device.ExecuteCommandList({command_list});
  device.WaitExecute();
  return true;
}

//更新
bool EnemyMoveViewer::Update() {
  if (ImGui::Begin("Transform")) {
    //カメラ座標
    math::Vector3 camera_position = camera_.GetPosition();
    ImGui::SliderFloat3("Position", &camera_position.x, -100.0f, 100.0f);
    camera_.SetPosition(camera_position);
    //カメラ回転角
    math::Vector3 camera_rotation =
        math::Quaternion::ToEular(camera_.GetRotation()) *
        math::util::RAD_2_DEG;
    ImGui::SliderFloat3("Rotation", &camera_rotation.x, -180.0f, 180.0f);
    camera_.SetRotation(
        math::Quaternion::FromEular(camera_rotation * math::util::DEG_2_RAD));

    //カメラの上方向ベクトルを変更する
    if (ImGui::Button("X_UP")) {
      camera_.SetUpVector(math::Vector3::kRightVector);
    }
    if (ImGui::Button("Y_UP")) {
      camera_.SetUpVector(math::Vector3::kUpVector);
    }
    if (ImGui::Button("Z_UP")) {
      camera_.SetUpVector(math::Vector3::kForwardVector);
    }
    float fov = camera_.GetFov() * math::util::RAD_2_DEG;
    ImGui::SliderFloat("FOV", &fov, 0.01f, 90.0f);
    camera_.SetFov(fov * math::util::DEG_2_RAD);
  }
  ImGui::End();

  // float speed = 0.1f;
  // auto pos = transform_.GetPosition();
  // auto h = game::GameDevice::GetInstance()->GetInput().GetHorizontal();
  // auto v = game::GameDevice::GetInstance()->GetInput().GetVertical();
  // auto mov = (math::Vector3(h, 0, v)).Normalized();
  // transform_.SetPosition(pos + mov * speed/*
  // *game::GameDevice::GetInstance()->GetFPSCounter().GetTotalSeconds()*/);
  // transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  // transform_cb_.UpdateStaging();

  enemy_manager_.Update();

  return true;
}

//描画
void EnemyMoveViewer::Draw() {
  Scene::Draw();

  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& render_resource_manager = device.GetRenderResourceManager();
  auto& resource = game::GameDevice::GetInstance()->GetResource();
  auto& command_list = device.GetCurrentFrameResource()->GetCommandList();

  render_resource_manager.SetRenderTargets(
      command_list, directx::render_target::RenderTargetID::BACK_BUFFER, false,
      directx::render_target::DepthStencilTargetID::DEPTH_ONLY, true);

  game::GameDevice::GetInstance()
      ->GetResource()
      .GetPipeline()
      .Get(util::resource::id::Pipeline::MODEL_VIEW)
      ->SetGraphicsCommandList(command_list);
  camera_.RenderStart();

  enemy_manager_.Draw();
}

void EnemyMoveViewer::Finalize() {
  util::resource::Resource& resource =
      game::GameDevice::GetInstance()->GetResource();
  resource.GetVertexShader().Unload(
      util::resource::id::VertexShader::MODEL_VIEW);
  resource.GetPixelShader().Unload(util::resource::id::PixelShader::MODEL_VIEW);
  resource.GetPipeline().Unload(util::resource::id::Pipeline::MODEL_VIEW);
  resource.GetModel().Unload(util::resource::id::Model::ERASER);
  game::GameDevice::GetInstance()->GetDevice().GetHeapManager().RemoveLocalHeap(
      directx::descriptor_heap::heap_parameter::LocalHeapID::ENEMY_MOVE_TEST);
}

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend

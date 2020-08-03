#include "src/scenes/debugscene/enemy_move_viewer.h"

#include "src/directx/shader/alpha_blend_desc.h"
#include "src/directx/shader/shader_register_id.h"
#include "src/game/game_device.h"
#include "src/util/path.h"
#include "src/util/resource/pixel_shader.h"
#include "src/util/resource/vertex_shader.h"

namespace legend {
namespace scenes {
namespace debugscene {

//コンストラクタ
EnemyMoveViewer::EnemyMoveViewer(ISceneChange* scene_change)
    : Scene(scene_change), transform_() {}

EnemyMoveViewer::~EnemyMoveViewer() {
  game::GameDevice::GetInstance()->GetDevice().WaitForGPU();
}

//初期化
bool EnemyMoveViewer::Initialize() {
  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();

  //このシーンで使用するシェーダーを事前に読み込んでおく
  util::resource::Resource& resource =
      game::GameDevice::GetInstance()->GetResource();
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
      util::Path::GetInstance()->model() / "1000cmObject.glb";
  if (!resource.GetModel().Load(util::resource::ModelID::OBJECT_1000CM,
                                model_path)) {
    return false;
  }

  auto gps = std::make_shared<directx::shader::GraphicsPipelineState>();
  gps->Init(device);
  gps->SetVertexShader(resource.GetVertexShader().Get(
      util::resource::id::VertexShader::MODEL_VIEW));
  gps->SetPixelShader(resource.GetPixelShader().Get(
      util::resource::id::PixelShader::MODEL_VIEW));
  gps->SetBlendDesc(directx::shader::alpha_blend_desc::BLEND_DESC_ALIGNMENT, 0);
  device.GetRenderResourceManager().WriteRenderTargetInfoToPipeline(
      device, directx::render_target::RenderTargetID::BACK_BUFFER, gps.get());
  device.GetRenderResourceManager().WriteDepthStencilTargetInfoToPipeline(
      device, directx::render_target::DepthStencilTargetID::Depth, gps.get());
  gps->SetRootSignature(device.GetDefaultRootSignature());
  gps->SetPrimitiveTopology(
      D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
  gps->CreatePipelineState(device);
  resource.GetPipeline().Register(util::resource::id::Pipeline::MODEL_VIEW,
                                  gps);

  //トランスフォームバッファを作成する
  if (!transform_cb_.Init(
          device, directx::shader::ConstantBufferRegisterID::Transform,
          device.GetLocalHeapHandle(directx::descriptor_heap::heap_parameter::
                                        LocalHeapID::MODEL_VIEW_SCENE),
          L"Transform ConstantBuffer")) {
    return false;
  }

  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();

  //カメラの初期化
  {
    const math::Vector3 camera_position = math::Vector3(0, 10, -10);
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

  float speed = 0.1f;
  auto pos = transform_.GetPosition();
  auto h = game::GameDevice::GetInstance()->GetInput().GetHorizontal();
  auto v = game::GameDevice::GetInstance()->GetInput().GetVertical();
  auto mov = (math::Vector3(h, 0, v)).Normalized();

  transform_.SetPosition(pos + mov * speed/* *game::GameDevice::GetInstance()->GetFPSCounter().GetTotalSeconds()*/);
  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();

  return true;
}

//描画
void EnemyMoveViewer::Draw() {
  Scene::Draw();

  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();
  device.GetRenderResourceManager().SetDepthStencilTargetID(
      directx::render_target::DepthStencilTargetID::Depth);
  device.GetRenderResourceManager().SetRenderTargetsToCommandList(device);
  device.GetRenderResourceManager().ClearCurrentDepthStencilTarget(device);

  game::GameDevice::GetInstance()
      ->GetResource()
      .GetPipeline()
      .Get(util::resource::id::Pipeline::MODEL_VIEW)
      ->SetGraphicsCommandList(device);
  camera_.RenderStart();
  transform_cb_.SetToHeap(device);
  game::GameDevice::GetInstance()
      ->GetResource()
      .GetModel()
      .Get(util::resource::ModelID::OBJECT_1000CM)
      ->Draw();
}

void EnemyMoveViewer::Finalize() {
  game::GameDevice::GetInstance()->GetDevice().WaitForGPU();

  util::resource::Resource& resource =
      game::GameDevice::GetInstance()->GetResource();
  resource.GetVertexShader().Unload(
      util::resource::id::VertexShader::MODEL_VIEW);
  resource.GetPixelShader().Unload(util::resource::id::PixelShader::MODEL_VIEW);
  resource.GetPipeline().Unload(util::resource::id::Pipeline::MODEL_VIEW);
  resource.GetModel().Unload(util::resource::ModelID::OBJECT_1000CM);
}

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend

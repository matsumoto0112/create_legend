#include "src/scenes/debugscene/perspective_camera_test.h"

#include "src/directx/shader/alpha_blend_desc.h"
#include "src/directx/shader/shader_register_id.h"
#include "src/directx/vertex.h"
#include "src/game/game_device.h"
#include "src/util/loader/glb_loader.h"
#include "src/util/path.h"

namespace legend {
namespace scenes {
namespace debugscene {

//コンストラクタ
PerspectiveCameraTest::PerspectiveCameraTest(ISceneChange* scene_change)
    : Scene(scene_change) {}

//デストラクタ
PerspectiveCameraTest::~PerspectiveCameraTest() {}

//初期化
bool PerspectiveCameraTest::Initialize() {
  std::filesystem::path filepath =
      util::Path::GetInstance()->model() / L"checkXYZ.glb";

  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();
  if (!model_.Init(filepath)) {
    return false;
  }

  if (!pipeline_state_.Init(device)) {
    return false;
  }
  pipeline_state_.SetRootSignature(device.GetDefaultRootSignature());

  const std::filesystem::path shader_root_path =
      util::Path::GetInstance()->shader();
  auto vertex_shader = std::make_shared<directx::shader::VertexShader>();
  if (!vertex_shader->Init(
          device, shader_root_path / L"modelview" / L"model_view_vs.cso",
          directx::input_element::GetElementDescs<directx::Vertex>())) {
    return false;
  }
  pipeline_state_.SetVertexShader(vertex_shader);

  auto pixel_shader = std::make_shared<directx::shader::PixelShader>();
  if (!pixel_shader->Init(
          device, shader_root_path / L"modelview" / L"model_view_ps.cso")) {
    return false;
  }
  pipeline_state_.SetPixelShader(pixel_shader);

  pipeline_state_.SetBlendDesc(
      directx::shader::alpha_blend_desc::BLEND_DESC_DEFAULT, 0);
  pipeline_state_.SetRenderTargetInfo(device.GetRenderTarget(), true);
  if (!pipeline_state_.CreatePipelineState(device)) {
    return false;
  }

  const math::Quaternion camera_rotation = math::Quaternion::kIdentity;
  const math::IntVector2 screen_size =
      game::GameDevice::GetInstance()->GetWindow().GetScreenSize();
  const float aspect_ratio = screen_size.x * 1.0f / screen_size.y;
  if (!camera_.Init(L"MainCamera", math::Vector3(0.0f, 1.0f, -1.0f),
                    camera_rotation, 60.0f * math::util::DEG_2_RAD,
                    aspect_ratio, math::Vector3::kUpVector)) {
    return false;
  }

  return true;
}

//更新
bool PerspectiveCameraTest::Update() {
  if (ImGui::Begin("Camera")) {
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
  return true;
}

//描画
void PerspectiveCameraTest::Draw() {
  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();

  camera_.RenderStart();
  pipeline_state_.SetGraphicsCommandList(device);
  device.GetHeapManager().SetGraphicsCommandList(device);
  model_.Draw();
}

//終了処理
void PerspectiveCameraTest::Finalize() {}

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend

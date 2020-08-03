#include "src/scenes/mainscene/main_scene_1.h"

#include "src/game/game_device.h"

namespace legend {
namespace scenes {
namespace mainscene {

//コンストラクタ
MainScene1::MainScene1(ISceneChange* scene_change) : Scene(scene_change) {}

//デストラクタ
MainScene1::~MainScene1() {
  game::GameDevice::GetInstance()->GetDevice().WaitForGPU();
}

//初期化
bool MainScene1::Initialize() {
  //カメラの初期化
  {
    const math::Vector3 camera_position = math::Vector3(0, 0.5f, -0.5f);
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
bool MainScene1::Update() {
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

  return true;
}

//描画
void MainScene1::Draw() {
  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();
  //device.GetRenderResourceManager().SetDepthStencilTargetID(
  //    directx::render_target::DepthStencilTargetID::Depth);
  //device.GetRenderResourceManager().SetRenderTargetsToCommandList(device);
  //device.GetRenderResourceManager().ClearCurrentDepthStencilTarget(device);

  //game::GameDevice::GetInstance()
  //    ->GetResource()
  //    .GetPipeline()
  //    .Get(util::resource::id::Pipeline::MODEL_VIEW)
  //    ->SetGraphicsCommandList(device);
  camera_.RenderStart();
}

//終了
void MainScene1::Finalize() {
  game::GameDevice::GetInstance()->GetDevice().WaitForGPU();

  util::resource::Resource& resource =
      game::GameDevice::GetInstance()->GetResource();
}
}  // namespace mainscene
}  // namespace scenes
}  // namespace legend
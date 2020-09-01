#include "src/camera/camera_manager.h"

#include "src/camera/lookat_target_camera.h"
#include "src/camera/perspective_camera.h"
#include "src/game/game_device.h"
#include "src/player/player.h"

namespace legend {
namespace camera {

CameraManager::CameraManager() {}

CameraManager::~CameraManager() {}

bool CameraManager::Init(actor::IActorMediator* mediator) {
  actor_mediator_ = mediator;

  const math::IntVector2 screen_size =
      game::GameDevice::GetInstance()->GetWindow().GetScreenSize();
  const float aspect_ratio = screen_size.x * 1.0f / screen_size.y;

  auto CreateMainCamera = [&]() -> std::unique_ptr<camera::LookAtTargetCamera> {
    const math::Quaternion camera_rotation =
        math::Quaternion::FromEular(math::util::DEG_2_RAD * 45.0f, 0.0f, 0.0f);
    auto main_camera = std::make_unique<camera::LookAtTargetCamera>();
    if (!main_camera->Init(L"MainCamera", mediator->GetPlayer(), 20.0f, 30.0f,
                           math::util::DEG_2_RAD * 50.0f, aspect_ratio,
                           math::Vector3::kUpVector, 0.1f, 1000.0f)) {
      return nullptr;
    }

    return std::move(main_camera);
  };

  auto CreateBirdsEyeViewCamera =
      [&]() -> std::unique_ptr<camera::PerspectiveCamera> {
    auto camera = std::make_unique<camera::PerspectiveCamera>();
    if (!camera->Init(L"BirdsEyeViewCamera", math::Vector3(0.0f, 100.0f, 0.0f),
                      math::Quaternion::FromEular(90.0f * math::util::DEG_2_RAD,
                                                  0.0f, 0.0f),
                      50.0f * math::util::DEG_2_RAD, aspect_ratio,
                      math::Vector3::kForwardVector, 0.1f, 300.0f)) {
      return nullptr;
    }
    return std::move(camera);
  };

  player_lookat_camera_ = CreateMainCamera();
  birds_eye_view_camera_ = CreateBirdsEyeViewCamera();

  return true;
}
void CameraManager::UpdateCamera() {
  //メインカメラの回転処理
  auto& input = game::GameDevice::GetInstance()->GetInput();
  float theta = player_lookat_camera_->GetTheta();

  const float delta_time =
      game::GameDevice::GetInstance()->GetFPSCounter().GetDeltaSeconds<float>();

  //右コントローラの入力を取得する
  const math::Vector2 right_input = input.GetGamepad()->GetStickRight();
  constexpr float POWER = 1.0f;
  theta += right_input.x * POWER * delta_time;
  player_lookat_camera_->SetTheta(theta);

  const math::Vector3 player_position =
      actor_mediator_->GetPlayer()->GetPosition();
  birds_eye_view_camera_->SetPosition(
      math::Vector3(player_position.x, birds_eye_view_camera_->GetPosition().y,
                    player_position.z));
}

void CameraManager::SetCameraMode(camera_mode::Enum camera_mode) {
  current_camera_mode_ = camera_mode;
}

camera::Camera* CameraManager::GetCurrentCamera() const {
  switch (current_camera_mode_) {
    case legend::camera::camera_mode::PLAYER_LOOKAT:
      return player_lookat_camera_.get();
    case legend::camera::camera_mode::BIRDS_EYE_VIEW:
      return birds_eye_view_camera_.get();
  }
  MY_ASSERTION(false, L"未定義のカメラが選択されました。");
  return nullptr;
}

void CameraManager::RenderStart() {
  switch (current_camera_mode_) {
    case legend::camera::camera_mode::PLAYER_LOOKAT:
      player_lookat_camera_->RenderStart();
      break;
    case legend::camera::camera_mode::BIRDS_EYE_VIEW:
      birds_eye_view_camera_->RenderStart();
      break;
  }
}

}  // namespace camera
}  // namespace legend
#include "src/camera/camera_manager.h"

#include "src/game/game_device.h"
#include "src/player/player.h"

namespace {
int id;
enum Type {
  Type_A,  //ランダム
  Type_B,  //遠いやつ
  Type_C,  //近いやつ
  MAX
} type_;
}  // namespace

namespace legend {
namespace camera {

CameraManager::CameraManager() {}

CameraManager::~CameraManager() {}

bool CameraManager::Init(
    actor::IActorMediator* mediator,
    const std::vector<stage_generate::CameraGenerateInfo>& infos) {
  actor_mediator_ = mediator;
  type_ = Type::Type_C;

  const math::IntVector2 screen_size =
      game::GameDevice::GetInstance()->GetWindow().GetScreenSize();
  const float aspect_ratio = screen_size.x * 1.0f / screen_size.y;
  const float fov = math::util::DEG_2_RAD * 50.0f;
  const float near_z = 0.1f;
  const float far_z = 1000.0f;

  auto CreateMainCamera =
      [&]() -> std::unique_ptr<camera::LookatFollowTargetCamera> {
    const math::Quaternion camera_rotation =
        math::Quaternion::FromEular(math::util::DEG_2_RAD * 45.0f, 0.0f, 0.0f);
    auto main_camera = std::make_unique<camera::LookatFollowTargetCamera>();
    if (!main_camera->Init(L"MainCamera", mediator->GetPlayer(), 20.0f, 30.0f,
                           fov, aspect_ratio, math::Vector3::kUpVector, near_z,
                           far_z)) {
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
                      fov, aspect_ratio, math::Vector3::kForwardVector, near_z,
                      far_z)) {
      return nullptr;
    }
    return std::move(camera);
  };

  player_lookat_camera_ = CreateMainCamera();
  birds_eye_view_camera_ = CreateBirdsEyeViewCamera();

  const u32 size = static_cast<u32>(infos.size());
  for (u32 i = 0; i < size; i++) {
    auto camera = std::make_unique<camera::LookatTargetCamera>();
    const std::wstring name =
        util::string_util::Format(L"LookatTargetCamera[%d]", i);
    if (!camera->Init(name, actor_mediator_->GetPlayer(), infos[i].position,
                      fov, aspect_ratio, math::Vector3::kUpVector, near_z,
                      far_z)) {
      return false;
    }
    free_cameras_.emplace_back(std::move(camera));
  }

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

  // if (input.GetKeyboard()->GetKeyDown(input::key_code::C)) {
  //  type_ = static_cast<Type>((type_ + 1) % Type::MAX);
  //}
}

void CameraManager::SetCameraMode(camera_mode::Enum camera_mode) {
  current_camera_mode_ = camera_mode;
  id = game::GameDevice::GetInstance()->GetRandom().Range(
      0, static_cast<i32>(free_cameras_.size()));
}

camera::Camera* CameraManager::GetCurrentCamera() const {
  auto GetMostFarCameraIDBetweenPlayer = [&]() {
    std::vector<float> distances(free_cameras_.size());
    const u64 size = free_cameras_.size();
    for (u64 i = 0; i < size; i++) {
      distances[i] = (free_cameras_[i]->GetPosition() -
                      actor_mediator_->GetPlayer()->GetPosition())
                         .MagnitudeSquared();
    }
    const auto min_dist_iter =
        std::max_element(distances.begin(), distances.end());
    const i32 index =
        static_cast<i32>(std::distance(distances.begin(), min_dist_iter));
    return index;
  };

  auto GetMostNearCameraIDBetweenPlayer = [&]() {
    std::vector<float> distances(free_cameras_.size());
    const u64 size = free_cameras_.size();
    for (u64 i = 0; i < size; i++) {
      distances[i] = (free_cameras_[i]->GetPosition() -
                      actor_mediator_->GetPlayer()->GetPosition())
                         .MagnitudeSquared();
    }
    const auto min_dist_iter =
        std::min_element(distances.begin(), distances.end());
    const i32 index =
        static_cast<i32>(std::distance(distances.begin(), min_dist_iter));
    return index;
  };

  id = [&]() {
    switch (type_) {
      case Type_A:
        return id;
      case Type_B:
        return GetMostFarCameraIDBetweenPlayer();
      case Type_C:
        return GetMostNearCameraIDBetweenPlayer();
      default:
        return 0;
    }
  }();

  switch (current_camera_mode_) {
    case legend::camera::camera_mode::PLAYER_LOOKAT:
      return player_lookat_camera_.get();
    case legend::camera::camera_mode::BIRDS_EYE_VIEW:
      return birds_eye_view_camera_.get();
    case legend::camera::camera_mode::FREE:
      return free_cameras_[id].get();
  }
  MY_ASSERTION(false, L"未定義のカメラが選択されました。");
  return nullptr;
}

void CameraManager::RenderStart() { GetCurrentCamera()->RenderStart(); }

}  // namespace camera
}  // namespace legend
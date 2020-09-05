#include "src/camera/perspective_camera.h"

#include "src/directx/shader/shader_register_id.h"
#include "src/game/game_device.h"

namespace {
constexpr bool ENABLE_DEBUG_CAMERA = false;
}  // namespace

namespace legend {
namespace camera {

//コンストラクタ
PerspectiveCamera::PerspectiveCamera()
    : Camera(),
      position_(math::Vector3::kZeroVector),
      rotation_(math::Quaternion::kIdentity),
      up_vector_(math::Vector3::kUpVector),
      fov_(90.0f),
      aspect_ratio_(1.0f),
      near_z_(0.1f),
      far_z_(300.0f) {}

//デストラクタ
PerspectiveCamera::~PerspectiveCamera() {}

//初期化
bool PerspectiveCamera::Init(const std::wstring& name,
                             const math::Vector3& position,
                             const math::Quaternion& rotation, float fov,
                             float aspect_ratio, const math::Vector3& up_vector,
                             float near_z, float far_z) {
  this->name_ = name;
  this->position_ = position;
  this->rotation_ = rotation;
  this->up_vector_ = up_vector;
  this->fov_ = fov;
  this->aspect_ratio_ = aspect_ratio;
  this->near_z_ = near_z;
  this->far_z_ = far_z;
  this->update_enable_ = true;

  return Camera::Init(name, CreateView(), CreateProjection());
}

//コンスタントバッファの更新
void PerspectiveCamera::UpdateConstantBufferStaging() {
  if (update_enable_) {
    if constexpr (ENABLE_DEBUG_CAMERA) {
      auto& input = game::GameDevice::GetInstance()->GetInput();
      const float input_vert = input.GetGamepad()->GetStickRight().y;
      position_ += math::Vector3(0, input_vert, 0);
    }
    view_ = CreateView();
    projection_ = CreateProjection();
  }

  Camera::UpdateConstantBufferStaging();
}

//ビュー行列
math::Matrix4x4 PerspectiveCamera::CreateView() const {
  const math::Vector3 forward_vector =
      rotation_ * math::Vector3::kForwardVector;
  const math::Vector3 lookat = position_ + forward_vector;
  return math::Matrix4x4::CreateView(position_, lookat, up_vector_);
}

//プロジェクション行列
math::Matrix4x4 PerspectiveCamera::CreateProjection() const {
  return math::Matrix4x4::CreateProjection(fov_, aspect_ratio_, near_z_,
                                           far_z_);
}

}  // namespace camera
}  // namespace legend

#include "src/camera/perspective_camera.h"

#include "src/directx/shader/shader_register_id.h"
#include "src/game/game_device.h"

namespace legend {
namespace camera {

//コンストラクタ
PerspectiveCamera::PerspectiveCamera()
    : name_(L""),
      position_(math::Vector3::kZeroVector),
      rotation_(math::Quaternion::kIdentity),
      up_vector_(math::Vector3::kUpVector),
      fov_(90.0f),
      aspect_ratio_(1.0f),
      near_z_(0.1f),
      far_z_(300.0f),
      world_context_constant_buffer_() {}

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

  if (!InitConstantBuffer()) {
    return false;
  }

  UpdateConstantBufferStaging();
  return true;
}

//描画開始
void PerspectiveCamera::RenderStart() {
  UpdateConstantBufferStaging();

  world_context_constant_buffer_.RegisterHandle(
      game::GameDevice::GetInstance()->GetDevice(),
      directx::shader::ConstantBufferRegisterID::WORLD_CONTEXT);
}

//コンスタントバッファの初期化
bool PerspectiveCamera::InitConstantBuffer() {
  if (!world_context_constant_buffer_.Init(
          game::GameDevice::GetInstance()->GetDevice(),
          game::GameDevice::GetInstance()->GetDevice().GetLocalHandle(
              directx::descriptor_heap::heap_parameter::LocalHeapID::ONE_PLAY),
          name_ + L"_WorldContext_ConstantBuffer")) {
    return false;
  }
  return true;
}

//コンスタントバッファの更新
void PerspectiveCamera::UpdateConstantBufferStaging() {
  const math::Vector3 forward_vector =
      rotation_ * math::Vector3::kForwardVector;
  const math::Vector3 lookat = position_ + forward_vector;

  world_context_constant_buffer_.GetStagingRef().view =
      math::Matrix4x4::CreateView(position_, lookat, up_vector_);
  world_context_constant_buffer_.GetStagingRef().projection =
      math::Matrix4x4::CreateProjection(fov_, aspect_ratio_, near_z_, far_z_);
  world_context_constant_buffer_.UpdateStaging();
}

}  // namespace camera
}  // namespace legend

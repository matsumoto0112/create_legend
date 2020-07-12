#include "src/camera/perspective_camera.h"

#include "src/directx/shader/shader_register_id.h"
#include "src/game/game_device.h"

namespace legend {
namespace camera {

//コンストラクタ
PerspectiveCamera::PerspectiveCamera() {}

//デストラクタ
PerspectiveCamera::~PerspectiveCamera() {}

//初期化
bool PerspectiveCamera::Init(const std::wstring& name,
                             const math::Vector3& position,
                             const math::Vector3& lookat,
                             const math::Vector3& up_vector, float fov,
                             float aspect_ratio, float near_z, float far_z) {
  this->name_ = name;
  if (!world_context_constant_buffer_.Init(
          game::GameDevice::GetInstance()->GetDevice(),
          directx::shader::ConstantBufferRegisterID::WorldContext,
          name_ + L"_WorldContext_ConstantBuffer")) {
    return false;
  }

  world_context_constant_buffer_.GetStagingRef().view =
      math::Matrix4x4::CreateView(position, lookat, up_vector);
  world_context_constant_buffer_.GetStagingRef().projection =
      math ::Matrix4x4::CreateProjection(fov, aspect_ratio, near_z, far_z);
  world_context_constant_buffer_.UpdateStaging();
  return true;
}

void PerspectiveCamera::RenderStart() {
  world_context_constant_buffer_.UpdateStaging();
  world_context_constant_buffer_.SetToHeap(
      game::GameDevice::GetInstance()->GetDevice());
}

}  // namespace camera
}  // namespace legend

#include "src/camera/perspective_camera.h"

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
                             const math::Quaternion& rotation,
                             const math::Vector3& up_vector, float fov,
                             float aspect_ratio, float near_z, float far_z) {
  if (!world_context_constant_buffer_.Init(
          game::GameDevice::GetInstance()->GetDevice(), 1,
          name + L"_WorldContext_ConstantBuffer")) {
    return false;
  }
  world_context_constant_buffer_.GetStagingRef().view =
      math::Matrix4x4::CreateView(math::Vector3(1, 1, -1),
                                  math::Vector3(0, 0, 0),
                                  math::Vector3::kUpVector);
  world_context_constant_buffer_.GetStagingRef().projection =
      math ::Matrix4x4::CreateProjection(45.0f * math::util::DEG_2_RAD,
                                         1280.0f / 720.0f, 0.1f, 300.0f);
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

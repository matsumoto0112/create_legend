#include "src/camera/perspective_camera.h"

#include "src/directx/shader/shader_register_id.h"
#include "src/game/game_device.h"

namespace legend {
namespace camera {

//�R���X�g���N�^
PerspectiveCamera::PerspectiveCamera() {}

//�f�X�g���N�^
PerspectiveCamera::~PerspectiveCamera() {}

//������
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

//�`��J�n
void PerspectiveCamera::RenderStart() {
  UpdateConstantBufferStaging();

  world_context_constant_buffer_.SetToHeap(
      game::GameDevice::GetInstance()->GetDevice());
}

//�R���X�^���g�o�b�t�@�̏�����
bool PerspectiveCamera::InitConstantBuffer() {
  if (!world_context_constant_buffer_.Init(
          game::GameDevice::GetInstance()->GetDevice(),
          directx::shader::ConstantBufferRegisterID::WorldContext,
          game::GameDevice::GetInstance()->GetDevice().GetLocalHeapHandle(0),
          name_ + L"_WorldContext_ConstantBuffer")) {
    return false;
  }
  return true;
}

//�R���X�^���g�o�b�t�@�̍X�V
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

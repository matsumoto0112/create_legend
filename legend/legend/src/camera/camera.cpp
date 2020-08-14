#include "src/camera/camera.h"

#include "src/directx/shader/shader_register_id.h"
#include "src/game/game_device.h"

namespace legend {
namespace camera {

//�R���X�g���N�^
Camera::Camera() : name_(L""), world_context_constant_buffer_() {}

//�f�X�g���N�^
Camera::~Camera() {}

//������
bool Camera::Init(const std::wstring& name, const math::Matrix4x4& view,
                  const math::Matrix4x4& projection) {
  if (!InitConstantBuffer()) {
    return false;
  }

  UpdateConstantBufferStaging();
  return true;
}

//�`��J�n
void Camera::RenderStart() {
  UpdateConstantBufferStaging();

  auto& device = game::GameDevice::GetInstance()->GetDevice();
  const u32 register_id =
      directx::shader::ConstantBufferRegisterID::WORLD_CONTEXT;
  world_context_constant_buffer_.RegisterHandle(device, register_id);
}

//������
bool Camera::InitConstantBuffer() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  const directx::descriptor_heap::DescriptorHandle handle =
      device.GetLocalHandle(
          directx::descriptor_heap::heap_parameter::LocalHeapID::ONE_PLAY);
  if (!world_context_constant_buffer_.Init(
          device, handle, name_ + L"_WorldContext_ConstantBuffer")) {
    return false;
  }

  return true;
}

//�X�V
void Camera::UpdateConstantBufferStaging() {
  world_context_constant_buffer_.GetStagingRef().view = view_;
  world_context_constant_buffer_.GetStagingRef().projection = projection_;
  world_context_constant_buffer_.UpdateStaging();
}

}  // namespace camera
}  // namespace legend
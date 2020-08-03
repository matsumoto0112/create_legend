#include "src/draw/particle/smoke_particle.h"

#include "src/directx/shader/shader_register_id.h"
#include "src/game/game_device.h"

namespace legend {
namespace draw {
namespace particle {

SmokeParticle::SmokeParticle() {}

SmokeParticle::~SmokeParticle() {}

bool SmokeParticle::Init(std::shared_ptr<directx::buffer::Texture2D> texture) {
  ParticleEmitter::Init(texture);

  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();
  util::Random& random = game::GameDevice::GetInstance()->GetRandom();

  for (u32 i = 0; i < 100; i++) {
    const math::Vector3 position(random.Range(-5.0f, 5.0f), 0.0f,
                                 random.Range(-5.0f, 5.0f));
    const math::Quaternion rotation =
        math::Quaternion::FromEular(0.0f, math::util::DEG_2_RAD * 180.0f, 0.0f);
    util::Transform transform(position, rotation);
    particle_transforms_.emplace_back(transform);

    directx::buffer::ConstantBuffer<
        directx::constant_buffer_structure::Transform>
        cb;
    const std::wstring name =
        util::string_util::Format(L"SmokeParticle_ConstantBuffer %d", i);
    if (!cb.Init(
            device, directx::shader::ConstantBufferRegisterID::Transform,
            device.GetLocalHeapHandle(directx::descriptor_heap::heap_parameter::
                                          LocalHeapID::GLOBAL_ID),
            name)) {
      return false;
    }
    constant_buffers_.emplace_back(cb);
  }

  return true;
}

bool SmokeParticle::Update(const math::Matrix4x4& billboard_matrix) {
  const u32 particle_num = static_cast<u32>(particle_transforms_.size());
  util::Random& random = game::GameDevice::GetInstance()->GetRandom();

  for (u32 i = 0; i < particle_num; i++) {
    particle_transforms_.pop_front();
    const math::Vector3 position(random.Range(-5.0f, 5.0f), 0.0f,
                                 random.Range(-5.0f, 5.0f));
    const math::Quaternion rotation =
        math::Quaternion::FromEular(0.0f, math::util::DEG_2_RAD * 180.0f, 0.0f);
    util::Transform transform(position, rotation);
    particle_transforms_.emplace_back(transform);

    particle_transforms_[i].SetPosition(particle_transforms_[i].GetPosition() +
                                        math::Vector3(0, 0.1f, 0));
    constant_buffers_[i].GetStagingRef().world =
        particle_transforms_[i].CreateWorldMatrix() * billboard_matrix;
  }
  return true;
}

}  // namespace particle
}  // namespace draw
}  // namespace legend
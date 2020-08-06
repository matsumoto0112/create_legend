#include "src/draw/particle/particle_emitter.h"

#include "src/game/game_device.h"

namespace legend {
namespace draw {
namespace particle {

ParticleEmitter::ParticleEmitter() {}

ParticleEmitter::~ParticleEmitter() {}

bool ParticleEmitter::Init(
    std::shared_ptr<directx::buffer::Texture2D> texture) {
  texture_ = texture;
  particle_transforms_.clear();
  constant_buffers_.clear();

  return true;
}

void ParticleEmitter::Render(directx::buffer::IndexBuffer& draw_index_buffer) {
  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();
  texture_->SetToHeap(device);

  for (auto& cb : constant_buffers_) {
    cb.UpdateStaging();
    cb.SetToHeap(device);
    //device.GetHeapManager().CopyHeapAndSetToGraphicsCommandList(device);

    draw_index_buffer.Draw(device);
  }
}

}  // namespace particle
}  // namespace draw
}  // namespace legend

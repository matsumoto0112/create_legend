#include "src/draw/particle/particle_manager.h"

#include "src/directx/directx_helper.h"

namespace legend {
namespace draw {
namespace particle {

using directx::directx_helper::Failed;

ParticleManager::ParticleManager() {}

ParticleManager::~ParticleManager() {}

bool ParticleManager::Init(directx::device::IDirectXAccessor& accessor,
                           u32 frame_count) {
  D3D12_COMMAND_QUEUE_DESC compute_queue_desc = {};
  compute_queue_desc.Flags =
      D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;
  compute_queue_desc.Type =
      D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_COMPUTE;
  if (Failed(accessor.GetDevice()->CreateCommandQueue(
          &compute_queue_desc, IID_PPV_ARGS(&command_queue_)))) {
    return false;
  }

  frame_resources_.resize(frame_count);
  for (u32 i = 0; i < frame_count; i++) {
    if (!frame_resources_[i].Init(
            accessor,
            D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_COMPUTE)) {
      return false;
    }
  }

  if (Failed(accessor.GetDevice()->CreateFence(
          fence_value_, D3D12_FENCE_FLAGS::D3D12_FENCE_FLAG_NONE,
          IID_PPV_ARGS(&fence_)))) {
    return false;
  }
  fence_value_++;

  return true;
}

void ParticleManager::AddParticle(std::unique_ptr<ParticleSystem> particle) {
  particles_.emplace_back(std::move(particle));
}

void ParticleManager::BeginFrame(directx::device::IDirectXAccessor& accessor) {
  current_frame_resource_ =
      &frame_resources_[accessor.GetCurrentBackBufferIndex()];

  const UINT64 last_completed_fence = fence_->GetCompletedValue();

  if (current_frame_resource_->fence_value_ > last_completed_fence) {
    HANDLE event_handle = CreateEvent(nullptr, false, false, nullptr);
    if (!event_handle) {
      return;
    }
    if (Failed(fence_->SetEventOnCompletion(
            current_frame_resource_->fence_value_, event_handle))) {
      return;
    }
    WaitForSingleObject(event_handle, INFINITE);
    CloseHandle(event_handle);
  }
  current_frame_resource_->Ready();
}

void ParticleManager::Update() {
  for (auto&& particle : particles_) {
    particle->Update(current_frame_resource_->GetCommandList());
  }
  current_frame_resource_->GetCommandList().Close();
  ID3D12CommandList* command_lists[] = {
      current_frame_resource_->GetCommandList().GetCommandList()};
  command_queue_->ExecuteCommandLists(1, command_lists);

  const UINT64 fence_to_wait_for = fence_value_;
  if (Failed(command_queue_->Signal(fence_.Get(), fence_to_wait_for))) {
    return;
  }
  fence_value_++;
}

void ParticleManager::Render(directx::device::CommandList& command_list) {
  for (auto&& particle : particles_) {
    particle->Render(command_list);
  }
}

}  // namespace particle
}  // namespace draw
}  // namespace legend
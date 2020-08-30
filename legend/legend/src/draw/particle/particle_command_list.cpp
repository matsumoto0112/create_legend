#include "src/draw/particle/particle_command_list.h"

#include "src/directx/directx_helper.h"

namespace legend {
namespace draw {
namespace particle {

using directx::directx_helper::Failed;

//コンストラクタ
ParticleCommandList::ParticleCommandList() {}

//デストラクタ
ParticleCommandList::~ParticleCommandList() {}

//初期化
bool ParticleCommandList::Init(directx::device::IDirectXAccessor& accessor,
                               u32 frame_count) {
  //コンピュートシェーダー実行用キュー作成
  D3D12_COMMAND_QUEUE_DESC compute_queue_desc = {};
  compute_queue_desc.Flags =
      D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;
  compute_queue_desc.Type =
      D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_COMPUTE;
  if (Failed(accessor.GetDevice()->CreateCommandQueue(
          &compute_queue_desc, IID_PPV_ARGS(&command_queue_)))) {
    return false;
  }

  //フレームリソースの作成
  frame_resources_.resize(frame_count);
  for (u32 i = 0; i < frame_count; i++) {
    if (!frame_resources_[i].Init(
            accessor,
            D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_COMPUTE)) {
      return false;
    }
  }

  //同期用オブジェクト作成
  if (Failed(accessor.GetDevice()->CreateFence(
          fence_value_, D3D12_FENCE_FLAGS::D3D12_FENCE_FLAG_NONE,
          IID_PPV_ARGS(&fence_)))) {
    return false;
  }
  fence_value_++;

  return true;
}

void ParticleCommandList::UpdateParticles() {
  for (auto&& particle : particle_emitters_) {
    particle->Update(current_frame_resource_->GetCommandList());
  }

  particle_emitters_.erase(
      std::remove_if(particle_emitters_.begin(), particle_emitters_.end(),
                     [](auto&& p) { return p->StandDeleteFlag(); }),
      particle_emitters_.end());

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

void ParticleCommandList::RenderParticle(
    directx::device::CommandList& render_command_list) {
  for (auto&& particle : particle_emitters_) {
    particle->Render(render_command_list);
  }

  std::copy(add_particle_list_.begin(), add_particle_list_.end(),
            std::back_inserter(particle_emitters_));
  add_particle_list_.clear();
}

//フレーム開始時
void ParticleCommandList::BeginFrame(directx::device::DirectXDevice& device) {
  current_frame_resource_ =
      &frame_resources_[device.GetCurrentBackBufferIndex()];

  const UINT64 last_completed_fence = fence_->GetCompletedValue();

  //待機が必要なら待機する
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

  device.GetHeapManager().SetCommandList(
      current_frame_resource_->GetCommandList());
  current_frame_resource_->GetCommandList()
      .GetCommandList()
      ->SetComputeRootSignature(
          device.GetDefaultRootSignature()->GetRootSignature());
  device.GetHeapManager().SetHeapTableToComputeCommandList(
      device, current_frame_resource_->GetCommandList());
}

}  // namespace particle
}  // namespace draw
}  // namespace legend
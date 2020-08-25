#include "src/actor/actor_render_command_list.h"

#include "src/directx/shader/shader_register_id.h"
#include "src/game/game_device.h"
#include "src/util/resource/resource_names.h"

namespace legend {
namespace actor {
ActorRenderCommandList::ActorRenderCommandList() {}

ActorRenderCommandList::~ActorRenderCommandList() {}

void ActorRenderCommandList::Push(Actor* actor) {
  actor_list_.emplace_back(actor);
}

void ActorRenderCommandList::Clear() { actor_list_.clear(); }

void ActorRenderCommandList::ShadowPass() {
  auto& resource = game::GameDevice::GetInstance()->GetResource();
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& command_list = device.GetCurrentFrameResource()->GetCommandList();

  resource.GetPipeline()
      .Get(util::resource::resource_names::pipeline::SHADOW_MAP)
      ->SetCommandList(command_list);
  for (auto&& a : actor_list_) {
    a->transform_cb_.GetStagingRef().world = a->transform_.CreateWorldMatrix();
    a->transform_cb_.UpdateStaging();
    a->transform_cb_.RegisterHandle(
        device, directx::shader::ConstantBufferRegisterID::TRANSFORM);

    a->model_->Draw(command_list);
  }
}

void ActorRenderCommandList::RenderPass() {
  for (auto&& a : actor_list_) {
    a->Draw();
  }
}

}  // namespace actor
}  // namespace legend
#include "src/actor/actor.h"

#include "src/directx/shader/shader_register_id.h"
#include "src/game/game_device.h"
#include "src/util/resource/resource_names.h"

namespace legend {
namespace actor {

const wchar_t* Actor::PIPELINE_NAME =
    util::resource::resource_names::pipeline::DIFFERED_RENDERING_PRE;

Actor::Actor(const std::wstring& name) : name_(name) {}

Actor::~Actor() {}

bool Actor::Init(IActorMediator* mediator) {
  this->mediator_ = mediator;

  auto& device = game::GameDevice::GetInstance()->GetDevice();

  //トランスフォームバッファを作成する
  if (!transform_cb_.Init(
          device,
          device.GetLocalHandle(
              directx::descriptor_heap::heap_parameter::LocalHeapID::ONE_PLAY),
          name_ + L"_TransformConstantBuffer")) {
    return false;
  }

  return true;
}

void Actor::Draw() {
  MY_ASSERTION(model_.get(), L"モデルが存在しません。");

  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& resource = game::GameDevice::GetInstance()->GetResource();
  auto& command_list = device.GetCurrentFrameResource()->GetCommandList();
  resource.GetPipeline().Get(PIPELINE_NAME)->SetCommandList(command_list);

  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();
  transform_cb_.RegisterHandle(
      device, directx::shader::ConstantBufferRegisterID::TRANSFORM);

  model_->Draw(command_list);
}

}  // namespace actor
}  // namespace legend
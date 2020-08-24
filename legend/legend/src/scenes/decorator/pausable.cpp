#include "src/scenes/decorator/pausable.h"

#include "src/game/game_device.h"
#include "src/util/resource/resource_names.h"

namespace {
constexpr legend::directx::descriptor_heap::heap_parameter::LocalHeapID
    USE_HEAP_ID = legend::directx::descriptor_heap::heap_parameter::
        LocalHeapID::GLOBAL_ID;
}  // namespace

namespace legend {
namespace scenes {
namespace decorator {

Pausable::Pausable(ISceneChange* scene_change, std::unique_ptr<Scene> inner)
    : Scene(scene_change), inner_(std::move(inner)), is_pause_(false) {}

Pausable::~Pausable() {}

bool Pausable::Initialize() {
  auto& resource = game::GameDevice::GetInstance()->GetResource();
  if (!pause_image_.Init(resource.GetTexture().Get(
                             util::resource::resource_names::texture::TEX),
                         USE_HEAP_ID)) {
    return false;
  }

  return inner_->Initialize();
}

bool Pausable::Update() {
  auto& input = game::GameDevice::GetInstance()->GetInput();
  if (input.GetCommand(input::input_code::Pause)) {
    is_pause_ = !is_pause_;
  }

  if (is_pause_) {
    if (input.GetCommand(input::input_code::CAMERA_CHANGE)) {
      scene_change_->ChangeScene(SceneType::TITLE);
    }
    return true;
  }
  return inner_->Update();
}

void Pausable::Draw() {
  inner_->Draw();
  if (is_pause_) {
    auto& device = game::GameDevice::GetInstance()->GetDevice();
    auto& command_list = game::GameDevice::GetInstance()
                             ->GetDevice()
                             .GetCurrentFrameResource()
                             ->GetCommandList();
    device.GetRenderResourceManager().SetRenderTargets(
        command_list, directx::render_target::RenderTargetID::BACK_BUFFER,
        false, directx::render_target::DepthStencilTargetID::DEPTH_ONLY, false);
    auto& sprite = game::GameDevice::GetInstance()->GetSpriteRenderer();
    sprite.AddDrawItems(&pause_image_);
    sprite.DrawItems(command_list);
  }
}

void Pausable::Finalize() { inner_->Finalize(); }

}  // namespace decorator
}  // namespace scenes
}  // namespace legend
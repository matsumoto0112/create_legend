#include "src/scenes/decorator/fade_in_out.h"

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
FadeInOut::FadeInOut(ISceneChange* scene_change, std::unique_ptr<Scene> inner)
    : Scene(scene_change), inner_(std::move(inner)) {}

FadeInOut::~FadeInOut() {}

bool FadeInOut::Initialize() {
  auto& resource = game::GameDevice::GetInstance()->GetResource();
  if (!pause_image_.Init(resource.GetTexture().Get(
                             util::resource::resource_names::texture::TEX),
                         USE_HEAP_ID)) {
    return false;
  }

  return inner_->Initialize();
}

bool FadeInOut::Update() {
  // pause_image_.Alpha();
  return inner_->Update();
}

void FadeInOut::Draw() {
  inner_->Draw();
  //auto& device = game::GameDevice::GetInstance()->GetDevice();
  //auto& command_list = game::GameDevice::GetInstance()
  //                         ->GetDevice()
  //                         .GetCurrentFrameResource()
  //                         ->GetCommandList();
  //device.GetRenderResourceManager().SetRenderTargets(
  //    command_list, directx::render_target::RenderTargetID::BACK_BUFFER, false,
  //    directx::render_target::DepthStencilTargetID::DEPTH_ONLY, false);
  //auto& sprite = game::GameDevice::GetInstance()->GetSpriteRenderer();
  //sprite.AddDrawItems(&pause_image_);
  //sprite.DrawItems(command_list);
}

void FadeInOut::Finalize() { inner_->Finalize(); }

}  // namespace decorator
}  // namespace scenes
}  // namespace legend
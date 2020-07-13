#include "src/scenes/debugscene/sprite_render_test.h"

#include "src/game/game_device.h"
#include "src/util/path.h"

namespace legend {
namespace scenes {
namespace debugscene {

SpriteRenderTest::SpriteRenderTest(ISceneChange* scene_change)
    : Scene(scene_change) {}

SpriteRenderTest::~SpriteRenderTest() {}

bool SpriteRenderTest::Initialize() {
  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();

  if (!sprite_renderer_.Init(math::Vector2(1280.0f, 720.0f))) {
    return false;
  }

  const std::filesystem::path texture_path =
      util::Path::GetInstance()->texture() / "tex.png";
  if (!sprite_.Initialize(texture_path)) {
    return false;
  }

  return true;
}

bool SpriteRenderTest::Update() {
  // if (ImGui::Begin("Camera")) {
  //  static float camera_position[3];
  //  ImGui::SliderFloat3("position", camera_position, -100.0f, 100.0f);
  //  world_cb_.GetStagingRef().view = math::Matrix4x4::CreateTranslate(
  //      -1 * math::Vector3(camera_position[0], camera_position[1],
  //                         camera_position[2]));
  //  world_cb_.UpdateStaging();
  //}
  // ImGui::End();
  return true;
}

void SpriteRenderTest::Draw() {
  sprite_renderer_.AddDrawItems(&sprite_);
  sprite_renderer_.DrawItems();
}

void SpriteRenderTest::Finalize() {}

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend
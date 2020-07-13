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

  const std::filesystem::path texture_path =
      util::Path::GetInstance()->texture() / "tex.png";
  if (!sprite_.Init(texture_path)) {
    return false;
  }
  sprite_.SetScale(math::Vector2(0.2f, 0.2f));
  sprite_.SetZOrder(0.5f);

  return true;
}

bool SpriteRenderTest::Update() { return true; }

void SpriteRenderTest::Draw() {
  legend::draw::SpriteRenderer& sprite_renderer =
      game::GameDevice::GetInstance()->GetSpriteRenderer();

  sprite_renderer.AddDrawItems(&sprite_);
  sprite_renderer.DrawItems();
}

void SpriteRenderTest::Finalize() {}

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend
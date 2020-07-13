#include "src/scenes/debugscene/sprite_render_test.h"

#include "src/game/game_device.h"
#include "src/util/path.h"

namespace legend {
namespace scenes {
namespace debugscene {

SpriteRenderTest::SpriteRenderTest(ISceneChange* scene_change)
    : Scene(scene_change) {}

SpriteRenderTest::~SpriteRenderTest() {}

bool SpriteRenderTest::Initialize() { return true; }

bool SpriteRenderTest::Update() {
  if (ImGui::Begin("Sprite")) {
    ImGui::Text("Sprite Count: %d", sprites_.size());
    if (ImGui::Button("Add Sprite")) {
      const std::filesystem::path texture_path =
          util::Path::GetInstance()->texture() / "tex.png";
      draw::Sprite2D sprite;
      if (!sprite.Init(texture_path)) {
        return false;
      }
      const math::IntVector2 window_size =
          game::GameDevice::GetInstance()->GetWindow().GetScreenSize();
      util::Random& random = game::GameDevice::GetInstance()->GetRandom();
      const float x = random.Range(0.0f, static_cast<float>(window_size.x));
      const float y = random.Range(0.0f, static_cast<float>(window_size.y));
      sprite.SetPosition(math::Vector2(x, y));
      const float sx = random.Range(0.01f, 0.1f);
      const float sy = random.Range(0.01f, 0.1f);
      sprite.SetScale(math::Vector2(sx, sy));
      sprites_.push_back(sprite);
    }
  }
  ImGui::End();
  return true;
}

void SpriteRenderTest::Draw() {
  legend::draw::SpriteRenderer& sprite_renderer =
      game::GameDevice::GetInstance()->GetSpriteRenderer();

  for (auto&& sp : sprites_) {
    sprite_renderer.AddDrawItems(&sp);
  }
  sprite_renderer.DrawItems();
}

void SpriteRenderTest::Finalize() {}

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend
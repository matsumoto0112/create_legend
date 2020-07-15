﻿#include "src/scenes/debugscene/sprite_render_test.h"

#include "src/draw/texture_string.h"
#include "src/game/game_device.h"
#include "src/util/path.h"

namespace {
legend::draw::TextureString string_;
// const std::wstring text =
//    L"寿限無寿限無五劫の擦り切れ海砂利水魚の水行末雲来末風来末喰う寝る処に住"
//    L"む処藪ら柑子の藪柑子パイポ・パイポ・パイポのシューリンガンシューリンガン"
//    L"のグーリンダイグーリンダイのポンポコピーのポンポコナの長久命の長助";
}  // namespace

namespace legend {
namespace scenes {
namespace debugscene {

SpriteRenderTest::SpriteRenderTest(ISceneChange* scene_change)
    : Scene(scene_change) {}

SpriteRenderTest::~SpriteRenderTest() {}

bool SpriteRenderTest::Initialize() {
  // if (!string_.Init(text)) {
  //  return false;
  //}
  return true;
}

static int a = 0;

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
    static float pos[2] = {0.0f, 0.0f};
    ImGui::SliderFloat2("String Position", pos, -2000.0f, 2000.0f);
    string_.SetPosition(math::Vector2(pos[0], pos[1]));
    static float scale[2] = {1.0f, 1.0f};
    ImGui::SliderFloat2("String Scale", scale, 0.0f, 20.0f);
    string_.SetScale(math::Vector2(scale[0], scale[1]));

    static char buf[16];
    ImGui::InputText("Append String", buf, _countof(buf));
    if (ImGui::Button("Append")) {
      if (!string_.Append(util::string_util::String_2_WString(buf))) {
        return false;
      }
      ZeroMemory(buf, _countof(buf));
    }
  }
  ImGui::End();
  return true;
}

void SpriteRenderTest::Draw() {
  legend::draw::SpriteRenderer& sprite_renderer =
      game::GameDevice::GetInstance()->GetSpriteRenderer();

  string_.Draw(sprite_renderer);
  for (auto&& sp : sprites_) {
    sprite_renderer.AddDrawItems(&sp);
  }
  sprite_renderer.DrawItems();
}

void SpriteRenderTest::Finalize() {}

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend
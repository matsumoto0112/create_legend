﻿#include "src/scenes/debugscene/sprite_render_test.h"

#include "src/directx/shader/shader_register_id.h"
#include "src/game/game_device.h"
#include "src/util/loader/font_loader.h"
#include "src/util/path.h"
#include "src/util/resource/resource_names.h"

namespace legend {
namespace scenes {
namespace debugscene {

SpriteRenderTest::SpriteRenderTest(ISceneChange* scene_change)
    : Scene(scene_change) {}

SpriteRenderTest::~SpriteRenderTest() {}

bool SpriteRenderTest::Initialize() {
  const std::filesystem::path font =
      util::Path::GetInstance()->exe() / "assets" / "fonts" / "みかちゃん.ttf";
  const std::wstring name = util::loader::FontLoader::GetInstance()->Load(font);
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  directx::device::CommandList command_list;
  if (!command_list.Init(
          device, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT)) {
    return false;
  }
  if (!string_.Init(command_list, L"HOGE",
                    legend::directx::descriptor_heap::heap_parameter::
                        LocalHeapID::GLOBAL_ID,
                    name, 128)) {
    return false;
  }

  if (!command_list.Close()) {
    return false;
  }
  device.ExecuteCommandList({command_list});
  device.WaitExecute();
  return true;
}

bool SpriteRenderTest::Update() {
  if (ImGui::Begin("Sprite")) {
    ImGui::Text("Sprite Count: %d", sprites_.size());

    //ボタンを押すとスプライトを追加する
    if (ImGui::Button("Add Sprite")) {
      draw::Sprite2D sprite;
      if (!sprite.Init(
              game::GameDevice::GetInstance()->GetResource().GetTexture().Get(
                  util::resource::resource_names::texture::TEX),
              directx::descriptor_heap::heap_parameter::LocalHeapID::
                  GLOBAL_ID)) {
        return false;
      }

      //ウィンドウ内にランダムなポジションをとる
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

    //スプライトのUV
    static float uv_left = 0.0f;
    ImGui::SliderFloat("UV_Left", &uv_left, -5.0f, 5.0f);
    static float uv_top = 0.0f;
    ImGui::SliderFloat("UV_Top", &uv_top, -5.0f, 5.0f);
    static float uv_width = 1.0f;
    ImGui::SliderFloat("UV_Width", &uv_width, -5.0f, 5.0f);
    static float uv_height = 1.0f;
    ImGui::SliderFloat("UV_Height", &uv_height, -5.0f, 5.0f);
    for (auto&& sp : sprites_) {
      sp.SetRect(math::Rect(uv_left, uv_top, uv_width, uv_height));
    }

    //文字列の座標・スケーリング調整スライダー
    static float pos[2] = {0.0f, 0.0f};
    ImGui::SliderFloat2("String Position", pos, -2000.0f, 2000.0f);
    string_.SetPosition(math::Vector2(pos[0], pos[1]));
    static float scale[2] = {1.0f, 1.0f};
    ImGui::SliderFloat2("String Scale", scale, 0.0f, 20.0f);
    string_.SetScale(math::Vector2(scale[0], scale[1]));

    //表示する文字列
    static char buf[1024] = u8"友情・努力・勝利";
    ImGui::InputText("Append String", buf, _countof(buf));
    if (ImGui::Button("Append")) {
      auto& device = game::GameDevice::GetInstance()->GetDevice();
      directx::device::CommandList command_list;
      if (!command_list.Init(
              device,
              D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT)) {
        return false;
      }
      if (!string_.Append(command_list,
                          util::string_util::UTF_8_2_WString(buf))) {
        return false;
      }
      if (!command_list.Close()) {
        return false;
      }
      device.ExecuteCommandList({command_list});
      device.WaitExecute();
      ZeroMemory(buf, _countof(buf));
    }
    if (ImGui::Button("Clear")) {
      string_.Clear();
    }
  }
  ImGui::End();
  return true;
}

void SpriteRenderTest::Draw() {
  legend::draw::SpriteRenderer& sprite_renderer =
      game::GameDevice::GetInstance()->GetSpriteRenderer();

  string_.StackDrawList(sprite_renderer);
  for (auto&& sp : sprites_) {
    sprite_renderer.AddDrawItems(&sp);
  }
  sprite_renderer.DrawItems(game::GameDevice::GetInstance()
                                ->GetDevice()
                                .GetCurrentFrameResource()
                                ->GetCommandList());
}

void SpriteRenderTest::Finalize() {
  game::GameDevice::GetInstance()->GetDevice().WaitExecute();
}

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend
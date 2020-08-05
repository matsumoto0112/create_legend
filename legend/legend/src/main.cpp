#include "src/directx/descriptor_heap/heap_parameter.h"
#include "src/game/application.h"
#include "src/game/game_device.h"
#include "src/scenes/scene_manager.h"
#include "src/scenes/scene_names.h"
#include "src/window/window.h"

namespace {
using legend::directx::descriptor_heap::heap_parameter::LocalHeapID;
static const LocalHeapID USE_HEAP_IDS[] = {
    LocalHeapID::MODEL_VIEW_SCENE,
    LocalHeapID::MULTI_RENDER_TARGET_TEST_SCENE,
    LocalHeapID::SPRITE_RENDER_TEST,
    LocalHeapID::PHYSICS_TEST,
    LocalHeapID::ENEMY_MOVE_TEST,
    LocalHeapID::PLAYER_MOVE_VIEWER,
    LocalHeapID::MAIN_SCENE_1,
};
}  // namespace

namespace legend {
class MyApp final : public device::Application {
 public:
  MyApp() : Application(), scene_manager_() {}
  ~MyApp() {}
  bool Init() override {
    if (!Application::Init()) {
      return false;
    }

    directx::DirectX12Device& device =
        game::GameDevice::GetInstance()->GetDevice();
    //使用するローカルヒープを追加する
    directx::descriptor_heap::HeapManager& heap_manager =
        device.GetHeapManager();

    for (auto&& id : USE_HEAP_IDS) {
      if (!heap_manager.AddLocalHeap(device, id)) {
        return false;
      }
    }

    const math::IntVector2 screen_size =
        game::GameDevice::GetInstance()->GetWindow().GetScreenSize();
    if (!device.GetRenderResourceManager().CreateDepthStencil(
            device, directx::render_target::DepthStencilTargetID::Depth,
            DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT, screen_size.x, screen_size.y,
            1.0f, 0, L"DepthStencil")) {
      return false;
    }

    if (!scene_manager_.Initialize()) {
      return false;
    }

    return true;
  }
  bool Update() override {
    if (!Application::Update()) {
      return false;
    }
    if (!scene_manager_.Update()) {
      return false;
    }

    if (ImGui::Begin("Scenes")) {
      ImGui::Text(("CurrentScene: " + scenes::scene_names::Get(
                                          scene_manager_.GetCurrentSceneType()))
                      .c_str());

      constexpr scenes::SceneType SCENES[] = {
          scenes::SceneType::TITLE,
          scenes::SceneType::GAMEOVER,
          scenes::SceneType::MODEL_VIEW,
          scenes::SceneType::SOUND_TEST,
          scenes::SceneType::PHYSICS_TEST,
          scenes::SceneType::SPRITE_TEST,
          scenes::SceneType::MULTI_RENDER_TARGET_TEST,
          scenes::SceneType::ENEMY_MOVE_VIEWER,
          scenes::SceneType::PLAYER_MOVE_VIEWER,
          scenes::SceneType::MAIN_SCENE_1,
          scenes::SceneType::STAGE_GENERATE_TEST,
      };
      for (auto&& scene : SCENES) {
        if (ImGui::Button(scenes::scene_names::Get(scene).c_str())) {
          scene_manager_.ChangeScene(scene);
        }
      }
    }
    ImGui::End();
    if (ImGui::Begin("Debug")) {
      ImGui::Text(
          "TotalTime: %f",
          game::GameDevice::GetInstance()->GetFPSCounter().GetTotalSeconds());
      ImGui::Text("FrameRate: %.1f",
                  game::GameDevice::GetInstance()->GetFPSCounter().GetFPS());
    }
    ImGui::End();
    return true;
  }

  bool Draw() override {
    if (!Application::Draw()) {
      return false;
    }

    scene_manager_.Draw();
    return true;
  }
  void Finalize() override {
    Application::Finalize();
    scene_manager_.Finalize();
  }

 private:
  scenes::SceneManager scene_manager_;
};
}  // namespace legend

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

  auto window = std::make_unique<legend::window::Window>();
  window->SetScreenSize(legend::math::IntVector2(1280, 720));
  window->SetWindowPosition(legend::math::IntVector2(0, 0));
  window->SetWindowTitle(L"Legend");

  legend::MyApp app;
  app.RegisterWindow(std::move(window));

  app.Run();

  return 0;
}
#include "src/game/application.h"
#include "src/game/game_device.h"
#include "src/scenes/scene_manager.h"
#include "src/scenes/scene_names.h"
#include "src/window/window.h"

namespace legend {
class MyApp final : public device::Application {
 public:
  MyApp() : Application(), scene_manager_() {}
  ~MyApp() {}
  bool Init() override {
    if (!Application::Init()) {
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
    timer_.Update();

    if (ImGui::Begin("Scenes")) {
      ImGui::Text(("CurrentScene: " + scenes::scene_names::Get(
                                          scene_manager_.GetCurrentSceneType()))
                      .c_str());

      constexpr scenes::SceneType SCENES[] = {
          scenes::SceneType::TITLE,
          scenes::SceneType::GAMEOVER,
          scenes::SceneType::MODEL_VIEW,
          scenes::SceneType::SOUND_TEST,
          scenes::SceneType::PERSPECTIVE_CAMERA_TEST,
          scenes::SceneType::PHYSICS_TEST,
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
  util::FPSCounter timer_;
};
}  // namespace legend

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

  std::shared_ptr<legend::window::Window> window =
      std::make_shared<legend::window::Window>();
  window->SetScreenSize(legend::math::IntVector2(1280, 720));
  window->SetWindowPosition(legend::math::IntVector2(0, 0));
  window->SetWindowTitle(L"Legend");

  legend::MyApp app;
  app.RegisterWindow(window);

  app.Run();

  return 0;
}
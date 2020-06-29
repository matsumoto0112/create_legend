#include "src/game/application.h"
#include "src/scenes/scene_manager.h"
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

    scene_manager_.Initialize();
    return true;
  }
  bool Update() override {
    if (!Application::Update()) {
      return false;
    }
    scene_manager_.Update();

    if (ImGui::Begin("Scenes")) {
      ImGui::Text("Current Scene:");
      if (ImGui::Button("Title")) {
        scene_manager_.ChangeScene(scenes::SceneType::TITLE);
      }
      if (ImGui::Button("GameOver")) {
        scene_manager_.ChangeScene(scenes::SceneType::GAMEOVER);
      }
      if (ImGui::Button("ModelView")) {
        scene_manager_.ChangeScene(scenes::SceneType::MODEL_VIEW);
      }

      ImGui::End();
    }
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
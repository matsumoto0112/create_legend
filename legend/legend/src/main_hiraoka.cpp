//#include "src/game/application.h"
//#include "src/scenes/scene.h"
//#include "src/scenes/scene_manager.h"
//#include "src/window/window.h"
//
//namespace legend {
//
//class MyApp final : public device::Application {
// public:
//  MyApp() : Application() {}
//  ~MyApp() {}
//  bool Init() override {
//    if (!Application::Init()) {
//      return false;
//    }
//
//    MY_LOG(L"init_myapp");
//
//    scene_manager_.Initialize();
//
//    return true;
//  }
//  bool Update() override {
//    if (!Application::Update()) {
//      return false;
//    }
//
//    scene_manager_.Update();
//
//    MY_LOG(L"update_myapp");
//    return true;
//  }
//
//  bool Draw() override {
//    if (!Application::Draw()) {
//      return false;
//    }
//
//    scene_manager_.Draw();
//    MY_LOG(L"draw_myapp");
//
//    return true;
//  }
//  void Finalize() override {
//    Application::Finalize();
//    MY_LOG(L"finalize_myapp");
//  }
//
// private:
//  scenes::SceneManager scene_manager_;
//};
//}  // namespace legend
//
//int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
//  std::shared_ptr<legend::window::Window> window =
//      std::make_shared<legend::window::Window>();
//  window->SetScreenSize(legend::math::IntVector2(1280, 720));
//  window->SetWindowPosition(legend::math::IntVector2(0, 0));
//  window->SetWindowTitle(L"Legend");
//
//  legend::MyApp app;
//  app.RegisterWindow(window);
//
//  app.Run();
//
//  return 0;
//}
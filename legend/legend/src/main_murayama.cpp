//#include "src/audio/audio_manager.h"
//#include "src/game/application.h"
//#include "src/scenes/scene_manager.h"
//#include "src/window/window.h"
//
//namespace legend {
//class MyApp final : public device::Application {
// public:
//  MyApp() : Application(), scene_manager_() {}
//  ~MyApp() {}
//  bool Init() override {
//    if (!Application::Init()) {
//      return false;
//    }
//    audio_manager_.Init();
//    // audio_manager_.LoadWav(L"../legend/assets/audios/free_3.wav");
//    // audio_manager_.LoadWav(L"../legend/assets/audios/free_2.wav");
//    i64 se1 = audio_manager_.Play(L"../legend/assets/audios/free_3.wav");
//    // audio_manager_.Play(L"../legend/assets/audios/free_3.wav", -1);
//    i64 se2 = audio_manager_.Play(L"../legend/assets/audios/free_2.wav");
//    // audio_manager_.Play(L"../legend/assets/audios/free_2.wav", 1);
//    // audio_manager_.Play(L"../legend/assets/audios/free_2.wav", -1);
//    // audio_manager_.Pause(se1);
//    // audio_manager_.Pause(se2);
//    // audio_manager_.Play(se1);
//    // audio_manager_.Play(se2);
//
//    scene_manager_.Initialize();
//    return true;
//  }
//  bool Update() override {
//    if (!Application::Update()) {
//      return false;
//    }
//    audio_manager_.Update();
//    scene_manager_.Update();
//    return true;
//  }
//
//  bool Draw() override {
//    if (!Application::Draw()) {
//      return false;
//    }
//
//    scene_manager_.Draw();
//    return true;
//  }
//  void Finalize() override {
//    Application::Finalize();
//    scene_manager_.Finalize();
//  }
//
// private:
//  scenes::SceneManager scene_manager_;
//  audio::AudioManager audio_manager_;
//};
//}  // namespace legend
//
//int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
//  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//
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
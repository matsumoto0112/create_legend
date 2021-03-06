#include <btBulletDynamicsCommon.h>

#include "src/directx/shader/alpha_blend_desc.h"
#include "src/directx/shader/graphics_pipeline_state_desc.h"
#include "src/game/application.h"
#include "src/game/game_device.h"
#include "src/scenes/scene_manager.h"
#include "src/scenes/scene_names.h"
#include "src/util/pipeline_initializer.h"
#include "src/util/resource/resource_names.h"

namespace legend {
class MyApp final : public device::Application {
 public:
  MyApp() : Application() {}
  ~MyApp() {}
  bool Init() override {
    if (!Application::Init()) {
      return false;
    }

    {
      auto& device = game::GameDevice::GetInstance()->GetDevice();
      auto& render_resource_manager = device.GetRenderResourceManager();
      const math::IntVector2 screen_size =
          game::GameDevice::GetInstance()->GetWindow().GetScreenSize();
      const u32 x = static_cast<u32>(screen_size.x);
      const u32 y = static_cast<u32>(screen_size.y);

      const util::Color4 back_color = util::Color4(0.2f, 0.2f, 0.2f, 1.0f);
      std::vector<directx::render_target::MultiRenderTargetTexture::Info> infos{
          {0, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, x, y, back_color,
           L"DIFFERED_RENDERING_PRE_1"},
          {1, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, x, y, back_color,
           L"DIFFERED_RENDERING_PRE_2"},
          {2, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, x, y, back_color,
           L"DIFFERED_RENDERING_PRE_3"},
      };
      if (!render_resource_manager.AddRenderTarget(
              directx::render_target::RenderTargetID::DIFFERED_RENDERING_PRE,
              device, infos)) {
        return false;
      }
    }

    {
      auto& device = game::GameDevice::GetInstance()->GetDevice();
      auto& render_resource_manager = device.GetRenderResourceManager();
      const math::IntVector2 screen_size =
          game::GameDevice::GetInstance()->GetWindow().GetScreenSize();
      const u32 x = static_cast<u32>(screen_size.x);
      const u32 y = static_cast<u32>(screen_size.y);
      const directx::render_target::DepthStencil::DepthStencilDesc desc{
          L"ShadowDepth", DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT, x, y, 1.0f, 0};
      if (!render_resource_manager.AddDepthStencil(
              directx::render_target::DepthStencilTargetID::SHADOW_MAP, device,
              desc)) {
        return false;
      }
    }

    if (!util::PipelineInitializer::Init(std::filesystem::path("parameters") /
                                         "pipeline.txt")) {
      return false;
    }

    //使用する音源を事前に読み込む
    // audioデータは大容量のため、すべてを読み込むのは本来NGだが、今回のappで使用する音源の量がそれほど大きくないため、一括で読み込んでおく。
    auto AudioPreLoad = [&]() {
      enum AudioLisFormat {
        NAME,
        TYPE,
        SPLIT_FLAG,
      };

      const std::filesystem::path root_path =
          util::Path::GetInstance()->exe() / "assets" / "audios";
      auto& audio_manager = game::GameDevice::GetInstance()->GetAudioManager();
      const std::vector<u8> data =
          game::GameDevice::GetInstance()
              ->GetResource()
              .GetArchiveLoader()
              .Load(std::filesystem::path("parameters") / "audio_list.txt");
      std::stringstream ss(std::string(data.begin(), data.end()));
      std::string line;
      while (std::getline(ss, line)) {
        if (line.back() == '\r') line = line.substr(0, line.size() - 1);
        const auto splited = util::string_util::StringSplit(line, ',');
        const std::wstring audio_name =
            util::string_util::String_2_WString(splited[NAME]);
        const AudioType type = static_cast<AudioType>(std::stoi(splited[TYPE]));
        const AudioSplitType split_flag =
            static_cast<AudioSplitType>(std::stoi(splited[SPLIT_FLAG]));
        if (!audio_manager.LoadWav(audio_name, type, split_flag)) {
          MY_LOG(L"読み込みに失敗: %s", audio_name.c_str());
          return false;
        }
      }
      return true;
    };
    if (!AudioPreLoad()) {
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

    if (ImGui::Begin("Debug")) {
      auto& fps = game::GameDevice::GetInstance()->GetFPSCounter();
      ImGui::Text("TotalTime: %f", fps.GetTotalSeconds());
      ImGui::Text("FrameRate: %.1f", fps.GetFPS());
    }
    ImGui::End();

    return true;
  }

  bool Render() override {
    if (!Application::Render()) {
      return false;
    }

    scene_manager_.Draw();
    return true;
  }
  void Destroy() override {
    scene_manager_.Finalize();
    Application::Destroy();
  }

 private:
  scenes::SceneManager scene_manager_;
};
}  // namespace legend

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

  legend::MyApp app;

  try {
    app.Run();
  } catch (const std::exception& e) {
    OutputDebugStringA(e.what());
  }

  return 0;
}
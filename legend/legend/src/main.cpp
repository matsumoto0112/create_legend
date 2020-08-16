#include "src/directx/shader/alpha_blend_desc.h"
#include "src/directx/shader/graphics_pipeline_state_desc.h"
#include "src/game/application.h"
#include "src/game/game_device.h"
#include "src/scenes/scene_manager.h"
#include "src/scenes/scene_names.h"
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

    auto& device = game::GameDevice::GetInstance()->GetDevice();
    auto& resource = game::GameDevice::GetInstance()->GetResource();

    //パイプラインの登録
    //パイプラインは外部ファイルに書き出してそれを読み取る形式にしたい
    {
      directx::shader::GraphicsPipelineStateDesc pso_desc = {};
      pso_desc.SetRenderTargets(
          device.GetRenderResourceManager().GetRenderTarget(
              directx::render_target::RenderTargetID::BACK_BUFFER));
      pso_desc.SetVertexShader(
          resource.GetVertexShader()
              .Get(util::resource::resource_names::vertex_shader::MODEL_VIEW)
              .get());
      pso_desc.SetPixelShader(
          resource.GetPixelShader()
              .Get(util::resource::resource_names::pixel_shader::MODEL_VIEW)
              .get());
      pso_desc.SetDepthStencilTarget(
          device.GetRenderResourceManager().GetDepthStencilTarget(
              directx::render_target::DepthStencilTargetID::DEPTH_ONLY));
      pso_desc.SetRootSignature(device.GetDefaultRootSignature());

      pso_desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
      pso_desc.PrimitiveTopologyType =
          D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
      pso_desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
      pso_desc.SampleDesc.Count = 1;
      pso_desc.SampleMask = UINT_MAX;
      auto pipeline = std::make_shared<directx::shader::PipelineState>();
      if (!pipeline->Init(device, pso_desc)) {
        return false;
      }
      resource.GetPipeline().Register(
          util::resource::resource_names::pipeline::MODEL_VIEW, pipeline);

      auto pipeline_graffiti =
          std::make_shared<directx::shader::PipelineState>();
      pso_desc.SetVertexShader(
          resource.GetVertexShader()
              .Get(util::resource::resource_names::vertex_shader::GRAFFITI)
              .get());
      pso_desc.SetPixelShader(
          resource.GetPixelShader()
              .Get(util::resource::resource_names::pixel_shader::GRAFFITI)
              .get());
      pso_desc.BlendState.AlphaToCoverageEnable = false;
      pso_desc.BlendState.RenderTarget[0] =
          directx::shader::alpha_blend_desc::BLEND_DESC_ALIGNMENT;
      if (!pipeline_graffiti->Init(device, pso_desc)) {
        return false;
      }
      resource.GetPipeline().Register(
          util::resource::resource_names::pipeline::GRAFFITI,
          pipeline_graffiti);
    }
    {
      auto pipeline = std::make_shared<directx::shader::PipelineState>();
      directx::shader::PipelineState::GraphicsPipelineStateDesc pso_desc = {};
      auto vs = resource.GetVertexShader().Get(
          util::resource::resource_names::vertex_shader::OBB);
      auto ps = resource.GetPixelShader().Get(
          util::resource::resource_names::pixel_shader::OBB);
      pso_desc.BlendState.RenderTarget[0] =
          directx::shader::alpha_blend_desc::BLEND_DESC_ALIGNMENT;
      pso_desc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
      pso_desc.DepthStencilState.DepthEnable = false;
      pso_desc.DSVFormat = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
      pso_desc.InputLayout = vs->GetInputLayout();
      pso_desc.NumRenderTargets = 1;
      pso_desc.RTVFormats[0] = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
      pso_desc.PrimitiveTopologyType =
          D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
      pso_desc.pRootSignature =
          device.GetDefaultRootSignature()->GetRootSignature();
      pso_desc.PS = ps->GetShaderBytecode();
      pso_desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
      pso_desc.SampleDesc.Count = 1;
      pso_desc.SampleMask = UINT_MAX;
      pso_desc.VS = vs->GetShaderBytecode();
      if (!pipeline->Init(device, pso_desc)) {
        return false;
      }
      resource.GetPipeline().Register(
          util::resource::resource_names::pipeline::OBB, pipeline);
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
          scenes::SceneType::ENEMY_MOVE_VIEWER,
          scenes::SceneType::PLAYER_MOVE_VIEWER,
          scenes::SceneType::MAIN_SCENE_1,
          scenes::SceneType::STAGE_GENERATE_TEST,
          scenes::SceneType::GRAFFITI_TEST,
          scenes::SceneType::GPU_PARTICLE_TEST,
          scenes::SceneType::SKILL_TEST,
      };
      for (auto&& scene : SCENES) {
        if (ImGui::Button(scenes::scene_names::Get(scene).c_str())) {
          scene_manager_.ChangeScene(scene);
        }
      }
    }
    ImGui::End();

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
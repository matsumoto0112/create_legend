#include "src/directx/shader/alpha_blend_desc.h"
#include "src/directx/shader/graphics_pipeline_state_desc.h"
#include "src/directx/shader/shader_register_id.h"
#include "src/game/application.h"
#include "src/game/game_device.h"
#include "src/scenes/scene_manager.h"
#include "src/scenes/scene_names.h"

namespace {
using std::filesystem::path;
//よく使うシェーダーやパイプライン、モデルなどを事前に読み込むためのリスト
struct VertexShader {
  using ID = legend::util::resource::id::VertexShader;
  ID id;
  path filepath;
};
const VertexShader VS_LIST[] = {
    {VertexShader::ID::MODEL_VIEW, path("modelview") / "model_view_vs.cso"},
    {VertexShader::ID::GRAFFITI, path("graffiti") / "graffiti_vs.cso"},
    {VertexShader::ID::OBB, path("physics") / "obb_vs.cso"},
};

struct PixelShader {
  using ID = legend::util::resource::id::PixelShader;
  ID id;
  path filepath;
};
const PixelShader PS_LIST[] = {
    {PixelShader::ID::MODEL_VIEW, path("modelview") / "model_view_ps.cso"},
    {PixelShader::ID::GRAFFITI, path("graffiti") / "graffiti_ps.cso"},
    {PixelShader::ID::OBB, path("physics") / "obb_ps.cso"},
};

struct Model {
  using ID = legend::util::resource::id::Model;
  ID id;
  path filepath;
};
const Model MODEL_LIST[] = {
    {Model::ID::CHECK_XYZ, path("checkXYZ.glb")},
    {Model::ID::DESK, path("desk.glb")},
    {Model::ID::ERASER, path("eraser_01.glb")},
    {Model::ID::KARI, path("kari.glb")},
    {Model::ID::OBJECT_1000CM, path("1000cmObject.glb")},
    {Model::ID::PLANE, path("plane.glb")},
};

struct Texture {
  using ID = legend::util::resource::id::Texture;
  ID id;
  path filepath;
};
const Texture TEXTURE_LIST[] = {{Texture::ID::TEX, path("tex.png")}};
}  // namespace

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

    {
      const math::IntVector2 screen_size =
          game::GameDevice::GetInstance()->GetWindow().GetScreenSize();
      const directx::render_target::DepthStencil::DepthStencilDesc desc = {
          L"DepthOnly",
          DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT,
          static_cast<u32>(screen_size.x),
          static_cast<u32>(screen_size.y),
          1.0f,
          0};
      if (!device.GetRenderResourceManager().AddDepthStencil(
              directx::render_target::DepthStencilTargetID::DEPTH_ONLY, device,
              desc)) {
        return false;
      }
    }

    directx::device::CommandList command_list;
    if (!command_list.Init(
            device, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT)) {
      return false;
    }

    auto& resource = game::GameDevice::GetInstance()->GetResource();
    {
      const path shader_path = util::Path::GetInstance()->shader();
      for (auto&& vs : VS_LIST) {
        if (!resource.GetVertexShader().Load(vs.id, shader_path / vs.filepath))
          return false;
      }
      for (auto&& ps : PS_LIST) {
        if (!resource.GetPixelShader().Load(ps.id, shader_path / ps.filepath))
          return false;
      }
      const path model_path = util::Path::GetInstance()->model();
      for (auto&& model : MODEL_LIST) {
        if (!resource.GetModel().Load(model.id, model_path / model.filepath,
                                      command_list))
          return false;
      }
      const path texture_path = util::Path::GetInstance()->texture();
      for (auto&& tex : TEXTURE_LIST) {
        if (!resource.GetTexture().Load(
                command_list, tex.id, texture_path / tex.filepath,
                directx::shader::TextureRegisterID::ALBEDO,
                directx::descriptor_heap::heap_parameter::LocalHeapID::
                    GLOBAL_ID)) {
          return false;
        }
      }

      //パイプラインの登録
      //パイプラインは外部ファイルに書き出してそれを読み取る形式にしたい
      {
        directx::shader::GraphicsPipelineStateDesc pso_desc = {};
        pso_desc.SetRenderTargets(
            device.GetRenderResourceManager().GetRenderTarget(
                directx::render_target::RenderTargetID::BACK_BUFFER));
        pso_desc.SetVertexShader(
            resource.GetVertexShader().Get(VertexShader::ID::MODEL_VIEW).get());
        pso_desc.SetPixelShader(
            resource.GetPixelShader().Get(PixelShader::ID::MODEL_VIEW).get());
        pso_desc.SetDepthStencilTarget(
            device.GetRenderResourceManager().GetDepthStencilTarget(
                directx::render_target::DepthStencilTargetID::DEPTH_ONLY));
        pso_desc.SetRootSignature(device.GetDefaultRootSignature());

        pso_desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
        pso_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE::
            D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        pso_desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
        pso_desc.SampleDesc.Count = 1;
        pso_desc.SampleMask = UINT_MAX;
        auto pipeline = std::make_shared<directx::shader::PipelineState>();
        if (!pipeline->Init(device, pso_desc)) {
          return false;
        }
        resource.GetPipeline().Register(
            util::resource::id::Pipeline::MODEL_VIEW, pipeline);
        auto pipeline_graffiti =
            std::make_shared<directx::shader::PipelineState>();
        pso_desc.SetVertexShader(
            resource.GetVertexShader().Get(VertexShader::ID::GRAFFITI).get());
        pso_desc.SetPixelShader(
            resource.GetPixelShader().Get(PixelShader::ID::GRAFFITI).get());
        pso_desc.BlendState.AlphaToCoverageEnable = false;
        pso_desc.BlendState.RenderTarget[0] =
            directx::shader::alpha_blend_desc::BLEND_DESC_ALIGNMENT;
        if (!pipeline_graffiti->Init(device, pso_desc)) {
          return false;
        }
        resource.GetPipeline().Register(util::resource::id::Pipeline::GRAFFITI,
                                        pipeline_graffiti);
        pso_desc.RasterizerState.FillMode =
            D3D12_FILL_MODE::D3D12_FILL_MODE_WIREFRAME;
        auto pipeline_wireframe =
            std::make_shared<directx::shader::PipelineState>();
        if (!pipeline_wireframe->Init(device, pso_desc)) {
          return false;
        }
        resource.GetPipeline().Register(
            util::resource::id::Pipeline::OBJECT_WIREFRAME, pipeline_wireframe);
      }
      {
        auto pipeline = std::make_shared<directx::shader::PipelineState>();
        directx::shader::PipelineState::GraphicsPipelineStateDesc pso_desc = {};
        auto vs = resource.GetVertexShader().Get(VertexShader::ID::OBB);
        auto ps = resource.GetPixelShader().Get(PixelShader::ID::OBB);
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
            util::resource::id::Pipeline::PRIMITIVE_LINE, pipeline);
      }
    }

    command_list.Close();
    device.ExecuteCommandList({command_list});
    device.WaitExecute();
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
          scenes::SceneType::GRAFFITI_TEST,
          scenes::SceneType::GPU_PARTICLE_TEST,
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
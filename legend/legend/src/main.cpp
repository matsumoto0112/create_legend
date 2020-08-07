#include "src/game/application.h"
#include "src/game/game_device.h"
#include "src/scenes/scene_manager.h"
#include "src/scenes/scene_names.h"

namespace {
using std::filesystem::path;
//�悭�g���V�F�[�_�[��p�C�v���C���A���f���Ȃǂ����O�ɓǂݍ��ނ��߂̃��X�g
struct VertexShader {
  using ID = legend::util::resource::id::VertexShader;
  ID id;
  path filepath;
};
const VertexShader VS_LIST[] = {
    {VertexShader::ID::MODEL_VIEW, path("modelview") / "model_view_vs.cso"},
};

struct PixelShader {
  using ID = legend::util::resource::id::PixelShader;
  ID id;
  path filepath;
};
const PixelShader PS_LIST[] = {
    {PixelShader::ID::MODEL_VIEW, path("modelview") / "model_view_ps.cso"},
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
};
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

      //�p�C�v���C���̓o�^
      //�p�C�v���C���͊O���t�@�C���ɏ����o���Ă����ǂݎ��`���ɂ�����
      {
        D3D12_GRAPHICS_PIPELINE_STATE_DESC pso_desc = {};
        pso_desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
        pso_desc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC1(D3D12_DEFAULT);
        pso_desc.DSVFormat = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
        pso_desc.InputLayout = resource.GetVertexShader()
                                   .Get(VertexShader::ID::MODEL_VIEW)
                                   ->GetInputLayout();
        pso_desc.NumRenderTargets = 1;
        pso_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE::
            D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        pso_desc.pRootSignature =
            device.GetDefaultRootSignature()->GetRootSignature();
        pso_desc.PS = resource.GetPixelShader()
                          .Get(PixelShader::ID::MODEL_VIEW)
                          ->GetShaderBytecode();
        pso_desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
        pso_desc.RTVFormats[0] = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
        pso_desc.SampleDesc.Count = 1;
        pso_desc.SampleMask = UINT_MAX;
        pso_desc.VS = resource.GetVertexShader()
                          .Get(VertexShader::ID::MODEL_VIEW)
                          ->GetShaderBytecode();
        auto pipeline =
            std::make_shared<directx::shader::GraphicsPipelineState>();
        if (!pipeline->Init(device, pso_desc)) {
          return false;
        }
        resource.GetPipeline().Register(
            util::resource::id::Pipeline::MODEL_VIEW, pipeline);

        pso_desc.RasterizerState.FillMode =
            D3D12_FILL_MODE::D3D12_FILL_MODE_WIREFRAME;
        auto pipeline_wireframe =
            std::make_shared<directx::shader::GraphicsPipelineState>();
        if (!pipeline_wireframe->Init(device, pso_desc)) {
          return false;
        }
        resource.GetPipeline().Register(
            util::resource::id::Pipeline::OBJECT_WIREFRAME, pipeline_wireframe);
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
#include "src/scenes/debugscene/multi_render_target_test.h"

#include "src/directx/shader/alpha_blend_desc.h"
#include "src/directx/shader/shader_register_id.h"
#include "src/directx/vertex.h"
#include "src/game/game_device.h"
#include "src/util/loader/glb_loader.h"
#include "src/util/path.h"

namespace {
legend::u32 PRE_POST_PROCESS_RENDER_TARGET_ID = 2;
}  // namespace

namespace legend {
namespace scenes {
namespace debugscene {
MultiRenderTargetTest::MultiRenderTargetTest(ISceneChange* scene_change)
    : Scene(scene_change) {}

MultiRenderTargetTest::~MultiRenderTargetTest() {
  game::GameDevice::GetInstance()->GetDevice().WaitForGPU();
}
bool MultiRenderTargetTest::Initialize() {
  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();

  //通常描画用パラメータ設定
  {
    const std::filesystem::path filepath =
        util::Path::GetInstance()->model() / L"1000cmObject.glb";
    if (!model_.Init(filepath)) {
      return false;
    }

    const math::IntVector2 screen_size =
        game::GameDevice::GetInstance()->GetWindow().GetScreenSize();

    std::vector<directx::render_target::RenderResourceManager::Info> infos{
        {3, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
         static_cast<u32>(screen_size.x), static_cast<u32>(screen_size.y),
         util::Color4(1.0f, 0.0f, 0.0f, 1.0f), L"RenderTarget_0"},
        {5, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
         static_cast<u32>(screen_size.x), static_cast<u32>(screen_size.y),
         util::Color4(0.0f, 1.0f, 0.0f, 1.0f), L"RenderTarget_1"}};

    if (!device.GetRenderResourceManager().IsRegisteredRenderTargetID(
            PRE_POST_PROCESS_RENDER_TARGET_ID)) {
      //ポストプロセス描画用レンダーターゲット
      if (!device.GetRenderResourceManager().CreateRenderTargets(
              device, PRE_POST_PROCESS_RENDER_TARGET_ID, infos)) {
        return false;
      }
    }
    if (!pipeline_state_.Init(device)) {
      return false;
    }

    pipeline_state_.SetRootSignature(device.GetDefaultRootSignature());
    const std::filesystem::path shader_root_path =
        util::Path::GetInstance()->shader();
    auto vertex_shader = std::make_shared<directx::shader::VertexShader>();
    if (!vertex_shader->Init(
            device,
            shader_root_path / L"multi_render_target_test" /
                L"multi_render_target_test_vs.cso",
            directx::input_element::GetElementDescs<directx::Vertex>())) {
      return false;
    }
    pipeline_state_.SetVertexShader(vertex_shader);

    auto pixel_shader = std::make_shared<directx::shader::PixelShader>();
    if (!pixel_shader->Init(device, shader_root_path /
                                        L"multi_render_target_test" /
                                        L"multi_render_target_test_ps.cso")) {
      return false;
    }
    pipeline_state_.SetPixelShader(pixel_shader);

    pipeline_state_.SetBlendDesc(
        directx::shader::alpha_blend_desc::BLEND_DESC_DEFAULT, 0);
    device.GetRenderResourceManager().WriteRenderTargetInfoToPipelineDesc(
        device, PRE_POST_PROCESS_RENDER_TARGET_ID, pipeline_state_);
    if (!pipeline_state_.CreatePipelineState(device)) {
      return false;
    }

    const math::Quaternion camera_rotation = math::Quaternion::kIdentity;
    const float aspect_ratio = screen_size.x * 1.0f / screen_size.y;
    if (!camera_.Init(L"MainCamera", math::Vector3(0.0f, 1.0f, -1.0f),
                      camera_rotation, 60.0f * math::util::DEG_2_RAD,
                      aspect_ratio, math::Vector3::kUpVector)) {
      return false;
    }
  }

  //ポストプロセス描画用パラメータ
  {
    if (!post_process_pipeline_.Init(device)) {
      return false;
    }
    post_process_pipeline_.SetRootSignature(device.GetDefaultRootSignature());

    const std::filesystem::path shader_root_path =
        util::Path::GetInstance()->shader();
    auto vertex_shader = std::make_shared<directx::shader::VertexShader>();
    if (!vertex_shader->Init(
            device,
            shader_root_path / L"multi_render_target_test" /
                L"multi_render_target_test_pp_vs.cso",
            directx::input_element::GetElementDescs<directx::Sprite>())) {
      return false;
    }
    post_process_pipeline_.SetVertexShader(vertex_shader);

    auto pixel_shader = std::make_shared<directx::shader::PixelShader>();
    if (!pixel_shader->Init(device,
                            shader_root_path / L"multi_render_target_test" /
                                L"multi_render_target_test_pp_ps.cso")) {
      return false;
    }
    post_process_pipeline_.SetPixelShader(pixel_shader);

    post_process_pipeline_.SetBlendDesc(
        directx::shader::alpha_blend_desc::BLEND_DESC_DEFAULT, 0);
    device.GetRenderResourceManager().WriteRenderTargetInfoToPipelineDesc(
        device, 0, post_process_pipeline_);
    if (!post_process_pipeline_.CreatePipelineState(device)) {
      return false;
    }
    const math::IntVector2 screen_size =
        game::GameDevice::GetInstance()->GetWindow().GetScreenSize();

    const math::Vector2 screen_size_float(static_cast<float>(screen_size.x),
                                          static_cast<float>(screen_size.y));
    if (!post_process_transform_cb_.Init(
            device, directx::shader::ConstantBufferRegisterID::Transform,
            device.GetLocalHeapHandle(0),
            L"PostProcess_TransformConstantBuffer")) {
      return false;
    }
    post_process_transform_cb_.GetStagingRef().world =
        math::Matrix4x4::CreateScale(
            math::Vector3(screen_size_float.x, screen_size_float.y, 1.0f));
    post_process_transform_cb_.UpdateStaging();

    if (!post_process_world_cb_.Init(
            device, directx::shader::ConstantBufferRegisterID::WorldContext,
            device.GetLocalHeapHandle(0), L"PostProcess_WorldConstantBuffer")) {
      return false;
    }
    post_process_world_cb_.GetStagingRef().view = math::Matrix4x4::kIdentity;
    post_process_world_cb_.GetStagingRef().projection =
        math::Matrix4x4::CreateOrthographic(screen_size_float);
    post_process_world_cb_.UpdateStaging();

    //ポストプロセス描画用頂点
    const std::vector<directx::Sprite> vertices{
        {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
        {{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}}};
    if (!post_process_vertex_buffer_.Init(device, sizeof(directx::Sprite),
                                          static_cast<u32>(vertices.size()),
                                          L"PostProcess_VertexBuffer")) {
      return false;
    }
    if (!post_process_vertex_buffer_.WriteBufferResource(vertices)) {
      return false;
    }

    const std::vector<u16> indices{0, 1, 2, 0, 2, 3};
    if (!post_process_index_buffer_.InitAndWrite(
            device, indices, directx::PrimitiveTopology::TriangleList,
            L"PostProcess_IndexBuffer")) {
      return false;
    }

    if (!post_process_local_cb_.Init(device, 2, device.GetLocalHeapHandle(1),
                                     L"Local")) {
      return false;
    }
    post_process_local_cb_.GetStagingRef().border = 0.5f;
    post_process_local_cb_.UpdateStaging();
  }

  return true;
}

bool MultiRenderTargetTest::Update() {
  if (ImGui::Begin("Camera")) {
    //カメラ座標
    math::Vector3 camera_position = camera_.GetPosition();
    ImGui::SliderFloat3("Position", &camera_position.x, -100.0f, 100.0f);
    camera_.SetPosition(camera_position);
    //カメラ回転角
    math::Vector3 camera_rotation =
        math::Quaternion::ToEular(camera_.GetRotation()) *
        math::util::RAD_2_DEG;
    ImGui::SliderFloat3("Rotation", &camera_rotation.x, -180.0f, 180.0f);
    camera_.SetRotation(
        math::Quaternion::FromEular(camera_rotation * math::util::DEG_2_RAD));
    if (ImGui::Button("X_UP")) {
      camera_.SetUpVector(math::Vector3::kRightVector);
    }
    if (ImGui::Button("Y_UP")) {
      camera_.SetUpVector(math::Vector3::kUpVector);
    }
    if (ImGui::Button("Z_UP")) {
      camera_.SetUpVector(math::Vector3::kForwardVector);
    }
    float fov = camera_.GetFov() * math::util::RAD_2_DEG;
    ImGui::SliderFloat("FOV", &fov, 0.01f, 90.0f);
    camera_.SetFov(fov * math::util::DEG_2_RAD);

    float border = post_process_local_cb_.GetStagingRef().border;
    ImGui::SliderFloat("Border", &border, 0.0f, 1.0f);
    post_process_local_cb_.GetStagingRef().border = border;
    post_process_local_cb_.UpdateStaging();
  }
  ImGui::End();
  return true;
}
void MultiRenderTargetTest::Draw() {
  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();

  device.GetRenderResourceManager().SetRenderTarget(
      PRE_POST_PROCESS_RENDER_TARGET_ID);
  device.GetRenderResourceManager().SetRenderTargetsToCommandList(device);
  device.GetRenderResourceManager().ClearCurrentRenderTarget(device);
  camera_.RenderStart();
  pipeline_state_.SetGraphicsCommandList(device);
  model_.Draw();

  device.GetRenderResourceManager().SetRenderTarget(0);
  device.GetRenderResourceManager().SetRenderTargetsToCommandList(device);
  device.GetRenderResourceManager().ClearCurrentRenderTarget(device);
  post_process_pipeline_.SetGraphicsCommandList(device);
  device.GetRenderResourceManager().UseRenderTargetToShaderResource(
      device, PRE_POST_PROCESS_RENDER_TARGET_ID, 0);
  device.GetRenderResourceManager().UseRenderTargetToShaderResource(
      device, PRE_POST_PROCESS_RENDER_TARGET_ID, 1);
  post_process_world_cb_.SetToHeap(device);
  post_process_transform_cb_.SetToHeap(device);
  post_process_local_cb_.SetToHeap(device);
  device.GetHeapManager().CopyHeapAndSetToGraphicsCommandList(device);

  post_process_vertex_buffer_.SetGraphicsCommandList(device);
  post_process_index_buffer_.SetGraphicsCommandList(device);
  post_process_index_buffer_.Draw(device);
}
void MultiRenderTargetTest::Finalize() {
  game::GameDevice::GetInstance()
      ->GetDevice()
      .GetHeapManager()
      .ResetLocalHeapAllocateCounter(1);
}

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend
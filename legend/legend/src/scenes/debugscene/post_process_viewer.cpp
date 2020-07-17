#include "src/scenes/debugscene/post_process_viewer.h"

#include "src/directx/shader/alpha_blend_desc.h"
#include "src/directx/shader/shader_register_id.h"
#include "src/directx/vertex.h"
#include "src/game/game_device.h"
#include "src/util/loader/glb_loader.h"
#include "src/util/path.h"

namespace legend {
namespace scenes {
namespace debugscene {
PostProcessViewer::PostProcessViewer(ISceneChange* scene_change)
    : Scene(scene_change) {}

PostProcessViewer::~PostProcessViewer() {}
bool PostProcessViewer::Initialize() {
  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();

  //通常描画用パラメータ設定
  {
    const std::filesystem::path filepath =
        util::Path::GetInstance()->model() / L"1000cmObject.glb";
    util::loader::GLBLoader loader;
    if (!loader.Load(filepath)) {
      return false;
    }

    const u32 vertex_num = loader.GetVertexNum();
    std::vector<directx::Vertex> vertices(vertex_num);
    //頂点座標の読み込み
    [&]() {
      if (loader.GetPositionComponentSize() != 3) return;
      const std::vector<float> position = loader.GetPosition();
      for (u32 i = 0; i < vertex_num; i++) {
        vertices[i].position.x = position[i * 3 + 0];
        vertices[i].position.y = position[i * 3 + 1];
        vertices[i].position.z = position[i * 3 + 2];
      }
    }();
    //法線情報の読み込み
    [&]() {
      if (loader.GetNormalComponentSize() != 3) return;
      const std::vector<float> normal = loader.GetNormal();
      for (u32 i = 0; i < vertex_num; i++) {
        vertices[i].normal.x = normal[i * 3 + 0];
        vertices[i].normal.y = normal[i * 3 + 1];
        vertices[i].normal.z = normal[i * 3 + 2];
      }
    }();
    // UV
    [&]() {
      if (loader.GetUVComponentSize() != 2) return;
      const std::vector<float> uv = loader.GetUV();
      for (u32 i = 0; i < vertex_num; i++) {
        vertices[i].uv.x = uv[i * 2 + 0];
        vertices[i].uv.y = uv[i * 2 + 1];
      }
    }();
    //接線
    [&]() {
      if (loader.GetTangentComponentSize() != 3) return;
      const std::vector<float> tangent = loader.GetTangent();
      for (u32 i = 0; i < vertex_num; i++) {
        vertices[i].tangent.x = tangent[i * 4 + 0];
        vertices[i].tangent.y = tangent[i * 4 + 1];
        vertices[i].tangent.z = tangent[i * 4 + 2];
        vertices[i].tangent.w = tangent[i * 4 + 3];
      }
    }();

    if (!vertex_buffer_.Init(device, sizeof(directx::Vertex), vertex_num,
                             L"VertexBuffer")) {
      return false;
    }
    if (!vertex_buffer_.WriteBufferResource(vertices)) {
      return false;
    }

    const std::vector<u16> indices = loader.GetIndex();
    if (!index_buffer_.InitAndWrite(device, indices,
                                    directx::PrimitiveTopology::TriangleList,
                                    L"IndexBuffer")) {
      return false;
    }

    const std::vector<u8> albedo = loader.GetAlbedo();
    if (!texture_.InitAndWrite(device,
                               directx::shader::TextureRegisterID::Albedo,
                               albedo, L"AlbedoTexture")) {
      return false;
    }

    if (!pipeline_state_.Init(device)) {
      return false;
    }

    pipeline_state_.SetRootSignature(device.GetDefaultRootSignature());
    const std::filesystem::path shader_root_path =
        util::Path::GetInstance()->shader();
    auto vertex_shader = std::make_shared<directx::shader::VertexShader>();
    if (!vertex_shader->Init(
            device, shader_root_path / L"modelview" / L"model_view_vs.cso",
            directx::input_element::GetElementDescs<directx::Vertex>())) {
      return false;
    }
    pipeline_state_.SetVertexShader(vertex_shader);

    auto pixel_shader = std::make_shared<directx::shader::PixelShader>();
    if (!pixel_shader->Init(
            device, shader_root_path / L"modelview" / L"model_view_ps.cso")) {
      return false;
    }
    pipeline_state_.SetPixelShader(pixel_shader);

    pipeline_state_.SetBlendDesc(
        directx::shader::alpha_blend_desc::BLEND_DESC_DEFAULT, 0);
    pipeline_state_.SetRenderTargetInfo(device.GetRenderTarget(), true);
    if (!pipeline_state_.CreatePipelineState(device)) {
      return false;
    }

    const math::Quaternion camera_rotation = math::Quaternion::kIdentity;
    const math::IntVector2 screen_size =
        game::GameDevice::GetInstance()->GetWindow().GetScreenSize();
    const float aspect_ratio = screen_size.x * 1.0f / screen_size.y;
    if (!camera_.Init(L"MainCamera", math::Vector3(0.0f, 1.0f, -1.0f),
                      camera_rotation, 60.0f * math::util::DEG_2_RAD,
                      aspect_ratio, math::Vector3::kUpVector)) {
      return false;
    }

    if (!transform_cb_.Init(
            device, directx::shader::ConstantBufferRegisterID::Transform,
            L"TransformConstantBuffer")) {
      return false;
    }
    transform_cb_.GetStagingRef().world = math::Matrix4x4::kIdentity;
    transform_cb_.UpdateStaging();
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
            device, shader_root_path / L"postprocess" / L"gray_scale_vs.cso",
            directx::input_element::GetElementDescs<directx::Sprite>())) {
      return false;
    }
    post_process_pipeline_.SetVertexShader(vertex_shader);

    auto pixel_shader = std::make_shared<directx::shader::PixelShader>();
    if (!pixel_shader->Init(
            device, shader_root_path / L"postprocess" / L"gray_scale_ps.cso")) {
      return false;
    }
    post_process_pipeline_.SetPixelShader(pixel_shader);

    post_process_pipeline_.SetBlendDesc(
        directx::shader::alpha_blend_desc::BLEND_DESC_DEFAULT, 0);
    post_process_pipeline_.SetRenderTargetInfo(device.GetRenderTarget(), true);
    if (!post_process_pipeline_.CreatePipelineState(device)) {
      return false;
    }
    const math::IntVector2 screen_size =
        game::GameDevice::GetInstance()->GetWindow().GetScreenSize();
    //ポストプロセス描画用レンダーターゲット
    if (!render_target_texture_.Init(
            device, directx::shader::TextureRegisterID::Albedo,
            DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, screen_size.x,
            screen_size.y, util::Color4(0.0f, 0.0f, 0.0f, 1.0f), L"RTT")) {
      return false;
    }

    const math::Vector2 screen_size_float(static_cast<float>(screen_size.x),
                                          static_cast<float>(screen_size.y));
    if (!post_process_transform_cb_.Init(
            device, directx::shader::ConstantBufferRegisterID::Transform,
            L"PostProcess_TransformConstantBuffer")) {
      return false;
    }
    post_process_transform_cb_.GetStagingRef().world =
        math::Matrix4x4::CreateScale(
            math::Vector3(screen_size_float.x, screen_size_float.y, 1.0f));
    post_process_transform_cb_.UpdateStaging();

    if (!post_process_world_cb_.Init(
            device, directx::shader::ConstantBufferRegisterID::WorldContext,
            L"PostProcess_WorldConstantBuffer")) {
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
  }

  return true;
}

bool PostProcessViewer::Update() {
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
  }
  ImGui::End();
  return true;
}
void PostProcessViewer::Draw() {
  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();

  render_target_texture_.SetRenderTarget(device);
  render_target_texture_.ClearRenderTarget(device);
  camera_.RenderStart();
  pipeline_state_.SetGraphicsCommandList(device);
  device.GetHeapManager().SetGraphicsCommandList(device);

  transform_cb_.UpdateStaging();
  transform_cb_.SetToHeap(device);
  texture_.SetToHeap(device);
  device.GetHeapManager().CopyHeapAndSetToGraphicsCommandList(device);

  vertex_buffer_.SetGraphicsCommandList(device);
  index_buffer_.SetGraphicsCommandList(device);
  index_buffer_.Draw(device);

  device.SetBackBuffer();
  post_process_pipeline_.SetGraphicsCommandList(device);
  render_target_texture_.SetToGlobalHeap(device);
  post_process_world_cb_.SetToHeap(device);
  post_process_transform_cb_.SetToHeap(device);
  device.GetHeapManager().CopyHeapAndSetToGraphicsCommandList(device);

  post_process_vertex_buffer_.SetGraphicsCommandList(device);
  post_process_index_buffer_.SetGraphicsCommandList(device);
  post_process_index_buffer_.Draw(device);
}
void PostProcessViewer::Finalize() {}
}  // namespace debugscene
}  // namespace scenes
}  // namespace legend
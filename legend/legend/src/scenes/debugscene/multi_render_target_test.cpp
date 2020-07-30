#include "src/scenes/debugscene/multi_render_target_test.h"

#include "src/directx/descriptor_heap/heap_parameter.h"
#include "src/directx/shader/alpha_blend_desc.h"
#include "src/directx/shader/shader_register_id.h"
#include "src/directx/vertex.h"
#include "src/game/game_device.h"
#include "src/util/loader/glb_loader.h"
#include "src/util/path.h"

namespace {
struct VS_LOAD_INFO {
  legend::util::resource::id::VertexShader id;
  std::filesystem::path filepath;
};

static VS_LOAD_INFO VS_PAIRS[] = {
    {legend::util::resource::id::VertexShader::MULTI_RENDER_TARGET_TEST,
     std::filesystem::path("multi_render_target_test") /
         "multi_render_target_test_vs.cso"},
    {legend::util::resource::id::VertexShader::MULTI_RENDER_TARGET_TEST_PP,
     std::filesystem::path("multi_render_target_test") /
         "multi_render_target_test_pp_vs.cso"}};

struct PS_LOAD_INFO {
  legend::util::resource::id::PixelShader id;
  std::filesystem::path filepath;
};

static PS_LOAD_INFO PS_PAIRS[] = {
    {legend::util::resource::id::PixelShader::MULTI_RENDER_TARGET_TEST,
     std::filesystem::path("multi_render_target_test") /
         "multi_render_target_test_ps.cso"},
    {legend::util::resource::id::PixelShader::MULTI_RENDER_TARGET_TEST_PP,
     std::filesystem::path("multi_render_target_test") /
         "multi_render_target_test_pp_ps.cso"}};

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
  util::resource::Resource& resource =
      game::GameDevice::GetInstance()->GetResource();

  //事前に使用するリソースを読み込む
  {
    const std::filesystem::path shader_path =
        util::Path::GetInstance()->shader();

    for (auto&& info : VS_PAIRS) {
      resource.GetVertexShader().Load(info.id, shader_path / info.filepath);
    }
    for (auto&& info : PS_PAIRS) {
      resource.GetPixelShader().Load(info.id, shader_path / info.filepath);
    }
  }

  //通常描画用パラメータ設定
  {
    //描画するモデルを読み込む
    const std::filesystem::path filepath =
        util::Path::GetInstance()->model() / L"1000cmObject.glb";
    if (!model_.Init(filepath)) {
      return false;
    }

    const math::IntVector2 screen_size =
        game::GameDevice::GetInstance()->GetWindow().GetScreenSize();

    const u32 w = static_cast<u32>(screen_size.x);
    const u32 h = static_cast<u32>(screen_size.y);
    std::vector<directx::render_target::RenderResourceManager::Info> infos{
        {3, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, w, h,
         util::Color4(1.0f, 0.0f, 0.0f, 1.0f), L"RenderTarget_0"},
        {5, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, w, h,
         util::Color4(0.0f, 1.0f, 0.0f, 1.0f), L"RenderTarget_1"}};

    //まだレンダーターゲットが作られていないなら作る
    if (!device.GetRenderResourceManager().IsRegisteredRenderTargetID(
            directx::render_target::RenderTargetID::MULTI_RENDER_TARGET_TEST)) {
      if (!device.GetRenderResourceManager().CreateRenderTargets(
              device,
              directx::render_target::RenderTargetID::MULTI_RENDER_TARGET_TEST,
              infos)) {
        return false;
      }
    }
    auto ps = std::make_shared<directx::shader::GraphicsPipelineState>();
    ps->Init(device);
    ps->SetVertexShader(resource.GetVertexShader().Get(
        util::resource::id::VertexShader::MULTI_RENDER_TARGET_TEST));
    ps->SetPixelShader(resource.GetPixelShader().Get(
        util::resource::id::PixelShader::MULTI_RENDER_TARGET_TEST));
    ps->SetBlendDesc(directx::shader::alpha_blend_desc::BLEND_DESC_DEFAULT, 0);
    device.GetRenderResourceManager().WriteDepthStencilTargetInfoToPipeline(
        device, directx::render_target::DepthStencilTargetID::Depth, ps.get());
    device.GetRenderResourceManager().WriteRenderTargetInfoToPipeline(
        device,
        directx::render_target::RenderTargetID::MULTI_RENDER_TARGET_TEST,
        ps.get());
    ps->SetRootSignature(device.GetDefaultRootSignature());
    ps->CreatePipelineState(device);
    resource.GetPipeline().Register(
        util::resource::id::Pipeline::MULTI_RENDER_TARGET_TEST, ps);

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
    auto ps = std::make_shared<directx::shader::GraphicsPipelineState>();
    ps->Init(device);
    ps->SetVertexShader(resource.GetVertexShader().Get(
        util::resource::id::VertexShader::MULTI_RENDER_TARGET_TEST_PP));
    ps->SetPixelShader(resource.GetPixelShader().Get(
        util::resource::id::PixelShader::MULTI_RENDER_TARGET_TEST_PP));
    ps->SetBlendDesc(directx::shader::alpha_blend_desc::BLEND_DESC_DEFAULT, 0);
    device.GetRenderResourceManager().WriteDepthStencilTargetInfoToPipeline(
        device, directx::render_target::DepthStencilTargetID::None, ps.get());
    device.GetRenderResourceManager().WriteRenderTargetInfoToPipeline(
        device, directx::render_target::RenderTargetID::BACK_BUFFER, ps.get());
    ps->SetRootSignature(device.GetDefaultRootSignature());
    ps->CreatePipelineState(device);
    resource.GetPipeline().Register(
        util::resource::id::Pipeline::MULTI_RENDER_TARGET_TEST_PP, ps);

    const math::IntVector2 screen_size =
        game::GameDevice::GetInstance()->GetWindow().GetScreenSize();

    const math::Vector2 screen_size_float(static_cast<float>(screen_size.x),
                                          static_cast<float>(screen_size.y));
    if (!post_process_transform_cb_.Init(
            device, directx::shader::ConstantBufferRegisterID::Transform,
            device.GetLocalHeapHandle(
                directx::descriptor_heap::heap_parameter::LocalHeapID::
                    MULTI_RENDER_TARGET_TEST_SCENE),
            L"PostProcess_TransformConstantBuffer")) {
      return false;
    }
    post_process_transform_cb_.GetStagingRef().world =
        math::Matrix4x4::CreateScale(
            math::Vector3(screen_size_float.x, screen_size_float.y, 1.0f));
    post_process_transform_cb_.UpdateStaging();

    if (!post_process_world_cb_.Init(
            device, directx::shader::ConstantBufferRegisterID::WorldContext,
            device.GetLocalHeapHandle(
                directx::descriptor_heap::heap_parameter::LocalHeapID::
                    MULTI_RENDER_TARGET_TEST_SCENE),
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

    if (!post_process_local_cb_.Init(
            device, 2,
            device.GetLocalHeapHandle(
                directx::descriptor_heap::heap_parameter::LocalHeapID::
                    MULTI_RENDER_TARGET_TEST_SCENE),
            L"Local")) {
      return false;
    }
    post_process_local_cb_.GetStagingRef().border = 0.5f;
    post_process_local_cb_.UpdateStaging();
  }

  constexpr u32 MODEL_NUM = 10;
  for (u32 i = 0; i < MODEL_NUM; i++) {
    transforms_.emplace_back(math::Vector3(3.0f * i, 0, 0));
    auto& cb = transform_cbs_.emplace_back();
    if (!cb.Init(device, directx::shader::ConstantBufferRegisterID::Transform,
                 device.GetLocalHeapHandle(
                     directx::descriptor_heap::heap_parameter::LocalHeapID::
                         MULTI_RENDER_TARGET_TEST_SCENE),
                 L"")) {
      return false;
    }
  }
  device.WaitForGPU();
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

    //カメラの上方向ベクトルを変更する
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

    //ボーダーラインを移動する
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

  //まず、マルチレンダーターゲット側にモデルを描画する
  device.GetRenderResourceManager().SetRenderTargetID(
      directx::render_target::RenderTargetID::MULTI_RENDER_TARGET_TEST);
  device.GetRenderResourceManager().SetDepthStencilTargetID(
      directx::render_target::DepthStencilTargetID::Depth);
  device.GetRenderResourceManager().SetRenderTargetsToCommandList(device);
  device.GetRenderResourceManager().ClearCurrentRenderTarget(device);
  device.GetRenderResourceManager().ClearCurrentDepthStencilTarget(device);
  camera_.RenderStart();
  game::GameDevice::GetInstance()
      ->GetResource()
      .GetPipeline()
      .Get(util::resource::id::Pipeline::MULTI_RENDER_TARGET_TEST)
      ->SetGraphicsCommandList(device);

  for (u32 i = 0; i < static_cast<u32>(transforms_.size()); i++) {
    transform_cbs_[i].GetStagingRef().world =
        transforms_[i].CreateWorldMatrix();
    transform_cbs_[i].UpdateStaging();
    transform_cbs_[i].SetToHeap(device);
    model_.Draw();
  }

  //バックバッファに切り替え、ポストプロセスを利用して描画をする
  device.GetRenderResourceManager().SetRenderTargetID(
      directx::render_target::RenderTargetID::BACK_BUFFER);
  device.GetRenderResourceManager().SetDepthStencilTargetID(
      directx::render_target::DepthStencilTargetID::None);
  device.GetRenderResourceManager().SetRenderTargetsToCommandList(device);
  device.GetRenderResourceManager().ClearCurrentRenderTarget(device);
  game::GameDevice::GetInstance()
      ->GetResource()
      .GetPipeline()
      .Get(util::resource::id::Pipeline::MULTI_RENDER_TARGET_TEST_PP)
      ->SetGraphicsCommandList(device);

  device.GetRenderResourceManager().UseRenderTargetToShaderResource(
      device, directx::render_target::RenderTargetID::MULTI_RENDER_TARGET_TEST,
      0);
  device.GetRenderResourceManager().UseRenderTargetToShaderResource(
      device, directx::render_target::RenderTargetID::MULTI_RENDER_TARGET_TEST,
      1);
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
      .ResetLocalHeapAllocateCounter(
          directx::descriptor_heap::heap_parameter::LocalHeapID::
              MULTI_RENDER_TARGET_TEST_SCENE);

  util::resource::Resource& resource =
      game::GameDevice::GetInstance()->GetResource();
  for (auto&& info : VS_PAIRS) {
    resource.GetVertexShader().Unload(info.id);
  }
  for (auto&& info : PS_PAIRS) {
    resource.GetPixelShader().Unload(info.id);
  }
  resource.GetPipeline().Unload(
      util::resource::id::Pipeline::MULTI_RENDER_TARGET_TEST);
  resource.GetPipeline().Unload(
      util::resource::id::Pipeline::MULTI_RENDER_TARGET_TEST_PP);
}

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend
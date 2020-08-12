#include "src/scenes/debugscene/multi_render_target_test.h"

#include "src/directx/shader/alpha_blend_desc.h"
#include "src/directx/shader/shader_register_id.h"
#include "src/directx/vertex.h"
#include "src/game/game_device.h"
#include "src/util/resource/resource_id.h"

namespace {

using VertexShader_ID = legend::util::resource::id::VertexShader;
using PixelShader_ID = legend::util::resource::id::PixelShader;
using Pipeline_ID = legend::util::resource::id::Pipeline;
using Model_ID = legend::util::resource::id::Model;
using RenderTarget_ID = legend::directx::render_target::RenderTargetID;
using DepthStencilTarget_ID =
    legend::directx::render_target::DepthStencilTargetID;
using ConstantBuffer_ID =
    legend::directx::shader::ConstantBufferRegisterID::Enum;

struct VS_LOAD_INFO {
  VertexShader_ID id;
  std::filesystem::path filepath;
};

//読み込む頂点シェーダーのリスト
static VS_LOAD_INFO VS_LOAD_LISTS[] = {
    {VertexShader_ID::MULTI_RENDER_TARGET_TEST,
     std::filesystem::path("multi_render_target_test") /
         "multi_render_target_test_vs.cso"},
    {VertexShader_ID::MULTI_RENDER_TARGET_TEST_PP,
     std::filesystem::path("multi_render_target_test") /
         "multi_render_target_test_pp_vs.cso"}};

struct PS_LOAD_INFO {
  PixelShader_ID id;
  std::filesystem::path filepath;
};

//読み込むピクセルシェーダーのリスト
static PS_LOAD_INFO PS_LOAD_LISTS[] = {
    {PixelShader_ID::MULTI_RENDER_TARGET_TEST,
     std::filesystem::path("multi_render_target_test") /
         "multi_render_target_test_ps.cso"},
    {PixelShader_ID::MULTI_RENDER_TARGET_TEST_PP,
     std::filesystem::path("multi_render_target_test") /
         "multi_render_target_test_pp_ps.cso"}};

}  // namespace

namespace legend {
namespace scenes {
namespace debugscene {

//コンストラクタ
MultiRenderTargetTest::MultiRenderTargetTest(ISceneChange* scene_change)
    : Scene(scene_change) {}

//デストラクタ
MultiRenderTargetTest::~MultiRenderTargetTest() {}

//初期化
bool MultiRenderTargetTest::Initialize() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& resource = game::GameDevice::GetInstance()->GetResource();

  directx::device::CommandList command_list;
  if (!command_list.Init(
          device, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT)) {
    return false;
  }

  //事前に使用するリソースを読み込む
  {
    const std::filesystem::path shader_path =
        util::Path::GetInstance()->shader();

    for (auto&& info : VS_LOAD_LISTS) {
      if (!resource.GetVertexShader().Load(info.id,
                                           shader_path / info.filepath)) {
        return false;
      }
    }
    for (auto&& info : PS_LOAD_LISTS) {
      if (!resource.GetPixelShader().Load(info.id,
                                          shader_path / info.filepath)) {
        return false;
      }
    }
  }

  //通常描画用パラメータ設定
  {
    const math::IntVector2 screen_size =
        game::GameDevice::GetInstance()->GetWindow().GetScreenSize();

    const u32 w = static_cast<u32>(screen_size.x);
    const u32 h = static_cast<u32>(screen_size.y);

    // multi_render_target_ppのテクスチャのレジスター番号
    constexpr u32 OUTPUT_1_ID = 0;
    constexpr u32 OUTPUT_2_ID = 1;
    std::vector<directx::render_target::RenderResourceManager::Info> infos{
        {OUTPUT_1_ID, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, w, h,
         util::Color4(1.0f, 0.0f, 0.0f, 1.0f), L"RenderTarget_0"},
        {OUTPUT_2_ID, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, w, h,
         util::Color4(0.0f, 1.0f, 0.0f, 1.0f), L"RenderTarget_1"}};

    //レンダーターゲットが登録済みでなければ登録する
    if (!device.GetRenderResourceManager().IsRegisteredRenderTarget(
            RenderTarget_ID::MULTI_RENDER_TARGET_TEST)) {
      if (!device.GetRenderResourceManager().AddRenderTarget(
              RenderTarget_ID::MULTI_RENDER_TARGET_TEST, device, infos)) {
        return false;
      }
    }

    auto ps = std::make_shared<directx::shader::PipelineState>();

    D3D12_GRAPHICS_PIPELINE_STATE_DESC pso_desc = {};
    pso_desc.BlendState.RenderTarget[0] =
        directx::shader::alpha_blend_desc::BLEND_DESC_ALIGNMENT;
    pso_desc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC1(D3D12_DEFAULT);
    pso_desc.DSVFormat = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
    pso_desc.Flags = D3D12_PIPELINE_STATE_FLAGS::D3D12_PIPELINE_STATE_FLAG_NONE;
    pso_desc.InputLayout = resource.GetVertexShader()
                               .Get(VertexShader_ID::MULTI_RENDER_TARGET_TEST)
                               ->GetInputLayout();
    pso_desc.NodeMask = 0;
    pso_desc.NumRenderTargets = 2;
    pso_desc.PrimitiveTopologyType =
        D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    pso_desc.pRootSignature =
        device.GetDefaultRootSignature()->GetRootSignature();
    pso_desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    pso_desc.RTVFormats[0] = pso_desc.RTVFormats[1] =
        DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
    pso_desc.SampleDesc.Count = 1;
    pso_desc.SampleMask = UINT_MAX;
    pso_desc.PS = resource.GetPixelShader()
                      .Get(PixelShader_ID::MULTI_RENDER_TARGET_TEST)
                      ->GetShaderBytecode();
    pso_desc.VS = resource.GetVertexShader()
                      .Get(VertexShader_ID::MULTI_RENDER_TARGET_TEST)
                      ->GetShaderBytecode();
    if (!ps->Init(device, pso_desc)) {
      return false;
    }
    resource.GetPipeline().Register(
        util::resource::id::Pipeline::MULTI_RENDER_TARGET_TEST, ps);

    const math::Quaternion camera_rotation = math::Quaternion::kIdentity;
    const float aspect_ratio = screen_size.x * 1.0f / screen_size.y;
    if (!camera_.Init(L"MainCamera", math::Vector3(0.0f, 1.0f, -1.0f),
                      camera_rotation, 60.0f * math::util::DEG_2_RAD,
                      aspect_ratio, math::Vector3::kUpVector, 0.1f, 1000.0f)) {
      return false;
    }
  }

  //ポストプロセス描画用パラメータ
  {
    auto ps = std::make_shared<directx::shader::PipelineState>();
    directx::shader::PipelineState::GraphicsPipelineStateDesc pso_desc = {};
    pso_desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    pso_desc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
    pso_desc.DepthStencilState.DepthEnable = false;
    pso_desc.DSVFormat = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
    pso_desc.InputLayout =
        resource.GetVertexShader()
            .Get(VertexShader_ID::MULTI_RENDER_TARGET_TEST_PP)
            ->GetInputLayout();
    pso_desc.NumRenderTargets = 1;
    pso_desc.PrimitiveTopologyType =
        D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    pso_desc.pRootSignature =
        device.GetDefaultRootSignature()->GetRootSignature();
    pso_desc.PS = resource.GetPixelShader()
                      .Get(PixelShader_ID::MULTI_RENDER_TARGET_TEST_PP)
                      ->GetShaderBytecode();
    pso_desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    pso_desc.RTVFormats[0] = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
    pso_desc.SampleDesc.Count = 1;
    pso_desc.SampleMask = UINT_MAX;
    pso_desc.VS = resource.GetVertexShader()
                      .Get(VertexShader_ID::MULTI_RENDER_TARGET_TEST_PP)
                      ->GetShaderBytecode();
    if (!ps->Init(device, pso_desc)) {
      return false;
    }
    resource.GetPipeline().Register(
        util::resource::id::Pipeline::MULTI_RENDER_TARGET_TEST_PP, ps);

    const math::IntVector2 screen_size =
        game::GameDevice::GetInstance()->GetWindow().GetScreenSize();

    const math::Vector2 screen_size_float(static_cast<float>(screen_size.x),
                                          static_cast<float>(screen_size.y));
    if (!post_process_transform_cb_.Init(
            device, ConstantBuffer_ID::TRANSFORM,
            device.GetLocalHandle(directx::descriptor_heap::heap_parameter::
                                      LocalHeapID::GLOBAL_ID),
            L"PostProcess_TransformConstantBuffer")) {
      return false;
    }
    post_process_transform_cb_.GetStagingRef().world =
        math::Matrix4x4::CreateScale(
            math::Vector3(screen_size_float.x, screen_size_float.y, 1.0f));
    post_process_transform_cb_.UpdateStaging();

    if (!post_process_world_cb_.Init(
            device, ConstantBuffer_ID::WORLD_CONTEXT,
            device.GetLocalHandle(directx::descriptor_heap::heap_parameter::
                                      LocalHeapID::GLOBAL_ID),
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
    if (!post_process_vertex_buffer_.WriteBufferResource(vertices.data())) {
      return false;
    }

    const std::vector<u16> indices{0, 1, 2, 0, 2, 3};
    const u32 index_num = static_cast<u32>(indices.size());
    if (!post_process_index_buffer_.Init(
            device, sizeof(u16), index_num,
            directx::PrimitiveTopology::TRIANGLE_LIST,
            L"PostProcess_IndexBuffer")) {
      return false;
    }
    if (!post_process_index_buffer_.WriteBufferResource(indices.data())) {
      return false;
    }

    constexpr u32 POST_PROCESS_LOCAL_CONSTANT_BUFFER_ID = 4;
    if (!post_process_local_cb_.Init(
            device, POST_PROCESS_LOCAL_CONSTANT_BUFFER_ID,
            device.GetLocalHandle(directx::descriptor_heap::heap_parameter::
                                      LocalHeapID::GLOBAL_ID),
            L"Local")) {
      return false;
    }
    post_process_local_cb_.GetStagingRef().border = 0.5f;
    post_process_local_cb_.UpdateStaging();
  }

  constexpr u32 MODEL_NUM = 1;
  for (u32 i = 0; i < MODEL_NUM; i++) {
    transforms_.emplace_back(math::Vector3(i * 1.0f, 0, i * 1.0f));
    auto& cb = transform_cbs_.emplace_back();
    if (!cb.Init(
            device, ConstantBuffer_ID::TRANSFORM,
            device.GetLocalHandle(directx::descriptor_heap::heap_parameter::
                                      LocalHeapID::GLOBAL_ID),
            L"")) {
      return false;
    }
  }

  if (!command_list.Close()) {
    return false;
  }

  device.ExecuteCommandList({command_list});
  device.WaitExecute();

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

  for (u32 i = 0; i < static_cast<u32>(transforms_.size()); i++) {
    transform_cbs_[i].GetStagingRef().world =
        transforms_[i].CreateWorldMatrix();
    transform_cbs_[i].UpdateStaging();
  }

  return true;
}
void MultiRenderTargetTest::Draw() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& resource = game::GameDevice::GetInstance()->GetResource();
  auto& render_resource_manager = device.GetRenderResourceManager();

  auto& command_list = device.GetCurrentFrameResource()->GetCommandList();

  //まず、マルチレンダーターゲット側にモデルを描画する
  render_resource_manager.SetRenderTargets(
      command_list, RenderTarget_ID::MULTI_RENDER_TARGET_TEST, true,
      DepthStencilTarget_ID::DEPTH_ONLY, true);
  camera_.RenderStart();
  game::GameDevice::GetInstance()
      ->GetResource()
      .GetPipeline()
      .Get(util::resource::id::Pipeline::MULTI_RENDER_TARGET_TEST)
      ->SetCommandList(command_list);

  for (u32 i = 0; i < static_cast<u32>(transforms_.size()); i++) {
    transform_cbs_[i].SetToHeap(device);
    resource.GetModel().Get(Model_ID::OBJECT_1000CM)->Draw(command_list);
  }

  //バックバッファに切り替え、ポストプロセスを利用して描画をする
  render_resource_manager.SetRenderTargets(command_list,
                                           RenderTarget_ID::BACK_BUFFER, false,
                                           DepthStencilTarget_ID::NONE, false);
  game::GameDevice::GetInstance()
      ->GetResource()
      .GetPipeline()
      .Get(util::resource::id::Pipeline::MULTI_RENDER_TARGET_TEST_PP)
      ->SetCommandList(command_list);

  render_resource_manager.UseAsSRV(
      device, RenderTarget_ID::MULTI_RENDER_TARGET_TEST, 0);
  render_resource_manager.UseAsSRV(
      device, RenderTarget_ID::MULTI_RENDER_TARGET_TEST, 1);

  post_process_world_cb_.SetToHeap(device);
  post_process_transform_cb_.SetToHeap(device);
  post_process_local_cb_.SetToHeap(device);
  device.GetHeapManager().SetHeapTableToGraphicsCommandList(device,
                                                            command_list);

  post_process_vertex_buffer_.SetGraphicsCommandList(command_list);
  post_process_index_buffer_.SetGraphicsCommandList(command_list);
  post_process_index_buffer_.Draw(command_list);
}

void MultiRenderTargetTest::Finalize() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  device.WaitExecute();

  util::resource::Resource& resource =
      game::GameDevice::GetInstance()->GetResource();
  for (auto&& info : VS_LOAD_LISTS) {
    resource.GetVertexShader().Unload(info.id);
  }
  for (auto&& info : PS_LOAD_LISTS) {
    resource.GetPixelShader().Unload(info.id);
  }

  resource.GetPipeline().Unload(Pipeline_ID::MULTI_RENDER_TARGET_TEST);
  resource.GetPipeline().Unload(Pipeline_ID::MULTI_RENDER_TARGET_TEST_PP);
}

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend
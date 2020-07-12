#include "src/scenes/debugscene/perspective_camera_test.h"

#include "src/directx/shader/alpha_blend_desc.h"
#include "src/directx/shader/shader_register_id.h"
#include "src/directx/vertex.h"
#include "src/game/game_device.h"
#include "src/util/loader/glb_loader.h"
#include "src/util/path.h"

namespace legend {
namespace scenes {
namespace debugscene {

//コンストラクタ
PerspectiveCameraTest::PerspectiveCameraTest(ISceneChange* scene_change)
    : Scene(scene_change) {}

//デストラクタ
PerspectiveCameraTest::~PerspectiveCameraTest() {}

//初期化
bool PerspectiveCameraTest::Initialize() {
  std::filesystem::path filepath =
      util::Path::GetInstance()->model() / L"checkXYZ.glb";
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

  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();
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
  if (!texture_.InitAndWrite(device, directx::shader::TextureRegisterID::Albedo,
                             albedo, L"AlbedoTexture")) {
    return false;
  }

  root_signature_ = std::make_unique<directx::shader::RootSignature>();
  if (!root_signature_->Init(device, L"DefaultRootSignature")) {
    return false;
  }

  if (!pipeline_state_.Init(device)) {
    return false;
  }
  pipeline_state_.SetRootSignature(root_signature_);

  const std::filesystem::path shader_root_path =
      util::Path::GetInstance()->shader();
  const std::vector<D3D12_INPUT_ELEMENT_DESC> elements{
      {"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0,
       D3D12_APPEND_ALIGNED_ELEMENT,
       D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
       0},
      {"NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0,
       D3D12_APPEND_ALIGNED_ELEMENT,
       D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
       0},
      {"TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0,
       D3D12_APPEND_ALIGNED_ELEMENT,
       D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
       0},
      {"TANGENT", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
       D3D12_APPEND_ALIGNED_ELEMENT,
       D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
       0}};

  auto vertex_shader = std::make_shared<directx::shader::VertexShader>();
  if (!vertex_shader->Init(
          device, shader_root_path / L"modelview" / L"model_view_vs.cso",
          elements)) {
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

  if (!transform_cb_.Init(device,
                          directx::shader::ConstantBufferRegisterID::Transform,
                          L"TransformConstantBuffer")) {
    return false;
  }
  transform_cb_.GetStagingRef().world = math::Matrix4x4::kIdentity;
  transform_cb_.UpdateStaging();

  if (!camera_.Init(L"MainCamera", math::Vector3(0, 3, -3),
                    math::Vector3(0, 0, 0), math::Vector3::kUpVector,
                    60.0f * math::util::DEG_2_RAD, 1280.0f / 720.0f, 0.1f,
                    300.0f)) {
    return false;
  }

  return true;
}

//更新
bool PerspectiveCameraTest::Update() { return true; }

//描画
void PerspectiveCameraTest::Draw() {
  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();

  camera_.RenderStart();
  root_signature_->SetGraphicsCommandList(device);
  pipeline_state_.SetGraphicsCommandList(device);
  device.GetHeapManager().SetGraphicsCommandList(device);

  transform_cb_.UpdateStaging();
  transform_cb_.SetToHeap(device);
  texture_.SetToHeap(device);
  device.GetHeapManager().CopyHeapAndSetToGraphicsCommandList(device);

  vertex_buffer_.SetGraphicsCommandList(device);
  index_buffer_.SetGraphicsCommandList(device);
  index_buffer_.Draw(device);
}

//終了処理
void PerspectiveCameraTest::Finalize() {}
}  // namespace debugscene
}  // namespace scenes
}  // namespace legend

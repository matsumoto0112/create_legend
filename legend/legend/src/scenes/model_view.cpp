#include "src/scenes/model_view.h"

#include "src/directx/vertex.h"
#include "src/game/game_device.h"
#include "src/libs/stb_image.h"
#include "src/util/loader/glb_loader.h"
#include "src/util/path.h"

namespace legend {
namespace scenes {

//コンストラクタ
ModelView::ModelView(ISceneChange* scene_change) : Scene(scene_change) {}

ModelView::~ModelView() {}

//初期化
void ModelView::Initialize() {
  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();

  const std::wstring name = L"maru_UV_TexUmekomi";
  const std::filesystem::path model_path =
      util::Path::GetInstance()->model() / (name + L".glb");
  util::loader::GLBLoader loader(model_path);

  const u32 vertex_num = loader.GetVertexNum();
  std::vector<directx::Vertex> vertices(vertex_num);

  //頂点座標
  {
    const std::vector<float> position_list = loader.GetPosition();
    const u32 position_component_size = loader.GetPositionComponentSize();

    if (position_component_size == 3) {
      for (u32 i = 0; i < vertex_num; i++) {
        vertices[i].position.x = position_list[i * position_component_size + 0];
        vertices[i].position.y = position_list[i * position_component_size + 1];
        vertices[i].position.z = position_list[i * position_component_size + 2];
      }
    } else {
      MY_LOG(L"頂点座標情報の格納に失敗しました。リソース名は%sです",
             name.c_str());
    }
  }

  //法線
  {
    const std::vector<float> normal_list = loader.GetNormal();
    const u32 normal_component_size = loader.GetNormalComponentSize();
    if (normal_component_size == 3) {
      for (u32 i = 0; i < vertex_num; i++) {
        vertices[i].normal.x = normal_list[i * normal_component_size + 0];
        vertices[i].normal.y = normal_list[i * normal_component_size + 1];
        vertices[i].normal.z = normal_list[i * normal_component_size + 2];
      }
    } else {
      MY_LOG(L"法線情報の格納に失敗しました。リソース名は%sです", name.c_str());
    }
  }

  // UV
  {
    const std::vector<float> uv_list = loader.GetUV();
    const u32 uv_component_size = loader.GetUVComponentSize();
    if (uv_component_size == 2) {
      for (u32 i = 0; i < vertex_num; i++) {
        vertices[i].uv.x = uv_list[i * uv_component_size + 0];
        vertices[i].uv.y = uv_list[i * uv_component_size + 1];
      }
    } else {
      MY_LOG(L"UV情報の格納に失敗しました。リソース名は%sです", name.c_str());
    }
  }

  //接線
  {
    const std::vector<float> tangent_list = loader.GetTangent();
    const u32 tangent_component_size = loader.GetTangentComponentSize();
    if (tangent_component_size == 4) {
      for (u32 i = 0; i < vertex_num; i++) {
        vertices[i].tangent.x = tangent_list[i * tangent_component_size + 0];
        vertices[i].tangent.y = tangent_list[i * tangent_component_size + 1];
        vertices[i].tangent.z = tangent_list[i * tangent_component_size + 2];
        vertices[i].tangent.w = tangent_list[i * tangent_component_size + 3];
      }
    } else {
      MY_LOG(L"接線情報の格納に失敗しました。リソース名は%sです", name.c_str());
    }
  }

  //頂点バッファ作成
  if (!vertex_buffer_.Init(device, sizeof(directx::Vertex), vertex_num,
                           name + L"_VertexBuffer")) {
    return;
  }
  if (!vertex_buffer_.WriteBufferResource(vertices)) {
    return;
  }

  const std::vector<u16> index = loader.GetIndex();
  //インデックスバッファ作成
  if (!index_buffer_.Init(device, static_cast<u32>(index.size()),
                          directx::PrimitiveTopology::TriangleList,
                          name + L"_IndexBuffer")) {
    return;
  }
  if (!index_buffer_.WriteBufferResource(index)) {
    return;
  }

  //メインテクスチャの書き込み
  const std::vector<u8> albedo = loader.GetAlbedo();
  if (!texture_.InitAndWrite(device, 0, albedo, name + L"_Albedo")) {
    return;
  }

  //ルートシグネチャ作成
  root_signature_ = std::make_shared<directx::shader::RootSignature>();
  if (!root_signature_->Init(game::GameDevice::GetInstance()->GetDevice(),
                             L"Global Root Signature")) {
    return;
  }

  //頂点シェーダー
  std::filesystem::path path = util::Path::GetInstance()->shader();
  std::filesystem::path vertex_shader_path =
      path / L"modelview" / L"model_view_vs.cso";
  std::filesystem::path pixel_shader_path =
      path / L"modelview" / L"model_view_ps.cso";
  std::vector<D3D12_INPUT_ELEMENT_DESC> elements{
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

  //頂点シェーダー
  std::shared_ptr<directx::shader::VertexShader> vertex_shader =
      std::make_shared<directx::shader::VertexShader>();
  if (!vertex_shader->Init(game::GameDevice::GetInstance()->GetDevice(),
                           vertex_shader_path, elements)) {
    return;
  }

  //ピクセルシェーダー
  std::shared_ptr<directx::shader::PixelShader> pixel_shader =
      std::make_shared<directx::shader::PixelShader>();
  if (!pixel_shader->Init(game::GameDevice::GetInstance()->GetDevice(),
                          pixel_shader_path)) {
    return;
  }

  if (!pipeline_state_.Init(game::GameDevice::GetInstance()->GetDevice())) {
    return;
  }

  //パイプライン作成開始
  pipeline_state_.SetRootSignature(root_signature_);
  pipeline_state_.SetVertexShader(vertex_shader);
  pipeline_state_.SetPixelShader(pixel_shader);

  if (!pipeline_state_.CreatePipelineState(
          game::GameDevice::GetInstance()->GetDevice())) {
    return;
  }

  if (!transform_cb_.Init(device, 0, L"Transform ConstantBuffer")) {
    return;
  }

  rotation_ = math::Vector3::kZeroVector;
  scale_ = math::Vector3::kUnitVector * 15.0f;
  transform_cb_.GetStagingRef().world =
      math::Matrix4x4::CreateScale(scale_) *
      math::Matrix4x4::CreateRotation(rotation_);
  transform_cb_.UpdateStaging();

  if (!world_cb_.Init(device, 1, L"WorldContext ConstantBuffer")) {
    return;
  }

  world_cb_.GetStagingRef().view = math::Matrix4x4::CreateView(
      math::Vector3(0, 10, -10), math::Vector3(0, 0, 0),
      math::Vector3::kUpVector);
  const float aspect = 1280.0f / 720.0f;
  world_cb_.GetStagingRef().projection =
      math::Matrix4x4::CreateProjection(45.0f, aspect, 0.1f, 100.0);
  world_cb_.UpdateStaging();
}

//更新
void ModelView::Update() {
  // rotation_.y += 0.01f;
  // rotation_.z += 0.005f;
  transform_cb_.GetStagingRef().world =
      math::Matrix4x4::CreateScale(scale_) *
      math::Matrix4x4::CreateRotation(rotation_);
  transform_cb_.UpdateStaging();
}

//描画
void ModelView::Draw() {
  Scene::Draw();

  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();
  root_signature_->SetGraphicsCommandList(device);
  pipeline_state_.SetGraphicsCommandList(device);
  device.GetHeapManager().SetGraphicsCommandList(device);
  transform_cb_.SetToHeap(device);
  world_cb_.SetToHeap(device);
  texture_.SetToHeap(device);
  device.GetHeapManager().CopyHeapAndSetToGraphicsCommandList(device);

  vertex_buffer_.SetGraphicsCommandList(device);
  index_buffer_.SetGraphicsCommandList(device);
  index_buffer_.Draw(device);
}
void ModelView::Finalize() {
  game::GameDevice::GetInstance()->GetDevice().WaitForGPU();
}
}  // namespace scenes
}  // namespace legend

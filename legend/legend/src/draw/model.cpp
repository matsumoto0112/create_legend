#include "src/draw/model.h"

#include "src/directx/shader/shader_register_id.h"
#include "src/directx/vertex.h"
#include "src/game/game_device.h"
#include "src/util/loader/glb_loader.h"

namespace legend {
namespace draw {

//コンストラクタ
Model::Model()
    : position_(math::Vector3::kZeroVector),
      rotation_(math::Quaternion::kIdentity),
      scale_(math::Vector3::kUnitVector) {}

//デストラクタ
Model::~Model() {}

//初期化
bool Model::Init(const std::filesystem::path& path) {
  util::loader::GLBLoader loader;
  if (!loader.Load(path)) {
    MY_LOG(L"%sの読み込みに失敗しました", path.c_str());
  }

  this->model_name_ = path.filename().replace_extension();
  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();

  const u32 vertex_num = loader.GetVertexNum();
  std::vector<directx::Vertex> vertices(vertex_num);
  if (!vertex_buffer_.Init(device, sizeof(directx::Vertex), vertex_num,
                           model_name_ + L"_VertexBuffer")) {
    return false;
  }

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

  vertex_buffer_.WriteBufferResource(vertices);

  //インデックス配列
  const std::vector<u16> indices = loader.GetIndex();
  if (!index_buffer_.InitAndWrite(device, indices,
                                  directx::PrimitiveTopology::TriangleList,
                                  model_name_ + L"_IndexBuffer")) {
    return false;
  }

  //アルベドテクスチャ
  const std::vector<u8> albedo = loader.GetAlbedo();
  if (!albedo_.InitAndWrite(device, directx::shader::TextureRegisterID::Albedo,
                            albedo, model_name_ + L"_Albedo")) {
    return false;
  }

  if (!transform_constant_buffer_.Init(
          device, directx::shader::ConstantBufferRegisterID::Transform,
          model_name_ + L"_TransformConstantBuffer")) {
    return false;
  }
  transform_constant_buffer_.GetStagingRef().world =
      math::Matrix4x4::CreateScale(scale_) * rotation_.ToMatrix() *
      math::Matrix4x4::CreateTranslate(position_);
  transform_constant_buffer_.UpdateStaging();

  return true;
}

//描画する
void Model::Draw() {
  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();

  transform_constant_buffer_.GetStagingRef().world =
      math::Matrix4x4::CreateScale(scale_) * rotation_.ToMatrix() *
      math::Matrix4x4::CreateTranslate(position_);
  transform_constant_buffer_.UpdateStaging();
  transform_constant_buffer_.SetToHeap(device);
  albedo_.SetToHeap(device);
  device.GetHeapManager().CopyHeapAndSetToGraphicsCommandList(device);

  vertex_buffer_.SetGraphicsCommandList(device);
  index_buffer_.SetGraphicsCommandList(device);
  index_buffer_.Draw(device);
}

}  // namespace draw
}  // namespace legend
#include "src/draw/sprite_2d.h"

#include "src/directx/vertex.h"
#include "src/game/game_device.h"
#include "src/util/loader/texture_loader.h"
#include "src/util/path.h"

namespace legend {
namespace draw {
//コンストラクタ
Sprite2D::Sprite2D() {}
//デストラクタ
Sprite2D::~Sprite2D() {}
//初期化
bool Sprite2D::Initialize(const std::wstring file_name,
                          const math::IntVector2 window_size) {
  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();

  std::filesystem::path texture_path =
      util::Path::GetInstance()->texture() / file_name;
  util::loader::texture_loader::LoadedTextureData data =
      util::loader::texture_loader::Load(texture_path);

  std::vector<directx::Sprite> sprites = {
      {math::Vector3(-1, 1, 0), math::Vector2(0, 0)},
      {math::Vector3(1, 1, 0), math::Vector2(1, 0)},
      {math::Vector3(1, -1, 0), math::Vector2(1, 1)},
      {math::Vector3(-1, -1, 0), math::Vector2(0, 1)}};

  const u32 vertex_size = sizeof(directx::Sprite);
  const u32 vertex_num = static_cast<u32>(sprites.size());
  if (!sprite_.vertex_buffer_.Init(device, vertex_size, vertex_num,
                                   L"Sprite_VertexBuffer")) {
    return false;
  }
  if (!sprite_.vertex_buffer_.WriteBufferResource(sprites)) {
    return false;
  }

  const u32 index_num = sizeof(data.pixels.size());
  if (!sprite_.index_buffer_.Init(device, index_num,
                                  directx::PrimitiveTopology::TriangleList,
                                  L"Sprite_IndexBuffer")) {
    return false;
  }

  std::vector<u16> indexes = {0, 1, 3, 1, 2, 3};
  if (!sprite_.index_buffer_.WriteBufferResource(indexes)) {
    return false;
  }

  if (!sprite_.transform_constant_buffer_.Init(device, 0,
                                               L"Matrix ConstantBuffer")) {
    return false;
  }

  if (!world_constant_buffer_.Init(device, 1, L"World Context")) {
    return false;
  }

  //ウィンドウサイズ
  math::Vector2 size;
  size.x = static_cast<float>(window_size.x);
  size.y = static_cast<float>(window_size.y);

  world_constant_buffer_.GetStagingRef().view = math::Matrix4x4::CreateView(
      math::Vector3(0, 1, -1), math::Vector3(0, 0, 0),
      math::Vector3::kUpVector);
  world_constant_buffer_.GetStagingRef().projection =
      math::Matrix4x4::CreateOrthographic(size);
  world_constant_buffer_.UpdateStaging();

  std::filesystem::path p = util::Path::GetInstance()->texture() / L"tex.png";
  if (!texture_.InitAndWrite(device, 0, p)) {
    return false;
  }

  root_signature_ = std::make_shared<directx::shader::RootSignature>();
  if (!root_signature_->Init(device, L"Global Root Signature")) {
    return false;
  }
  pipeline_state_.SetRootSignature(root_signature_);

  //頂点シェーダー
  std::filesystem::path path = util::Path::GetInstance()->shader();
  std::filesystem::path vertex_shader_path =
      path / L"draw2d" / L"sprite_vs.cso";
  std::filesystem::path pixel_shader_path = path / L"draw2d" / L"sprite_ps.cso";
  std::vector<D3D12_INPUT_ELEMENT_DESC> elements{
      {"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0,
       D3D12_APPEND_ALIGNED_ELEMENT,
       D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
       0},
      {"TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0,
       D3D12_APPEND_ALIGNED_ELEMENT,
       D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
       0},
  };
  std::shared_ptr<directx::shader::VertexShader> vertex_shader =
      std::make_shared<directx::shader::VertexShader>();
  if (!vertex_shader->Init(device, vertex_shader_path, elements)) {
    return false;
  }

  //ピクセルシェーダー
  std::shared_ptr<directx::shader::PixelShader> pixel_shader =
      std::make_shared<directx::shader::PixelShader>();
  if (!pixel_shader->Init(device, pixel_shader_path)) {
    return false;
  }

  if (!pipeline_state_.Init(device)) {
    return false;
  }
  pipeline_state_.SetRootSignature(root_signature_);
  pipeline_state_.SetVertexShader(vertex_shader);
  pipeline_state_.SetPixelShader(pixel_shader);

  if (!pipeline_state_.CreatePipelineState(device)) {
    return false;
  }

  return true;
}
//描画
void Sprite2D::Draw() {
  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();

  root_signature_->SetGraphicsCommandList(device);
  pipeline_state_.SetGraphicsCommandList(device);
  device.GetHeapManager().SetGraphicsCommandList(device);
  texture_.SetToHeap(device);
  world_constant_buffer_.SetToHeap(device);

  sprite_.transform_constant_buffer_.SetToHeap(device);
  device.GetHeapManager().CopyHeapAndSetToGraphicsCommandList(device);

  sprite_.vertex_buffer_.SetGraphicsCommandList(device);
  sprite_.index_buffer_.SetGraphicsCommandList(device);
  sprite_.index_buffer_.Draw(device);
}
}  // namespace draw
}  // namespace legend

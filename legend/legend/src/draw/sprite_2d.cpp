#include "src/draw/sprite_2d.h"

#include "src/directx/shader/alpha_blend_desc.h"
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
                          const math::IntVector2 screen_size,
                          const math::Vector2 position, const float scale) {
  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();

  std::filesystem::path texture_path =
      util::Path::GetInstance()->texture() / file_name;
  util::loader::texture_loader::LoadedTextureData data =
      util::loader::texture_loader::Load(texture_path);

  //画像サイズ
  float texture_width = static_cast<float>(data.width);
  float texture_height = static_cast<float>(data.height);
  //ウィンドウサイズ
  float window_width = static_cast<float>(screen_size.x);
  float window_height = static_cast<float>(screen_size.y);

  //スクリーン座標の設定
  std::vector<directx::Sprite> vertices = {
      {math::Vector3(position.x, position.y, 0), math::Vector2(0, 0)},  // 0
      {math::Vector3(position.x + texture_width, position.y, 0) * scale,
       math::Vector2(1, 0)},  // 1
      {math::Vector3(position.x, position.y + texture_height, 0) * scale,
       math::Vector2(0, 1)},  // 2
      {math::Vector3(position.x + texture_width, position.y + texture_height,
                     0) *
           scale,
       math::Vector2(1, 1)}};  // 3

  const u32 vertex_size = sizeof(directx::Sprite);
  const u32 vertex_num = static_cast<u32>(vertices.size());
  if (!sprite_.vertex_buffer_.Init(device, vertex_size, vertex_num,
                                   L"Sprite_VertexBuffer")) {
    return false;
  }
  if (!sprite_.vertex_buffer_.WriteBufferResource(vertices)) {
    return false;
  }

  const u32 index_num = sizeof(data.pixels.size());
  std::vector<u16> indexes = {0, 1, 2, 1, 3, 2};
  if (!sprite_.index_buffer_.Init(device, index_num,
                                  directx::PrimitiveTopology::TriangleList,
                                  L"Sprite_IndexBuffer")) {
    return false;
  }
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

  world_constant_buffer_.GetStagingRef().projection =
      math::Matrix4x4::CreateOrthographic(
          math::Vector2(window_width, window_height));
  world_constant_buffer_.UpdateStaging();

  if (!texture_.InitAndWrite(device, 0, texture_path)) {
    return false;
  }

  root_signature_ = std::make_shared<directx::shader::RootSignature>();
  if (!root_signature_->Init(device, L"Global Root Signature")) {
    return false;
  }

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
  pipeline_state_.SetRenderTargetInfo(device.GetRenderTarget(), true);
  pipeline_state_.SetBlendDesc(
      directx::shader::alpha_blend_desc::BLEND_DESC_ALIGNMENT, 0);

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

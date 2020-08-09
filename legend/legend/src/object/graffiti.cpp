#include "src/object/graffiti.h"

#include "src/directx/shader/shader_register_id.h"
#include "src/game/game_device.h"

namespace {
DXGI_FORMAT GetFormat(legend::u32 size) {
  switch (size) {
    case 4:
      return DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
    default:
      return DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
  }
}
}  // namespace

namespace legend {
namespace object {

//コンストラクタ
Graffiti::Graffiti() {}

//デストラクタ
Graffiti::~Graffiti() {}

//初期化
bool Graffiti::Init(const GraffitiInitializeParameter& param,
                    directx::device::CommandList& command_list) {
  auto& device = game::GameDevice::GetInstance()->GetDevice();

  //通常のテクスチャをまず作成する
  const D3D12_RESOURCE_DESC tex_resource_desc = CD3DX12_RESOURCE_DESC::Tex2D(
      GetFormat(PIXEL_SIZE), MASK_WIDTH, MASK_HEIGHT);
  if (FAILED(device.GetDevice()->CreateCommittedResource(
          &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT),
          D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &tex_resource_desc,
          D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
          nullptr, IID_PPV_ARGS(&mask_texture_)))) {
    return false;
  }

  this->handle_ = device.GetHeapManager().GetLocalHeap(
      directx::descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID);

  //コピー用のテクスチャを作成する
  const D3D12_RESOURCE_DESC tex_copy_resource_desc =
      CD3DX12_RESOURCE_DESC::Buffer(
          GetRequiredIntermediateSize(mask_texture_.Get(), 0, 1));
  if (FAILED(device.GetDevice()->CreateCommittedResource(
          &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD),
          D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &tex_copy_resource_desc,
          D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
          IID_PPV_ARGS(&mask_texture_copy_)))) {
    return false;
  }

  //テクスチャピクセルの初期化
  for (u32 w = 0; w < MASK_WIDTH; w++) {
    for (u32 h = 0; h < MASK_HEIGHT; h++) {
      const u32 pos = h * MASK_WIDTH * PIXEL_SIZE + w * PIXEL_SIZE;
      pixels_[pos + 0] = 0xff;
      pixels_[pos + 1] = 0xff;
      pixels_[pos + 2] = 0xff;
      pixels_[pos + 3] = 0xff;
    }
  }

  UpdateTexture(command_list);

  D3D12_SHADER_RESOURCE_VIEW_DESC srv_view = {};
  srv_view.Texture2D.MipLevels = 1;
  srv_view.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
  srv_view.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
  srv_view.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2D;
  game::GameDevice::GetInstance()
      ->GetDevice()
      .GetDevice()
      ->CreateShaderResourceView(mask_texture_.Get(), &srv_view,
                                 handle_.cpu_handle_);

  if (!transform_cb_.Init(
          device, directx::shader::ConstantBufferRegisterID::TRANSFORM,
          device.GetHeapManager().GetLocalHeap(
              directx::descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID),
          L"Graffiti_TransformConstantBuffer")) {
    return false;
  }

  transform_.SetPosition(param.position);
  transform_.SetScale(param.scale);
  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();
  return true;
}

//更新
void Graffiti::Update() {
  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();

  //適当な場所の色を変えるテスト
  auto& random = game::GameDevice::GetInstance()->GetRandom();
  const u32 x = random.Range(0u, MASK_WIDTH);
  const u32 y = random.Range(0u, MASK_HEIGHT);
  const float r = random.Range(0.0f, 1.0f);
  const float g = random.Range(0.0f, 1.0f);
  const float b = random.Range(0.0f, 1.0f);
  const float a = random.Range(0.0f, 1.0f);
  SetTextureColor(x, y, util::Color4(r, g, b, a));
}

//描画
void Graffiti::Draw(directx::device::CommandList& command_list) {
  UpdateTexture(command_list);

  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& resource = game::GameDevice::GetInstance()->GetResource();

  resource.GetPipeline()
      .Get(util::resource::id::Pipeline::GRAFFITI)
      ->SetGraphicsCommandList(command_list);

  constexpr u32 MASK_TEXTURE_ID = 1;
  device.GetHeapManager().RegisterHandle(
      MASK_TEXTURE_ID, directx::shader::ResourceType::SRV, handle_);
  transform_cb_.SetToHeap(device);
  resource.GetModel()
      .Get(util::resource::id::Model::OBJECT_1000CM)
      ->Draw(command_list);
}

//テクスチャの色を設定する
void Graffiti::SetTextureColor(u32 x, u32 y, const util::Color4& color) {
  pixels_[y * MASK_WIDTH * PIXEL_SIZE + x * PIXEL_SIZE + 0] =
      static_cast<u8>(color.r * 255.0f);
  pixels_[y * MASK_WIDTH * PIXEL_SIZE + x * PIXEL_SIZE + 1] =
      static_cast<u8>(color.g * 255.0f);
  pixels_[y * MASK_WIDTH * PIXEL_SIZE + x * PIXEL_SIZE + 2] =
      static_cast<u8>(color.b * 255.0f);
  pixels_[y * MASK_WIDTH * PIXEL_SIZE + x * PIXEL_SIZE + 3] =
      static_cast<u8>(color.a * 255.0f);
}
void Graffiti::UpdateTexture(directx::device::CommandList& command_list) {
  command_list.GetCommandList()->ResourceBarrier(
      1, &CD3DX12_RESOURCE_BARRIER::Transition(
             mask_texture_.Get(),
             D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
             D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST));

  constexpr u64 row = MASK_WIDTH * PIXEL_SIZE;
  constexpr u64 slice = row * MASK_HEIGHT;

  //テクスチャのデータを更新する
  D3D12_SUBRESOURCE_DATA subresource_data = {};
  subresource_data.pData = pixels_.data();
  subresource_data.RowPitch = row;
  subresource_data.SlicePitch = slice;
  UpdateSubresources(command_list.GetCommandList(), mask_texture_.Get(),
                     mask_texture_copy_.Get(), 0, 0, 1, &subresource_data);
  command_list.GetCommandList()->ResourceBarrier(
      1,
      &CD3DX12_RESOURCE_BARRIER::Transition(
          mask_texture_.Get(),
          D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST,
          D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
}
}  // namespace object
}  // namespace legend

#include "src/skill/skill_paste.h"

#include "src/directx/shader/shader_register_id.h"
#include "src/game/game_device.h"
#include "src/util/resource/resource_names.h"

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
namespace skill {
namespace resource_name = util::resource::resource_names;

//コンストラクタ
SkillPaste::SkillPaste() : Parent(L"SkillPaste") {}

//デストラクタ
SkillPaste::~SkillPaste() {}

//初期化
void SkillPaste::Init(math::Vector3 position, actor::IActorMediator* mediator) {
  if (!Parent::Init(mediator)) {
    return;
  }

  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& command_list = device.GetCurrentFrameResource()->GetCommandList();

  //通常のテクスチャをまず作成する
  const D3D12_RESOURCE_DESC tex_resource_desc = CD3DX12_RESOURCE_DESC::Tex2D(
      GetFormat(PIXEL_SIZE), MASK_WIDTH, MASK_HEIGHT);
  if (FAILED(device.GetDevice()->CreateCommittedResource(
          &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT),
          D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &tex_resource_desc,
          D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
          nullptr, IID_PPV_ARGS(&mask_texture_)))) {
    return;
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
    return;
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
          device,
          device.GetHeapManager().GetLocalHeap(
              directx::descriptor_heap::heap_parameter::LocalHeapID::GLOBAL_ID),
          L"Paste_TransformConstantBuffer")) {
    return;
  }

  float y = game::GameDevice::GetInstance()->GetRandom().Range(-180.0f, 180.0f);
  math::Quaternion rotate =
      math::Quaternion::FromEular(0, y * math::util::DEG_2_RAD, 0);

  transform_.SetPosition(position);
  transform_.SetRotation(rotate);
  transform_.SetScale(math::Vector3::kUnitVector);

  bullet::BoundingBox::InitializeParameter parameter;
  parameter.position = transform_.GetPosition();
  parameter.rotation = transform_.GetRotation();
  parameter.scale = math::Vector3(3.25f, 0.25f, 3.25f);
  parameter.mass = 0.0f;
  box_ = std::make_shared<bullet::BoundingBox>(this, parameter);
  box_->SetFlags(box_->GetFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
  mediator->AddCollider(box_);

  auto& resource = game::GameDevice::GetInstance()->GetResource();

  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();
}

//更新
bool SkillPaste::Update() { return true; }

//描画
void SkillPaste::Draw() {
  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& command_list = device.GetCurrentFrameResource()->GetCommandList();
  auto& resource = game::GameDevice::GetInstance()->GetResource();

  UpdateTexture(command_list);

  resource.GetPipeline()
      .Get(util::resource::resource_names::pipeline::GRAFFITI)
      ->SetCommandList(command_list);

  constexpr u32 MASK_TEXTURE_ID = 1;
  device.GetHeapManager().RegisterHandle(
      MASK_TEXTURE_ID, directx::shader::ResourceType::SRV, handle_);
  transform_cb_.RegisterHandle(
      device, directx::shader::ConstantBufferRegisterID::TRANSFORM);
  resource.GetModel()
      .Get(util::resource::resource_names::model::GLUE_PLANE)
      ->Draw(command_list);
}

//削除
void SkillPaste::Destroy(actor::IActorMediator* mediator) {
  mediator->RemoveCollider(box_);
  mediator->RemoveActor(this);
}

void SkillPaste::UpdateTexture(directx::device::CommandList& command_list) {
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
}  // namespace skill
}  // namespace legend
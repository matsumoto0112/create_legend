#include "src/object/graffiti.h"

#include "src/directx/shader/shader_register_id.h"
#include "src/enemy/enemy.h"
#include "src/game/game_device.h"
#include "src/player/player.h"
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
namespace object {
namespace resource_name = util::resource::resource_names;

//コンストラクタ
Graffiti::Graffiti() : Parent(L"Graffiti") {}

//デストラクタ
Graffiti::~Graffiti() {}

//初期化
bool Graffiti::Init(actor::IActorMediator* mediator,
                    const GraffitiInitializeParameter& parameter,
                    directx::device::CommandList& command_list) {
  if (!Parent::Init(mediator)) {
    return false;
  }

  auto& resource = game::GameDevice::GetInstance()->GetResource();
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

  //モデルの指定
  switch (parameter.model_id) {
    case 0:
      model_ = resource.GetModel().Get(
          util::resource::resource_names::model::GRAFFITI_01);
      break;

    case 1:
      model_ = resource.GetModel().Get(
          util::resource::resource_names::model::GRAFFITI_02);
      break;
    case 2:
      model_ = resource.GetModel().Get(
          util::resource::resource_names::model::GRAFFITI_03);
      break;
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
          L"Graffiti_TransformConstantBuffer")) {
    return false;
  }

  this->transform_ = parameter.transform;

  //コライダーの設定
  bullet::BoundingBox::InitializeParameter params;
  params.position = this->transform_.GetPosition();
  params.rotation = this->transform_.GetRotation();
  params.scale = parameter.bounding_box_length;
  params.mass = 0.0f;
  params.friction = 0.8f;
  params.restitution = 1.0f;
  box_ = std::make_shared<bullet::BoundingBox>(this, params);
  box_->SetCollisionCallBack([&](bullet::Collider* other) { OnHit(other); });
  mediator_->AddCollider(box_);
  box_->SetFlags(box_->GetFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);

  remaining_graffiti_ = parameter.remaining_graffiti;
  can_erase_speed_ = parameter.can_erase_speed;
  is_erase_ = false;
  is_hit_ = false;
  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();

  delete_time_.Init(5.0f);
  return true;
}

//更新
bool Graffiti::Update() {
  update_time_ =
      game::GameDevice::GetInstance()->GetFPSCounter().GetDeltaSeconds<float>();

  if (is_hit_) {
    mediator_->AddFragment(InstanceFragment());
    is_hit_ = false;
  }

  if (is_erase_) {
    if (delete_time_.Update()) {
      if (box_) {
        mediator_->RemoveCollider(box_);
      }
      mediator_->RemoveActor(this);
    }
    return true;
  }

  return true;
}

//描画
void Graffiti::Draw() {
  if (is_erase_) return;

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
  model_->Draw(command_list);
}

float Graffiti::GetRemainingGraffiti() const { return remaining_graffiti_; }

bool Graffiti::GetIsErase() const { return is_erase_; }

bool Graffiti::GetIsHit() const { return is_hit_; }

void Graffiti::OnHit(bullet::Collider* other) {
  if (is_erase_) return;
  is_erase_ = remaining_graffiti_ <= 0.0f;
  auto& audio = game::GameDevice::GetInstance()->GetAudioManager();

  {
    player::Player* player = dynamic_cast<player::Player*>(other->GetOwner());
    if (player) {
      if (player->GetVelocity().Magnitude() >= can_erase_speed_) {
        const float percentage = 100.0f * update_time_;
        remaining_graffiti_ -= percentage;
        float a = math::util::Abs(fmodf(remaining_graffiti_, 10.0f));
        if (a < 1.0f || a > 9.5f) {
          //弱体化
          player->UpdateStrength(-0.04f);
          is_hit_ = true;
          SetInstancePosition(
              player->GetTransform().GetPosition(),
              player->GetCollider()->GetVelocity().Normalized());
          audio.Start(resource_name::audio::ERASE_GRAFFITI, 0.8f);
        }
      }
    }
  }
  {
    enemy::Enemy* enemy = dynamic_cast<enemy::Enemy*>(other->GetOwner());
    if (enemy) {
      if (enemy->GetVelocity().Magnitude() >= can_erase_speed_) {
        const float percentage = 100.0f * update_time_;
        remaining_graffiti_ -= percentage;
        float a = math::util::Abs(fmodf(remaining_graffiti_, 10.0f));
        if (a < 1.0f || a > 9.5f) {
          //弱体化
          enemy->UpdateStrength(-0.04f);
          is_hit_ = true;
          SetInstancePosition(enemy->GetTransform().GetPosition(),
                              enemy->GetCollider()->GetVelocity().Normalized());
          audio.Start(resource_name::audio::ERASE_GRAFFITI, 0.8f);
        }
      }
    }
  }
}

std::unique_ptr<Fragment> Graffiti::InstanceFragment() {
  float rotate_y =
      game::GameDevice::GetInstance()->GetRandom().Range(-180.0f, 180.0f);

  Fragment::InitializeParameter parameter;
  parameter.position =
      math::Vector3(instance_position_.x, 1.0f, instance_position_.z);
  parameter.rotation =
      math::Quaternion::FromEular(0, rotate_y * math::util::DEG_2_RAD, 0);
  parameter.scale = math::Vector3::kUnitVector;
  parameter.bounding_box_length = math::Vector3(0.6f, 0.3f, 0.3f);

  is_hit_ = false;
  std::unique_ptr<Fragment> f = std::make_unique<Fragment>();
  f->Init(mediator_, parameter);
  return (f);
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
void Graffiti::SetInstancePosition(math::Vector3 position,
                                   math::Vector3 velocity) {
  instance_position_ = position + (-3.5f * (velocity));
  float theta =
      mediator_->GetMainCameraThetaAngle() + math::util::DEG_2_RAD * 90.0f;
  float position_x =
      game::GameDevice::GetInstance()->GetRandom().Range(-3.0f, 3.0f);
  math::Vector3 pos(position_x, 0, -1.0f);

  instance_position_ += math::Matrix4x4::MultiplyCoord(
      pos, math::Matrix4x4::CreateRotationY(-theta));
}

}  // namespace object
}  // namespace legend

#include "src/physics/plane.h"

#include "src/directx/shader/alpha_blend_desc.h"
#include "src/directx/shader/shader_register_id.h"
#include "src/util/path.h"
#include "src/util/resource/pixel_shader.h"
#include "src/util/resource/vertex_shader.h"

namespace legend {
namespace physics {
//コンストラクタ
Plane::Plane()
    : position_(math::Vector3::kZeroVector), normal_(math::Vector3::kUpVector) {
  right_ = (normal_ == math::Vector3::kRightVector ||
            normal_ == math::Vector3::kLeftVector)
               ? 0
               : position_.x + 1;
  left_ = (normal_ == math::Vector3::kRightVector ||
           normal_ == math::Vector3::kLeftVector)
              ? 0
              : position_.x - 1;
  forward_ = (normal_ == math::Vector3::kForwardVector ||
              normal_ == math::Vector3::kBackwardVector)
                 ? 0
                 : position_.z - 1;
  back_ = (normal_ == math::Vector3::kForwardVector ||
           normal_ == math::Vector3::kBackwardVector)
              ? 0
              : position_.z + 1;
  up_ = (normal_ == math::Vector3::kUpVector ||
         normal_ == math::Vector3::kDownVector)
            ? 0
            : position_.y + 1;
  down_ = (normal_ == math::Vector3::kUpVector ||
           normal_ == math::Vector3::kDownVector)
              ? 0
              : position_.y - 1;
}

Plane::Plane(math::Vector3 position, math::Vector3 normal)
    : position_(position), normal_(normal) {
  right_ = (normal_ == math::Vector3::kRightVector ||
            normal_ == math::Vector3::kLeftVector)
               ? 0
               : position_.x + 1;
  left_ = (normal_ == math::Vector3::kRightVector ||
           normal_ == math::Vector3::kLeftVector)
              ? 0
              : position_.x - 1;
  forward_ = (normal_ == math::Vector3::kForwardVector ||
              normal_ == math::Vector3::kBackwardVector)
                 ? 0
                 : position_.z - 1;
  back_ = (normal_ == math::Vector3::kForwardVector ||
           normal_ == math::Vector3::kBackwardVector)
              ? 0
              : position_.z + 1;
  up_ = (normal_ == math::Vector3::kUpVector ||
         normal_ == math::Vector3::kDownVector)
            ? 0
            : position_.y + 1;
  down_ = (normal_ == math::Vector3::kUpVector ||
           normal_ == math::Vector3::kDownVector)
              ? 0
              : position_.y - 1;
}

Plane::~Plane() {
  util::resource::Resource& resource =
      game::GameDevice::GetInstance()->GetResource();
  resource.GetVertexShader().Unload(
      util::resource::id::VertexShader::MODEL_VIEW);
  resource.GetPixelShader().Unload(util::resource::id::PixelShader::MODEL_VIEW);
  resource.GetPipeline().Unload(util::resource::id::Pipeline::MODEL_VIEW);
  resource.GetModel().Unload(util::resource::ModelID::OBJECT_1000CM);
}

//初期化
bool Plane::Initialize(directx::DirectX12Device& device,
                       util::resource::Resource& resource) {
  //モデルデータを読み込む
  const std::filesystem::path model_path =
      util::Path::GetInstance()->model() / "desk.glb";
  if (!resource.GetModel().Load(util::resource::ModelID::OBJECT_1000CM,
                                model_path)) {
    return false;
  }

  auto gps = std::make_shared<directx::shader::GraphicsPipelineState>();
  gps->Init(device);
  gps->SetVertexShader(resource.GetVertexShader().Get(
      util::resource::id::VertexShader::MODEL_VIEW));
  gps->SetPixelShader(resource.GetPixelShader().Get(
      util::resource::id::PixelShader::MODEL_VIEW));
  gps->SetBlendDesc(directx::shader::alpha_blend_desc::BLEND_DESC_ALIGNMENT, 0);
  device.GetRenderResourceManager().WriteRenderTargetInfoToPipeline(
      device, directx::render_target::RenderTargetID::BACK_BUFFER, gps.get());
  device.GetRenderResourceManager().WriteDepthStencilTargetInfoToPipeline(
      device, directx::render_target::DepthStencilTargetID::Depth, gps.get());
  gps->SetRootSignature(device.GetDefaultRootSignature());
  gps->SetPrimitiveTopology(
      D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
  gps->CreatePipelineState(device);
  resource.GetPipeline().Register(util::resource::id::Pipeline::MODEL_VIEW,
                                  gps);

  //トランスフォームバッファを作成する
  if (!transform_cb_.Init(
          device, directx::shader::ConstantBufferRegisterID::Transform,
          device.GetLocalHeapHandle(directx::descriptor_heap::heap_parameter::
                                        LocalHeapID::MODEL_VIEW_SCENE),
          L"Transform ConstantBuffer")) {
    return false;
  }

  transform_cb_.GetStagingRef().world =
      math::Matrix4x4::CreateTranslate(GetPosition());
  transform_cb_.UpdateStaging();

  return true;
}

//描画
void Plane::Draw(directx::DirectX12Device& device) {
  device.GetRenderResourceManager().SetDepthStencilTargetID(
      directx::render_target::DepthStencilTargetID::Depth);
  device.GetRenderResourceManager().SetRenderTargetsToCommandList(device);
  device.GetRenderResourceManager().ClearCurrentDepthStencilTarget(device);

  game::GameDevice::GetInstance()
      ->GetResource()
      .GetPipeline()
      .Get(util::resource::id::Pipeline::MODEL_VIEW)
      ->SetGraphicsCommandList(device);
  transform_cb_.SetToHeap(device);
  game::GameDevice::GetInstance()
      ->GetResource()
      .GetModel()
      .Get(util::resource::ModelID::OBJECT_1000CM)
      ->Draw();
}

//位置の設定
void Plane::SetPosition(math::Vector3 position) { position_ = position; }

//法線ベクトルの設定
void Plane::SetNormal(math::Vector3 normal) { normal_ = normal; }

//位置の取得
math::Vector3 Plane::GetPosition() const { return position_; }

//法線ベクトルの取得
math::Vector3 Plane::GetNormal() const { return normal_; }

//距離を取得
float Plane::GetDistance(math::Vector3 position) const {
  return math::util::Abs(math::Vector3::Dot(position, GetNormal()));
}
}  // namespace physics
}  // namespace legend
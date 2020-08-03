#include "src/object/desk.h"

#include "src/directx/shader/shader_register_id.h"

namespace legend {
namespace object {

//コンストラク
Desk::Desk() : transform_() {
  obb_ = physics::BoundingBox();
  obb_.SetLength(2, 0.5f, 0.5f);
}

//コンストラク
Desk::Desk(math::Vector3 position, math::Quaternion rotation,
           math::Vector3 scale)
    : transform_() {
  SetPosition(position);
  SetRotation(rotation);
  SetScale(scale);
  obb_ = physics::BoundingBox(position, rotation, scale);
  obb_.SetLength(2, 0.5f, 0.5f);
}

//デストラクタ
Desk::~Desk() {}

//初期化
bool Desk::Initialize(directx::DirectX12Device& device,
                      util::resource::Resource& resource) {
  //モデルデータを読み込む
  const std::filesystem::path model_path =
      util::Path::GetInstance()->model() / "desk.glb";
  if (!resource.GetModel().Load(util::resource::ModelID::DESK, model_path)) {
    return false;
  }

  //トランスフォームバッファを作成する
  if (!transform_cb_.Init(
          device, directx::shader::ConstantBufferRegisterID::Transform,
          device.GetLocalHeapHandle(directx::descriptor_heap::heap_parameter::
                                        LocalHeapID::PLAYER_MOVE_VIEWER),
          L"Transform ConstantBuffer")) {
    return false;
  }

  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();

  return true;
}

//更新
bool Desk::Update() { return true; }

//描画
void Desk::Draw(directx::DirectX12Device& device) {
  transform_cb_.SetToHeap(device);
  game::GameDevice::GetInstance()
      ->GetResource()
      .GetModel()
      .Get(util::resource::ModelID::DESK)
      ->Draw();
}

//座標の設定
void Desk::SetPosition(math::Vector3 position) {
  transform_.SetPosition(position);
  obb_.SetPosition(position);
}

//回転の設定
void Desk::SetRotation(math::Quaternion rotation) {
  transform_.SetRotation(rotation);
  obb_.SetRotation(rotation);
}

//スケールの設定
void Desk::SetScale(math::Vector3 scale) { transform_.SetScale(scale); }

//座標の取得
math::Vector3 Desk::GetPosition() { return transform_.GetPosition(); }

//回転の取得
math::Quaternion Desk::GetRotation() { return transform_.GetRotation(); }

//スケールの取得
math::Vector3 Desk::GetScale() { return transform_.GetScale(); }

//直方体の取得
physics::BoundingBox& Desk::GetOBB() {
  physics::BoundingBox& obb = obb_;
  return obb;
}
}  // namespace object
}  // namespace legend
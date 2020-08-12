#include "src/object/desk.h"

#include "src/directx/shader/shader_register_id.h"

namespace legend {
namespace object {

//コンストラク
Desk::Desk() : Parent(L"Desk") {}

//デストラクタ
Desk::~Desk() {}

bool Desk::Init(const InitializeParameter& parameter) {
  if (!Parent::InitBuffer()) {
    return false;
  }

  this->transform_ = parameter.transform;
  this->collision_.SetPosition(transform_.GetPosition());
  this->collision_.SetRotation(transform_.GetRotation());
  this->collision_.SetScale(transform_.GetScale());
  this->collision_.SetLength(parameter.bounding_box_length);
  SetNormal(parameter.normal);

  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& resource = game::GameDevice::GetInstance()->GetResource();

  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();

  model_ = resource.GetModel().Get(util::resource::id::Model::DESK);
  return true;
}

//更新
bool Desk::Update() { return true; }

//座標の設定
void Desk::SetPosition(math::Vector3 position) {
  transform_.SetPosition(position);
  collision_.SetPosition(position);
}

//回転の設定
void Desk::SetRotation(math::Quaternion rotation) {
  transform_.SetRotation(rotation);
  collision_.SetRotation(rotation);
}

//スケールの設定
void Desk::SetScale(math::Vector3 scale) { transform_.SetScale(scale); }

//法線ベクトルの設定
void Desk::SetNormal(math::Vector3 normal) { normal_ = normal; }

//座標の取得
math::Vector3 Desk::GetPosition() { return transform_.GetPosition(); }

//回転の取得
math::Quaternion Desk::GetRotation() { return transform_.GetRotation(); }

//スケールの取得
math::Vector3 Desk::GetScale() { return transform_.GetScale(); }

//法線ベクトルの取得
math::Vector3 Desk::GetNormal() { return normal_; }

}  // namespace object
}  // namespace legend
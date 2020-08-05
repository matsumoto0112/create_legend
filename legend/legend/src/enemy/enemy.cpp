#include "src/enemy/enemy.h"

#include "src/directx/shader/alpha_blend_desc.h"
#include "src/directx/shader/shader_register_id.h"
#include "src/util/path.h"
#include "src/util/resource/pixel_shader.h"
#include "src/util/resource/vertex_shader.h"

namespace legend {
namespace enemy {
//コンストラクタ
Enemy::Enemy() : transform_(), velocity_(math::Vector3::kZeroVector) {
  transform_.SetScale(math::Vector3(8, 6, 7.5f));
  obb_ = physics::BoundingBox();
  obb_.SetLength(1, 0.5f, 2);
  is_move_ = false;
  deceleration_x_ = deceleration_z_ = 0;
}

Enemy::Enemy(math::Vector3 position, math::Quaternion rotation,
             math::Vector3 scale)
    : transform_(), velocity_(math::Vector3::kZeroVector) {
  transform_.SetPosition(position);
  transform_.SetRotation(rotation);
  transform_.SetScale(scale);
  obb_ = physics::BoundingBox(position, rotation, scale);
  obb_.SetLength(1, 1, 2);
  is_move_ = false;
  deceleration_x_ = deceleration_z_ = 0;
}

//デストラクタ
Enemy::~Enemy() {
}

//初期化
bool Enemy::Initilaize(directx::DirectX12Device& device,
                       util::resource::Resource& resource) {
  // if (!obb_.Initialize(device)) {
  //  return false;
  //}

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
bool Enemy::Update() {
  //obb_.Update();

  update_time_ =
      game::GameDevice::GetInstance()->GetFPSCounter().GetDeltaSeconds<float>();

  is_move_ = (0.01f < velocity_.Magnitude());
  Move();

  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();

  return true;
}

//描画
void Enemy::Draw(directx::DirectX12Device& device) {
  // obb_.Draw(device);

  transform_cb_.SetToHeap(device);
  game::GameDevice::GetInstance()
      ->GetResource()
      .GetModel()
      .Get(util::resource::ModelID::ERASER)
      ->Draw();
}

//移動
void Enemy::Move() {
  if (!is_move_) return;

  //移動距離を求める
   float length =
      math::util::Sqrt(velocity_.x * velocity_.x + velocity_.z * velocity_.z);

  //実際に動く距離
   float x = -velocity_.x / length;
   float z = -velocity_.z / length;

  //減速計算
   deceleration_x_ = x / (length * length);
   deceleration_z_ = z / (length * length);

  //移動処理
   math::Vector3 v = math::Vector3(x, 0, z);
   math::Vector3 position = GetPosition() + v * power_ * update_time_;
   SetPosition(position);

   Deceleration(2);
}

void Enemy::SetPosition(math::Vector3 position) {
  transform_.SetPosition(position);
  obb_.SetPosition(position);
}

//速度の設定
void Enemy::SetVelocity(math::Vector3 velocity) { velocity_ = velocity; }

void Enemy::SetRotation() {
   math::Quaternion rotation = transform_.GetRotation();
   rotation.y += velocity_.x;
   transform_.SetRotation(rotation);
   obb_.SetRotation(rotation);
}

//移動に必要なパラメータを初期化
void Enemy::ResetParameter() {
  if (velocity_.Magnitude() != 0.0f) return;

  deceleration_x_ = deceleration_z_ = 0;
  is_move_ = false;
}

//減速
void Enemy::Deceleration(float deceleration_rate) {
   float x = deceleration_x_ * deceleration_rate * update_time_;
   float z = deceleration_z_ * deceleration_rate * update_time_;

   if ((x <= velocity_.x && velocity_.x <= 0) ||
      (0 <= velocity_.x && velocity_.x <= x)) {
    velocity_.x = 0;
  } else {
    velocity_.x -= x;
  }
   if ((z <= velocity_.z && velocity_.z <= 0) ||
      (0 <= velocity_.z && velocity_.z <= z)) {
    velocity_.z = 0;
  } else {
    velocity_.z -= z;
  }
}

//座標の取得
math::Vector3 Enemy::GetPosition() const { return transform_.GetPosition(); }

//移動量の取得
math::Vector3 Enemy::GetVelocity() const { return velocity_; }

math::Quaternion Enemy::GetRotation() const { return transform_.GetRotation(); }

physics::BoundingBox& Enemy::GetOBB() {
  physics::BoundingBox& obb = obb_;
  return obb;
}
}  // namespace enemy
}  // namespace legend
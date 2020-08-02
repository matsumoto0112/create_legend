#include "src/player/player.h"

#include "src/directx/shader/alpha_blend_desc.h"
#include "src/directx/shader/shader_register_id.h"
#include "src/util/path.h"
#include "src/util/resource/pixel_shader.h"
#include "src/util/resource/vertex_shader.h"

namespace legend {
namespace player {
//コンストラクタ
Player::Player()
    : transform_(),
      velocity_(math::Vector3::kZeroVector),
      min_power_(0),
      max_power_(1) {
  transform_.SetScale(math::Vector3::kUnitVector * 20);
  obb_ = physics::BoundingBox();
  obb_.SetLength(1, 1, 2);
  is_move_ = false;
  impulse_ = min_power_;
  deceleration_x_ = deceleration_z_ = 0;
  input_velocity_ = math::Vector3::kZeroVector;
  is_set_power_ = false;
  up_power_ = true;
  is_input_ = false;
  velocity_update_time_ = 0;
}

Player::Player(math::Vector3 position, math::Quaternion rotation,
               math::Vector3 scale, float min_power, float max_power)
    : transform_(),
      velocity_(math::Vector3::kZeroVector),
      min_power_(min_power),
      max_power_(max_power) {
  transform_.SetPosition(position);
  transform_.SetRotation(rotation);
  transform_.SetScale(scale);
  obb_ = physics::BoundingBox(position, rotation, scale);
  obb_.SetLength(1, 1, 2);
  is_move_ = false;
  impulse_ = min_power_;
  deceleration_x_ = deceleration_z_ = 0;
  input_velocity_ = math::Vector3::kZeroVector;
  is_set_power_ = false;
  up_power_ = true;
  is_input_ = false;
  velocity_update_time_ = 0;
}

//デストラクタ
Player::~Player() {
  util::resource::Resource& resource =
      game::GameDevice::GetInstance()->GetResource();
  resource.GetVertexShader().Unload(
      util::resource::id::VertexShader::MODEL_VIEW);
  resource.GetPixelShader().Unload(util::resource::id::PixelShader::MODEL_VIEW);
  resource.GetPipeline().Unload(util::resource::id::Pipeline::MODEL_VIEW);
  resource.GetModel().Unload(util::resource::ModelID::OBJECT_1000CM);
}

//初期化
bool Player::Initilaize(directx::DirectX12Device& device) {
   if (!obb_.Initialize(device)) {
    return false;
  }

  //このシーンで使用するシェーダーを事前に読み込んでおく
  util::resource::Resource& resource =
      game::GameDevice::GetInstance()->GetResource();
  if (!resource.GetVertexShader().Load(
          util::resource::id::VertexShader::MODEL_VIEW,
          util::Path::GetInstance()->shader() / "modelview" /
              "model_view_vs.cso")) {
    return false;
  }
  if (!resource.GetPixelShader().Load(
          util::resource::id::PixelShader::MODEL_VIEW,
          util::Path::GetInstance()->shader() / "modelview" /
              "model_view_ps.cso")) {
    return false;
  }

  //モデルデータを読み込む
  const std::filesystem::path model_path =
      util::Path::GetInstance()->model() / "eraser_01.glb";
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

  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();

  //カメラの初期化
  {
      const math::Vector3 camera_position = math::Vector3(0, 10, -10);
      const math::Quaternion camera_rotation =
          math::Quaternion::FromEular(math::util::DEG_2_RAD * 45.0f, 0.0f,
          0.0f);
      const math::IntVector2 screen_size =
          game::GameDevice::GetInstance()->GetWindow().GetScreenSize();
      const float aspect_ratio = screen_size.x * 1.0f / screen_size.y;
      if (!camera_.Init(L"MainCamera", camera_position, camera_rotation,
          math::util::DEG_2_RAD * 50.0f, aspect_ratio)) {
          return false;
      }
  }

  return true;
}

//更新
bool Player::Update() {
  obb_.Update();

  update_time_ =
      game::GameDevice::GetInstance()->GetFPSCounter().GetDeltaSeconds<float>();

  if (is_input_ &&
      math::Vector3(change_amount_velocity_ - input_velocity_).Magnitude() >=
          1.0f) {
    is_move_ = true;
  }

  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();

  return true;
}

//描画
void Player::Draw(directx::DirectX12Device& device) {
   obb_.Draw(device);

  device.GetRenderResourceManager().SetDepthStencilTargetID(
      directx::render_target::DepthStencilTargetID::Depth);
  device.GetRenderResourceManager().SetRenderTargetsToCommandList(device);
  device.GetRenderResourceManager().ClearCurrentDepthStencilTarget(device);

  game::GameDevice::GetInstance()
      ->GetResource()
      .GetPipeline()
      .Get(util::resource::id::Pipeline::MODEL_VIEW)
      ->SetGraphicsCommandList(device);
   camera_.RenderStart();
  transform_cb_.SetToHeap(device);
  game::GameDevice::GetInstance()
      ->GetResource()
      .GetModel()
      .Get(util::resource::ModelID::OBJECT_1000CM)
      ->Draw();
}

//移動
void Player::Move() {
  if (!is_move_) return;

  math::Vector3 velocity = math::Vector3::kZeroVector;
  while (stick_velocities_.size() > 0) {
    if (velocity.Magnitude() < stick_velocities_.front().Magnitude()) {
      velocity = stick_velocities_.front();
      stick_velocities_.erase(stick_velocities_.begin());
      continue;
    }
    stick_velocities_.erase(stick_velocities_.begin());
  }
  //移動速度がゼロだったらreturn
  if (velocity == math::Vector3::kZeroVector) {
    ResetParameter();
    return;
  }

  //移動距離を求める
  float length =
      math::util::Sqrt(velocity.x * velocity.x + velocity.z * velocity.z);

  //実際に動く距離
  float x = velocity.x / length;
  float z = velocity.z / length;

  //減速計算
  deceleration_x_ = x / (length * length);
  deceleration_z_ = z / (length * length);

  //移動処理
  math::Vector3 v = math::Vector3(x, 0, z);
  math::Vector3 position =
      GetPosition() + v * impulse_ * power_ * update_time_;
  SetPosition(position);

  Deceleration(2);
}

void Player::SetPosition(math::Vector3 position) {
  transform_.SetPosition(position);
  obb_.SetPosition(position);
}

//速度の設定
void Player::SetVelocity(math::Vector3 velocity) { velocity_ = velocity; }

void Player::SetVelocity() {
  if (is_move_) return;

  input::InputManager& input = game::GameDevice::GetInstance()->GetInput();
  //ゲームパッドが一つだけ接続されている間

  if (input.GetGamepad()->GetCount() != 1) return;
  input_velocity_.x = -input.GetGamepad()->GetStickLeft().x;
  input_velocity_.z = -input.GetGamepad()->GetStickLeft().y;

  velocity_update_time_ += update_time_;
  if (velocity_update_time_ < change_time_) return;

  //左スティックの傾きに合わせて値を入れる
  velocity_.x = input_velocity_.x;
  velocity_.z = input_velocity_.z;
  if (velocity_.Magnitude() >= 0.1f) is_input_ = true;

  change_amount_velocity_ = velocity_;
  stick_velocities_.push_back(velocity_);
  while (stick_velocities_.size() > max_stick_velocity_num_) {
    stick_velocities_.erase(stick_velocities_.begin());
  }
  velocity_update_time_ = 0;
}

//パワーの設定
void Player::SetImpulse() {
  input::InputManager& input = game::GameDevice::GetInstance()->GetInput();
  //ゲームパッドが一つだけ接続されている間
  if (input.GetGamepad()->GetCount() != 1) return;

  //パワー調整を終えたらreturn
  if (is_set_power_ || !is_input_) return;

  //左スティックの傾きが0.1以下かつ、方向入力していたらパワー調整を完了にする
  if (math::util::Abs(input.GetGamepad()->GetStickLeft().Magnitude()) <= 0.1f &&
      is_input_) {
    is_set_power_ = true;
    return;
  }

  //パワー調整処理
  if (up_power_) {
    impulse_ += 0.1f * update_time_ * 2;
    if (impulse_ >= max_power_) {
      impulse_ = max_power_;
      up_power_ = false;
    }
  } else {
    impulse_ -= 0.1f * update_time_ * 2;
    if (impulse_ <= min_power_) {
      impulse_ = min_power_;
      up_power_ = true;
    }
  }
}

//移動に必要なパラメータを初期化
void Player::ResetParameter() {
  if (velocity_.Magnitude() != 0.0f) return;

  is_input_ = false;
  is_set_power_ = false;
  impulse_ = 0;
  up_power_ = true;
  change_amount_velocity_ = math::Vector3::kZeroVector;
  deceleration_x_ = deceleration_z_ = 0;
  input_velocity_ = math::Vector3::kZeroVector;
  stick_velocities_.clear();
  is_move_ = false;
  velocity_update_time_ = 0;
}

//減速
void Player::Deceleration(float deceleration_rate) {
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

  change_amount_velocity_ = velocity_;
  stick_velocities_.push_back(velocity_);
  while (stick_velocities_.size() > max_stick_velocity_num_) {
    stick_velocities_.erase(stick_velocities_.begin());
  }
}

//座標の取得
math::Vector3 Player::GetPosition() const { return transform_.GetPosition(); }

//移動量の取得
math::Vector3 Player::GetVelocity() const { return velocity_; }

math::Quaternion Player::GetRotation() const {
  return transform_.GetRotation();
}

float Player::GetImpulse() const { return impulse_; }

physics::BoundingBox& Player::GetOBB() {
  physics::BoundingBox& obb = obb_;
  return obb;
}
camera::PerspectiveCamera& Player::GetCamera()
{
    camera::PerspectiveCamera& camera = camera_;
    return camera;
}
}  // namespace player
}  // namespace legend
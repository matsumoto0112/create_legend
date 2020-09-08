#include "skill_item_box.h"

#include "src/camera/camera_manager.h"
#include "src/directx/shader/shader_register_id.h"
#include "src/draw/particle/particle_factory.h"
#include "src/game/game_device.h"
#include "src/player/player.h"
#include "src/util/resource/resource_names.h"

namespace legend {
namespace skill {
//コンストラクタ
SkillItemBox::SkillItemBox() : Parent(L"ItemBox") {}

//デストラクタ
SkillItemBox::~SkillItemBox() { move_particle_->Delete(); }

//初期化
bool SkillItemBox::Init(actor::IActorMediator* mediator,
                        const InitializeParameter& parameter,
                        std::shared_ptr<Skill> skill) {
  if (!Parent::Init(mediator)) {
    return false;
  }

  this->transform_ = parameter.transform;
  this->transform_.SetScale(math::Vector3(0.5f, 0.5f, 0.5f));

  //コライダーの設定
  bullet::BoundingBox::InitializeParameter params;
  params.position = transform_.GetPosition();
  params.rotation = transform_.GetRotation();
  params.scale = parameter.bounding_box_length;

  // TODO: ここら辺も初期化パラメータにいれる
  params.mass = 0.0f;
  box_ = std::make_shared<bullet::BoundingBox>(this, params);
  mediator_->AddCollider(box_);
  box_->SetFlags(box_->GetFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);

  delete_time_.Init(1.0f);
  skill_ = skill;
  is_dead_ = false;

  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();

  auto& resource = game::GameDevice::GetInstance()->GetResource();
  model_ = resource.GetModel().Get(
      util::resource::resource_names::model::ITEM_CAPSULE_01);

  auto& device = game::GameDevice::GetInstance()->GetDevice();
  std::shared_ptr<draw::Model> icon_model;
  if (parameter.skill_model_num == 0) {
    icon_model = resource.GetModel().Get(
        util::resource::resource_names::model::ITEM_PLANE_01);
  } else if (parameter.skill_model_num == 1) {
    icon_model = resource.GetModel().Get(
        util::resource::resource_names::model::ITEM_PLANE_02);
  } else {
    //後に変更
    icon_model = resource.GetModel().Get(
        util::resource::resource_names::model::ITEM_PLANE_01);
  }

  skill_icon_model_ = icon_model;
  //トランスフォームバッファを作成する
  if (!skill_icon_transform_cb_.Init(
          device,
          device.GetLocalHandle(
              directx::descriptor_heap::heap_parameter::LocalHeapID::ONE_PLAY),
          name_ + L"_TransformConstantBuffer")) {
    return false;
  }

  velocity_ = math::Vector3::kUpVector;

  move_particle_ = draw::particle::particle_factory::CreatePlayerMoveParticle();
  move_particle_->GetTransform().SetPosition(transform_.GetPosition());
  move_particle_->SetEmitEnable(false);

  return true;
}

//更新
bool SkillItemBox::Update() {
  if (is_dead_) {
    //上空に飛ばす
    util::Transform transform = transform_;
    transform.SetPosition(transform.GetPosition() + velocity_);
    box_->SetTransform(transform);

    auto ParticleUpdate = [&]() {
      const math::Vector3 MOVE_PARTICLE_OFFSET = velocity_.Normalized() * -3;
      const math::Vector3 move_particle_position =
          transform_.GetPosition() + MOVE_PARTICLE_OFFSET;
      move_particle_->GetTransformRef().SetPosition(move_particle_position);
      move_particle_->SetEmitEnable(true);
    };

    ParticleUpdate();

    //一定の高さまで行ったら削除
    if (transform_.GetPosition().y > 50.0f) {
      if (delete_time_.Update()) {
        move_particle_->Delete();
        if (box_) mediator_->RemoveCollider(box_);
        mediator_->RemoveActor(this);
        return true;
      }
    }
  }

  return true;
}
//描画
void SkillItemBox::Draw() {
  //一定の高さまで行ったら描画しない
  if (transform_.GetPosition().y > 100.0f) return;

  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& resource = game::GameDevice::GetInstance()->GetResource();
  auto& command_list = device.GetCurrentFrameResource()->GetCommandList();
  resource.GetPipeline()
      .Get(util::resource::resource_names::pipeline::ALPHA_OBJECT)
      ->SetCommandList(command_list);

  transform_cb_.GetStagingRef().world = transform_.CreateWorldMatrix();
  transform_cb_.UpdateStaging();
  transform_cb_.RegisterHandle(
      device, directx::shader::ConstantBufferRegisterID::TRANSFORM);

  model_->Draw(command_list);
  resource.GetPipeline()
      .Get(util::resource::resource_names::pipeline::ALPHA_OBJECT)
      ->SetCommandList(command_list);

  auto Lookat = [](const math::Vector3& position, const math::Vector3& other) {
    Vector3 z = (other - position).Normalized();
    Vector3 x = math::Vector3::Cross(math::Vector3::kUpVector, z).Normalized();
    Vector3 y = math::Vector3::Cross(z, x).Normalized();

    math::Matrix4x4 m = math::Matrix4x4::kIdentity;
    m.m[0][0] = x.x;
    m.m[0][1] = y.x;
    m.m[0][2] = z.x;
    m.m[1][0] = x.y;
    m.m[1][1] = y.y;
    m.m[1][2] = z.y;
    m.m[2][0] = x.z;
    m.m[2][1] = y.z;
    m.m[2][2] = z.z;

    std::array<float, 4> elem;
    elem[0] = m.m[0][0] - m.m[1][1] - m.m[2][2] + 1.0f;
    elem[1] = -m.m[0][0] + m.m[1][1] - m.m[2][2] + 1.0f;
    elem[2] = -m.m[0][0] - m.m[1][1] + m.m[2][2] + 1.0f;
    elem[3] = m.m[0][0] + m.m[1][1] + m.m[2][2] + 1.0f;

    int biggestIdx = 0;
    for (int i = 0; i < elem.size(); i++) {
      if (elem[i] > elem[biggestIdx]) {
        biggestIdx = i;
      }
    }

    if (elem[biggestIdx] < 0) {
      return math::Quaternion::kIdentity;
    }

    std::array<float, 4> q;
    float v = math::util::Sqrt(elem[biggestIdx]) * 0.5f;
    q[biggestIdx] = v;
    float mult = 0.25f / v;

    switch (biggestIdx) {
      case 0:
        q[1] = (m.m[1][0] + m.m[0][1]) * mult;
        q[2] = (m.m[0][2] + m.m[2][0]) * mult;
        q[3] = (m.m[2][1] - m.m[1][2]) * mult;
        break;
      case 1:
        q[0] = (m.m[1][0] + m.m[0][1]) * mult;
        q[2] = (m.m[2][1] + m.m[1][2]) * mult;
        q[3] = (m.m[0][2] - m.m[2][0]) * mult;
        break;
      case 2:
        q[0] = (m.m[0][2] + m.m[2][0]) * mult;
        q[1] = (m.m[2][1] + m.m[1][2]) * mult;
        q[3] = (m.m[1][0] - m.m[0][1]) * mult;
        break;
      case 3:
        q[0] = (m.m[2][1] - m.m[1][2]) * mult;
        q[1] = (m.m[0][2] - m.m[2][0]) * mult;
        q[2] = (m.m[1][0] - m.m[0][1]) * mult;
        break;
    }

    return math::Quaternion(q[0], q[1], q[2], q[3]);
  };

  auto CreateWorldMatrix = [&]() {
    auto camera_manager = mediator_->GetCameraManager();
    const math::Matrix4x4 scale =
        math::Matrix4x4::CreateScale(transform_.GetScale());
    const math::Vector3 position =
        this->transform_.GetPosition() + math::Vector3(0.0f, 3.0f, 0.0f);
    if (camera_manager->GetCameraMode() ==
        camera::camera_mode::BIRDS_EYE_VIEW) {
      math::Matrix4x4 rotate =
          math::Matrix4x4::CreateRotationZ(90.0f * math::util::DEG_2_RAD) *
          math::Matrix4x4::CreateRotationY(90.0f * math::util::DEG_2_RAD);
      math::Matrix4x4 world = scale * rotate;
      world.m[3][0] = position.x;
      world.m[3][1] = position.y;
      world.m[3][2] = position.z;
      return world;

    } else {
      auto cam = mediator_->GetCameraManager()->GetCurrentCamera();
      math::Matrix4x4 view = cam->GetView();
      math::Vector4 m1 =
          math::Vector4(view.m[0][0], view.m[0][1], view.m[0][2], view.m[0][3]);
      math::Vector4 m2 =
          math::Vector4(view.m[1][0], view.m[1][1], view.m[1][2], view.m[1][3]);
      math::Vector4 m3 =
          math::Vector4(view.m[2][0], view.m[2][1], view.m[2][2], view.m[2][3]);
      math::Vector4 m4 =
          math::Vector4(view.m[3][0], view.m[3][1], view.m[3][2], view.m[3][3]);
      float tx = -math::Vector4::Dot(m1, m4);
      float ty = -math::Vector4::Dot(m2, m4);
      float tz = -math::Vector4::Dot(m3, m4);
      math::Matrix4x4 rotate =
          Lookat(math::Vector3(tx, ty, tz), position).ToMatrix() *
          math::Matrix4x4::CreateRotationY(90.0f * math::util::DEG_2_RAD);

      math::Matrix4x4 world = scale * rotate;
      world.m[3][0] = position.x;
      world.m[3][1] = position.y;
      world.m[3][2] = position.z;
      return world;
    }
  };

  skill_icon_transform_cb_.GetStagingRef().world = CreateWorldMatrix();
  skill_icon_transform_cb_.UpdateStaging();
  skill_icon_transform_cb_.RegisterHandle(
      device, directx::shader::ConstantBufferRegisterID::TRANSFORM);

  skill_icon_model_->Draw(command_list);
}

//死亡判定の変更
void SkillItemBox::ChangeDead() { is_dead_ = true; }

//死亡判定の取得
bool SkillItemBox::GetIsDead() const { return is_dead_; }

//設定されたスキルの取得
std::shared_ptr<Skill> SkillItemBox::GetSkill() const { return skill_; }
}  // namespace skill
}  // namespace legend
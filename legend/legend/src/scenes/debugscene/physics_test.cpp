#include "src/scenes/debugscene/physics_test.h"

#include "src/directx/shader/alpha_blend_desc.h"
#include "src/physics/collision.h"
#include "src/util/path.h"

namespace legend {
namespace scenes {
namespace debugscene {

//コンストラクタ
PhysicsTest::PhysicsTest(ISceneChange* scene_change) : Scene(scene_change) {}

//デストラクタ
PhysicsTest::~PhysicsTest() {}

//初期化
bool PhysicsTest::Initialize() {
  obbs_.resize(obb_num_);
  obbs_[0].SetLength(2, 1, 2);
  obbs_[1].SetLength(1, 2, 1);

  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();

  for (i32 i = 0; i < obb_num_; i++) {
    if (!obbs_[i].Initialize(device)) {
      return false;
    }
  }

  // if (!plane_.Initialize(device)) {
  //  return false;
  //}

  // if (!sphere_.Initialize(device)) {
  //  return false;
  //}

  // if (!ray_.Initialize(device)) {
  //  return false;
  //}

  return true;
}

//更新
bool PhysicsTest::Update() {
  if (ImGui::Begin("Transform")) {
    //直方体1
    math::Vector3 obb1_position = obbs_[0].GetPosition();
    ImGui::SliderFloat3("OBB1_Position", &obb1_position.x, -180.0f, 180.0f);
    math::Vector3 obb1_rotation =
        obbs_[0].GetRotation().ToEular() * math::util::RAD_2_DEG;
    ImGui::SliderFloat3("OBB1_Rotation", &obb1_rotation.x, -180.0f, 180.0f);
    math::Vector3 obb1_scale = obbs_[0].GetScale();
    ImGui::SliderFloat3("OBB1_Scale", &obb1_scale.x, 0.1f, 2.0f);
    obbs_[0].SetPosition(obb1_position);
    obbs_[0].SetRotation(
        math::Quaternion::FromEular(obb1_rotation * math::util::DEG_2_RAD));
    obbs_[0].SetScale(obb1_scale);

    ////球
    // math::Vector3 sphere_position = sphere_.GetPosition();
    // math::Vector3 sphere_rotation =
    //    sphere_.GetRotation().ToEular() * math::util::RAD_2_DEG;
    // float sphere_scale = sphere_.GetScale();
    // ImGui::SliderFloat3("Sphere_Position", &sphere_position.x, -180.0f,
    // 180.0f); ImGui::SliderFloat3("Sphere_Rotation", &sphere_rotation.x,
    // -180.0f, 180.0f); ImGui::SliderFloat("Sphere_Scale", &sphere_scale,
    // 0.1f, 2.0f); sphere_.SetPosition(sphere_position); sphere_.SetRotation(
    //    math::Quaternion::FromEular(sphere_rotation * math::util::DEG_2_RAD));
    // sphere_.SetScale(sphere_scale);

    //直方体2
    math::Vector3 obb2_position = obbs_[1].GetPosition();
    ImGui::SliderFloat3("OBB2_Position", &obb2_position.x, -180.0f, 180.0f);
    math::Vector3 obb2_rotation =
        obbs_[1].GetRotation().ToEular() * math::util::RAD_2_DEG;
    ImGui::SliderFloat3("OBB2_Rotation", &obb2_rotation.x, -180.0f, 180.0f);
    math::Vector3 obb2_scale = obbs_[1].GetScale();
    ImGui::SliderFloat3("OBB2_Scale", &obb2_scale.x, 0.1f, 2.0f);
    obbs_[1].SetPosition(obb2_position);
    obbs_[1].SetRotation(
        math::Quaternion::FromEular(obb2_rotation * math::util::DEG_2_RAD));
    obbs_[1].SetScale(obb2_scale);
  }
  ImGui::End();
  for (i32 i = 0; i < obb_num_; i++) {
    obbs_[i].Update();
  }
  //sphere_.Update();

  if (physics::Collision::GetInstance()->Collision_OBB_OBB(obbs_[0],
                                                           obbs_[1])) {
    MY_LOG(L"直方体1と直方体2が衝突しました");
  }

  // if (physics::Collision::GetInstance()->Collision_OBB_Plane(obbs_[0],
  //                                                           plane_)) {
  //  MY_LOG(L"直方体1と平面が衝突しました");
  //}

  // if (physics::Collision::GetInstance()->Collision_Sphere_Plane(sphere_,
  //                                                              plane_)) {
  //  MY_LOG(L"球と平面が衝突しました");
  //}

  // if (physics::Collision::GetInstance()->Collision_Sphere_OBB(sphere_,
  //                                                            obbs_[0])) {
  //  MY_LOG(L"球と直方体1が衝突しました");
  //}

  // if (physics::Collision::GetInstance()->Collision_Ray_Sphere(ray_, sphere_))
  // {
  //    MY_LOG(L"レイと球が衝突しました");
  //}

  // if (physics::Collision::GetInstance()->Collision_Ray_OBB(ray_, obbs_[0])) {
  //    MY_LOG(L"レイと直方体が衝突しました");
  //}

  return true;
}

//描画
void PhysicsTest::Draw() {
  Scene::Draw();

  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();

  for (i32 i = 0; i < obb_num_; i++) {
    obbs_[i].Draw(device);
  }

  // plane_.Draw(device);
  // sphere_.Draw(device);
  // ray_.Draw(device);
}

//終了
void PhysicsTest::Finalize() {
  game::GameDevice::GetInstance()->GetDevice().WaitForGPU();
}
}  // namespace debugscene
}  // namespace scenes
}  // namespace legend
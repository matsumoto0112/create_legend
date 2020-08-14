#include "src/scenes/debugscene/physics_test.h"

#include "src/directx/shader/alpha_blend_desc.h"
#include "src/physics/collision.h"

namespace legend {
namespace scenes {
namespace debugscene {

//�R���X�g���N�^
PhysicsTest::PhysicsTest(ISceneChange* scene_change) : Scene(scene_change) {}

//�f�X�g���N�^
PhysicsTest::~PhysicsTest() {}

//������
bool PhysicsTest::Initialize() {
  obbs_.resize(obb_num_);
  obbs_[0].SetLength(2, 1, 2);
  // obbs_[1].SetLength(1, 2, 1);

  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& resource = game::GameDevice::GetInstance()->GetResource();

  directx::device::CommandList command_list;
  if (!command_list.Init(
          device, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT)) {
    return false;
  }

  for (i32 i = 0; i < obb_num_; i++) {
    if (!obbs_[i].Init()) {
      return false;
    }
  }

  // if (!plane_.Initialize()) {
  //  return false;
  //}

  // if (!sphere_.Initialize(device)) {
  //  return false;
  //}

  if (!ray_.Initialize()) {
    return false;
  }

  //�J�����̏�����
  {
    const math::Vector3 camera_position = math::Vector3(0, 10, -10);
    const math::Quaternion camera_rotation =
        math::Quaternion::FromEular(math::util::DEG_2_RAD * 45.0f, 0.0f, 0.0f);
    const math::IntVector2 screen_size =
        game::GameDevice::GetInstance()->GetWindow().GetScreenSize();
    const float aspect_ratio = screen_size.x * 1.0f / screen_size.y;
    if (!camera_.Init(L"MainCamera", camera_position, camera_rotation,
                      math::util::DEG_2_RAD * 50.0f, aspect_ratio)) {
      return false;
    }
  }

  command_list.Close();
  device.ExecuteCommandList({command_list});
  device.WaitExecute();
  return true;
}

//�X�V
bool PhysicsTest::Update() {
  if (ImGui::Begin("Camera")) {
    //�J�������W
    math::Vector3 camera_position = camera_.GetPosition();
    ImGui::SliderFloat3("Position", &camera_position.x, -100.0f, 100.0f);
    camera_.SetPosition(camera_position);
    //�J������]�p
    math::Vector3 camera_rotation =
        math::Quaternion::ToEular(camera_.GetRotation()) *
        math::util::RAD_2_DEG;
    ImGui::SliderFloat3("Rotation", &camera_rotation.x, -180.0f, 180.0f);
    camera_.SetRotation(
        math::Quaternion::FromEular(camera_rotation * math::util::DEG_2_RAD));

    //�J�����̏�����x�N�g����ύX����
    if (ImGui::Button("X_UP")) {
      camera_.SetUpVector(math::Vector3::kRightVector);
    }
    if (ImGui::Button("Y_UP")) {
      camera_.SetUpVector(math::Vector3::kUpVector);
    }
    if (ImGui::Button("Z_UP")) {
      camera_.SetUpVector(math::Vector3::kForwardVector);
    }
    float fov = camera_.GetFov() * math::util::RAD_2_DEG;
    ImGui::SliderFloat("FOV", &fov, 0.01f, 90.0f);
    camera_.SetFov(fov * math::util::DEG_2_RAD);
  }
  ImGui::End();
  if (ImGui::Begin("Transform")) {
    //������1
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

    ////��
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

    ////������2
    // math::Vector3 obb2_position = obbs_[1].GetPosition();
    // ImGui::SliderFloat3("OBB2_Position", &obb2_position.x, -180.0f, 180.0f);
    // math::Vector3 obb2_rotation =
    //    obbs_[1].GetRotation().ToEular() * math::util::RAD_2_DEG;
    // ImGui::SliderFloat3("OBB2_Rotation", &obb2_rotation.x, -180.0f, 180.0f);
    // math::Vector3 obb2_scale = obbs_[1].GetScale();
    // ImGui::SliderFloat3("OBB2_Scale", &obb2_scale.x, 0.1f, 2.0f);
    // obbs_[1].SetPosition(obb2_position);
    // obbs_[1].SetRotation(
    //    math::Quaternion::FromEular(obb2_rotation * math::util::DEG_2_RAD));
    // obbs_[1].SetScale(obb2_scale);
  }
  ImGui::End();
  for (i32 i = 0; i < obb_num_; i++) {
    obbs_[i].Update();
  }
  // sphere_.Update();

  // if (physics::Collision::GetInstance()->Collision_OBB_OBB(obbs_[0],
  // obbs_[1],
  //                                                         true, true)) {
  //  MY_LOG(L"������1�ƒ�����2���Փ˂��܂���");
  //}

  // if (physics::Collision::GetInstance()->Collision_OBB_Plane(obbs_[0],
  //                                                           plane_)) {
  //  MY_LOG(L"������1�ƕ��ʂ��Փ˂��܂���");
  //}

  // if (physics::Collision::GetInstance()->Collision_Sphere_Plane(sphere_,
  //                                                              plane_)) {
  //  MY_LOG(L"���ƕ��ʂ��Փ˂��܂���");
  //}

  // if (physics::Collision::GetInstance()->Collision_Sphere_OBB(sphere_,
  //                                                            obbs_[0])) {
  //  MY_LOG(L"���ƒ�����1���Փ˂��܂���");
  //}

  // if (physics::Collision::GetInstance()->Collision_Ray_Sphere(ray_, sphere_))
  // {
  //    MY_LOG(L"���C�Ƌ����Փ˂��܂���");
  //}

  float length = 10.0f;
  if (physics::Collision::GetInstance()->Collision_Ray_OBB(ray_, obbs_[0], &length)) {
    MY_LOG(L"���C�ƒ����̂��Փ˂��܂���");
  }
  if (length <= 2.0f) {
      MY_LOG(L"������2�ȉ�");
  }

  return true;
}

//�`��
void PhysicsTest::Draw() {
  Scene::Draw();

  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& command_list = device.GetCurrentFrameResource()->GetCommandList();
  // device.GetRenderResourceManager().SetRenderTargets(
  //    command_list, directx::render_target::RenderTargetID::BACK_BUFFER,
  //    false, directx::render_target::DepthStencilTargetID::DEPTH_ONLY, true);
  camera_.RenderStart();

  for (i32 i = 0; i < obb_num_; i++) {
    obbs_[i].DebugDraw(command_list);
  }
  ray_.Draw(command_list);

  // plane_.Draw();
  // sphere_.Draw(device);
}

//�I��
void PhysicsTest::Finalize() {
  game::GameDevice::GetInstance()->GetDevice().WaitExecute();
}
}  // namespace debugscene
}  // namespace scenes
}  // namespace legend
#include "src/bullet/physics_field.h"

#include "src/game/game_device.h"

namespace legend {
namespace bullet {

//�R���X�g���N�^
PhysicsField::PhysicsField() {}

//�f�X�g���N�^
PhysicsField::~PhysicsField() {}

//������
bool PhysicsField::Init() {
  //�Փˌ��m���@�̐ݒ�
  std::shared_ptr<btCollisionConfiguration> config =
      std::make_shared<btDefaultCollisionConfiguration>();
  std::shared_ptr<btCollisionDispatcher> dispatcher =
      std::make_shared<btCollisionDispatcher>(config.get());

  //�u���[�h�t�F�[�Y�@�̐ݒ�
  std::shared_ptr<btDbvtBroadphase> broadphase =
      std::make_shared<btDbvtBroadphase>();

  //�S���̃\���o�ݒ�
  std::shared_ptr<btSequentialImpulseConstraintSolver> solver =
      std::make_shared<btSequentialImpulseConstraintSolver>();

  // Bullet�̃��[���h�쐬
  world_ = std::make_shared<btDiscreteDynamicsWorld>(
      dispatcher.get(), broadphase.get(), solver.get(), config.get());

  //�d�͂̐ݒ�
  world_->setGravity(btVector3(0.0f, -9.8f, 0.0f));
  world_->setDebugDrawer(nullptr);

  debug_drawer_ = std::make_shared<directx::BulletDebugDraw>();

  auto& device = game::GameDevice::GetInstance()->GetDevice();
  if (!debug_drawer_->Init(device.GetDevice())) {
    return false;
  }
  debug_drawer_->setDebugMode(btIDebugDraw::DBG_DrawWireframe);

  return true;
}

//�X�V
bool PhysicsField::Update() {
  float delta_time =
      game::GameDevice::GetInstance()->GetFPSCounter().GetDeltaSeconds<float>();

  world_->stepSimulation(delta_time, 1);

  return true;
}

void PhysicsField::DebugDraw() {
  world_->debugDrawWorld();
  // math::Vector3 eye(0.0f, 100.0f, -100.0f);
  // math::Vector3 at(0.0f, 0.0f, 0.0f);
  // math::Vector3 up(0.0f, 1.0f, 0.0f);
  // float aspect =
  //   static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT);
  // float fov = 50.0f * math::util::DEG_2_RAD;
  // float near_z = 0.1f;
  // float far_z = 1000.0f;
  // debug_drawer_->Render(
  //   math::Matrix4x4::CreateView(eye, at, up),
  //   math::Matrix4x4::CreateProjection(fov, aspect, near_z, far_z),
  //   command_list);
}

}  // namespace bullet
}  // namespace legend
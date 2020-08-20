#include "src/bullet/physics_field.h"

#include "src/actor/actor.h"
#include "src/game/game_device.h"

namespace legend {
namespace bullet {

//コンストラクタ
PhysicsField::PhysicsField() {}

//デストラクタ
PhysicsField::~PhysicsField() {}

//初期化
bool PhysicsField::Init() {
  //衝突検知方法の設定
  config_ = std::make_shared<btDefaultCollisionConfiguration>();
  dispatcher_ = std::make_shared<btCollisionDispatcher>(config_.get());

  //ブロードフェーズ法の設定
  broadphase_ = std::make_shared<btDbvtBroadphase>();

  //拘束のソルバ設定
  solver_ = std::make_shared<btSequentialImpulseConstraintSolver>();

  // Bulletのワールド作成
  world_ = std::make_shared<btDiscreteDynamicsWorld>(
      dispatcher_.get(), broadphase_.get(), solver_.get(), config_.get());

  //重力の設定
  world_->setGravity(btVector3(0.0f, -9.8f, 0.0f));
  world_->setDebugDrawer(nullptr);

  debug_drawer_ = std::make_shared<directx::BulletDebugDraw>();

  auto& device = game::GameDevice::GetInstance()->GetDevice();
  if (!debug_drawer_->Init(device.GetDevice())) {
    return false;
  }
  debug_drawer_->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
  world_->setDebugDrawer(debug_drawer_.get());

  return true;
}

//更新
bool PhysicsField::Update() {
  float delta_time =
      game::GameDevice::GetInstance()->GetFPSCounter().GetDeltaSeconds<float>();

  auto check = [&]() {
    for (auto&& col : colliders_) {
      MY_LOG(col->GetOwner()->GetName().c_str());
    }
  };
  check();
  //テスト用
  // for (auto&& collider : colliders_) {
  //    collider->SetAngularVelocity(btVector3(0, 0, 0));
  //}

  if (world_) {
    world_->stepSimulation(delta_time, 1);
  }

  for (auto&& col : colliders_) {
    col->UpdateOwnerTransform();
  }
  // world_->rayTest(btVector3(0, 15, 0), btVector3(0, -5, 0), )

  return true;
}

void PhysicsField::DebugDraw(camera::Camera* rendering_camera) {
  world_->debugDrawWorld();

  auto& device = game::GameDevice::GetInstance()->GetDevice();
  auto& command_list = device.GetCurrentFrameResource()->GetCommandList();
  debug_drawer_->Render(rendering_camera->GetView(),
                        rendering_camera->GetProjection(), command_list);
}

void PhysicsField::AddRigidBody(btRigidBody* rigid_body) {
  world_->addRigidBody(rigid_body);
}

void PhysicsField::AddCollision(std::shared_ptr<Collider> collider) {
  colliders_.emplace_back(collider);
  AddRigidBody(collider->GetRigidBody());
  world_->addAction(collider.get());
}

}  // namespace bullet
}  // namespace legend
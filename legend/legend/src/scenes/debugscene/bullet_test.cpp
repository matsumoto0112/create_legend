#include "bullet_test.h"

#include "src\\stdafx.h"

namespace legend {
namespace scenes {
namespace debugscene {

BulletTest::BulletTest(ISceneChange* scene_change) : Scene(scene_change) {}

BulletTest::~BulletTest() {}

bool BulletTest::Initialize() {
  if (!physics_field_.Init()) {
    return false;
  }

  //âºÇ≈boxÇê∂ê¨
  bullet::BoundingBox::InitializeParameter parameter;
  parameter.mass = 0.0f;
  parameter.position = btVector3(0, -2.0f, 0);
  parameter.rotation = btQuaternion(0.0f, 0.0f, 0.0f);
  parameter.scale = btVector3(10, 1, 10);
  parameter.restitution = 0.8f;

  std::shared_ptr<bullet::BoundingBox> box =
      std::make_shared<bullet::BoundingBox>(parameter);
  physics_field_.AddCollision(box);

  return true;
}

bool BulletTest::Update() {
  if (!physics_field_.Update()) {
    return false;
  }

  if (ImGui::Begin("Bullet Test")) {
      if (ImGui::Button("Add Box")) {
          bullet::BoundingBox::InitializeParameter parameter;
          parameter.mass = 10.0f;
          parameter.position = btVector3(0, 10.0f, 0);
          parameter.rotation = btQuaternion(0.0f, 0.0f, 0.0f);
          parameter.scale = btVector3(1, 1, 1);
          parameter.restitution = 0.8f;

          std::shared_ptr<bullet::BoundingBox> box =
              std::make_shared<bullet::BoundingBox>(parameter);
          physics_field_.AddCollision(box);
      }
      if (ImGui::Button("Add Sphere")) {
          bullet::BoundingSphere::InitializeParameter parameter;
          parameter.mass = 10.0f;
          parameter.position = btVector3(0, 0.0f, 0);
          parameter.rotation = btQuaternion(0.0f, 0.0f, 0.0f);
          parameter.radius = 1.0f;
          parameter.restitution = 0.8f;

          std::shared_ptr<bullet::BoundingSphere> sphere =
              std::make_shared<bullet::BoundingSphere>(parameter);
          physics_field_.AddCollision(sphere);
      }
  }
  ImGui::End();


  return true;
}

void BulletTest::Draw() { physics_field_.DebugDraw(); }

void BulletTest::Finalize() {}

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend
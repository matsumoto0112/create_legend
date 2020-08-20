#include "src/scenes/debugscene/bullet_test.h"

#include "src/camera/perspective_camera.h"
#include "src/game/game_device.h"

namespace {
std::vector<std::shared_ptr<legend::bullet::BoundingBox>> boxes_;
}  // namespace

namespace legend {
namespace scenes {
namespace debugscene {

BulletTest::BulletTest(ISceneChange* scene_change) : Scene(scene_change) {}

BulletTest::~BulletTest() {}

bool BulletTest::Initialize() {
  //if (!physics_field_.Init()) {
  //  return false;
  //}

  ////����box�𐶐�
  //bullet::BoundingBox::InitializeParameter parameter;
  //parameter.mass = 0.0f;
  ////parameter.position = btVector3(0, -2.0f, 0);
  //parameter.scale = btVector3(50, 1, 50);

  ////std::shared_ptr<bullet::BoundingBox> box =
  ////    std::make_shared<bullet::BoundingBox>(parameter);
  ////physics_field_.AddCollision(box);
  //const math::IntVector2 screen_size =
  //    game::GameDevice::GetInstance()->GetWindow().GetScreenSize();
  //const float aspect_ratio = screen_size.x * 1.0f / screen_size.y;

  //camera_ = std::make_unique<camera::PerspectiveCamera>();
  //const math::Quaternion camera_rotation =
  //    math::Quaternion::FromEular(math::util::DEG_2_RAD * 45.0f, 0.0f, 0.0f);
  //if (!camera_->Init(L"MainCamera", math::Vector3(0.0f, 30.0f, -30.0f),
  //                   camera_rotation, math::util::DEG_2_RAD * 50.0f,
  //                   aspect_ratio, math::Vector3::kUpVector, 0.1f, 300.0f)) {
  //  return false;
  //}

  return true;
}

bool BulletTest::Update() {
  //if (!physics_field_.Update()) {
  //  return false;
  //}

  //if (ImGui::Begin("Bullet Test")) {
  //  if (ImGui::Button("Add Box")) {
  //    bullet::BoundingBox::InitializeParameter parameter;
  //    parameter.mass = 10.0f;
  //    //parameter.position = btVector3(0, 0.0f, 0);
  //    // parameter.rotation = btQuaternion(0.0f, 0.0f, 0.0f);
  //    // parameter.scale = btVector3(1, 1, 1);
  //    // parameter.restitution = 0.8f;
  //    // parameter.friction = 0.8f;

  //    //std::shared_ptr<bullet::BoundingBox> box =
  //    //    std::make_shared<bullet::BoundingBox>(parameter);
  //    //// box->ApplyCentralImpulse(btVector3(100, 0, 0));
  //    //physics_field_.AddCollision(box);
  //    //boxes_.emplace_back(box);
  //  }
  //  if (ImGui::Button("Add Sphere")) {
  //    bullet::BoundingSphere::InitializeParameter parameter;
  //    parameter.mass = 10.0f;
  //    //parameter.position = btVector3(0, 0.0f, 0);
  //    // parameter.rotation = btQuaternion(0.0f, 0.0f, 0.0f);
  //    parameter.radius = 10.0f;
  //    parameter.restitution = 0.8f;
  //    // parameter.friction = 0.8f;

  //    //std::shared_ptr<bullet::BoundingSphere> sphere =
  //    //    std::make_shared<bullet::BoundingSphere>(parameter);
  //    //physics_field_.AddCollision(sphere);
  //  }

  //  // for (auto&& box : boxes_) {
  //  //  btTransform tr;
  //  //  box->GetMotionState()->getWorldTransform(tr);
  //  //  const btVector3 pos = tr.getOrigin();
  //  //  ImGui::Text("(%f, %f, %f)", pos.x(), pos.y(), pos.z());
  //  //}
  //}
  //ImGui::End();

  return true;
}

void BulletTest::Draw() {
  camera_->RenderStart();
  physics_field_.DebugDraw(camera_.get());
}

void BulletTest::Finalize() {}

}  // namespace debugscene
}  // namespace scenes
}  // namespace legend
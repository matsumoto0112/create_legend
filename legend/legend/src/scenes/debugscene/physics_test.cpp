#include "src/scenes/debugscene/physics_test.h"
#include "src/physics/collision.h"

namespace legend {
namespace scenes {
namespace debugscene {

//�R���X�g���N�^
PhysicsTest::PhysicsTest(ISceneChange* scene_change) : Scene(scene_change) {}

//������
bool PhysicsTest::Initialize() { return true; }

//�X�V
bool PhysicsTest::Update() { return true; }

//�`��
void PhysicsTest::Draw() { Scene::Draw(); }
}  // namespace debugscene
}  // namespace scenes
}  // namespace legend
#include "src/scenes/debugscene/physics_test.h"
#include "src/physics/collision.h"

namespace legend {
namespace scenes {
namespace debugscene {

//コンストラクタ
PhysicsTest::PhysicsTest(ISceneChange* scene_change) : Scene(scene_change) {}

//初期化
bool PhysicsTest::Initialize() { return true; }

//更新
bool PhysicsTest::Update() { return true; }

//描画
void PhysicsTest::Draw() { Scene::Draw(); }
}  // namespace debugscene
}  // namespace scenes
}  // namespace legend
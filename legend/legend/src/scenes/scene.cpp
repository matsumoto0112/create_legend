#include "src/scenes/scene.h"

namespace legend {
namespace scenes {

//コンストラクタ
Scene::Scene(ISceneChange* scene_change) { 
    scene_change_ = scene_change; 
}

//終了
void Scene::Finalize() {}

//描画
void Scene::Draw() {}

}  // namespace scenes
}  // namespace legend

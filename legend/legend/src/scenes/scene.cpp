#include "src/scenes/scene.h"

namespace legend {
namespace scenes {

//�R���X�g���N�^
Scene::Scene(ISceneChange* scene_change) { 
    scene_change_ = scene_change; 
}

//�I��
void Scene::Finalize() {}

//�`��
void Scene::Draw() {}

}  // namespace scenes
}  // namespace legend

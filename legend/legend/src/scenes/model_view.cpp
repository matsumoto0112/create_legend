#include "src/scenes/model_view.h"

namespace legend {
namespace scenes {
ModelView::ModelView(ISceneChange* scene_change) : Scene(scene_change) {}
void ModelView::Initialize() {}
void ModelView::Update() {}
void ModelView::Draw() { Scene::Draw(); }
}  // namespace scenes
}  // namespace legend

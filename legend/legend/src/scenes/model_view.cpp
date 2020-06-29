#include "src/scenes/model_view.h"

#include "src/directx/vertex.h"
#include "src/game/game_device.h"
#include "src/util/loader/glb_loader.h"
#include "src/util/path.h"

namespace legend {
namespace scenes {
ModelView::ModelView(ISceneChange* scene_change) : Scene(scene_change) {}
void ModelView::Initialize() {
  util::loader::GLBLoader loader;
  std::filesystem::path model_path =
      util::Path::GetInstance()->exe() / L"assets" / L"maru_UV.glb";
  util::loader::LoadedMeshData mesh_data = loader.Load(model_path);
  directx::DirectX12Device& device =
      game::GameDevice::GetInstance()->GetDevice();

  std::vector<directx::Vertex> vertices;
  //mesh_data.positions

  //vertex_buffer_.Init()
}
void ModelView::Update() {}
void ModelView::Draw() { Scene::Draw(); }
}  // namespace scenes
}  // namespace legend

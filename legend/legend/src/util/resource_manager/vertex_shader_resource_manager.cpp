#include "src/util/resource_manager/vertex_shader_resource_manager.h"

#include "src/game/game_device.h"

namespace legend {
namespace util {
namespace resource_manager {

VertexShaderResourceManager::VertexShaderResourceManager() {}
VertexShaderResourceManager::~VertexShaderResourceManager() {}
bool VertexShaderResourceManager::Init() { return false; }
bool VertexShaderResourceManager::Load(VertexShaderID key,
                                       const std::filesystem::path& filepath) {
  auto vs = std::make_shared<directx::shader::VertexShader>();
  if (!vs->Init(game::GameDevice::GetInstance()->GetDevice(), filepath)) {
    return false;
  }

  return true;
}
bool VertexShaderResourceManager::Unload(VertexShaderID key) { return false; }

}  // namespace resource_manager
}  // namespace util
}  // namespace legend
#ifndef LEGEND_UTIL_RESOURCE_MANAGER_VERTEX_SHADER_RESOURCE_MANAGER_H_
#define LEGEND_UTIL_RESOURCE_MANAGER_VERTEX_SHADER_RESOURCE_MANAGER_H_

/**
 * @file vertex_shader_resource_manager.h
 */

#include "src/directx/shader/vertex_shader.h"
#include "src/util/resource_manager/resource_manager.h"

namespace legend {
namespace util {
namespace resource_manager {

enum class VertexShaderID {
  MODEL_VIEW,
  SPRITE,
  MULTI_RENDER_TARGET,
  MULTI_RENDER_TARGET_POST_PROCESS,
  OBB,
};

class VertexShaderResourceManager final
    : public ResourceManager<VertexShaderID,
                             std::shared_ptr<directx::shader::VertexShader>> {
 public:
  VertexShaderResourceManager();
  ~VertexShaderResourceManager();
  bool Init() override;
  bool Load(VertexShaderID key, const std::filesystem::path& filepath) override;
  bool Unload(VertexShaderID key) override;
};

}  // namespace resource_manager
}  // namespace util
}  // namespace legend
#endif  //! LEGEND_UTIL_RESOURCE_MANAGER_VERTEX_SHADER_RESOURCE_MANAGER_H_

#ifndef LEGEND_UTIL_RESOURCE_VERTEX_SHADER_H_
#define LEGEND_UTIL_RESOURCE_VERTEX_SHADER_H_

/**
 * @file vertex_shader.h
 */

#include "src/directx/shader/vertex_shader.h"
#include "src/util/resource/resource_manager.h"

namespace legend {
namespace util {
namespace resource {
enum class VertexShaderID {
  MODEL_VIEW,
  SPRITE,
  MULTI_RENDER_TARGET,
  MULTI_RENDER_TARGET_POST_PROCESS,
  OBB,
};

class VertexShader final
    : public ResourceManager<VertexShaderID,
                             std::shared_ptr<directx::shader::VertexShader>> {
 public:
  VertexShader();
  ~VertexShader();
  bool Init() override;
  bool Load(VertexShaderID key, const std::filesystem::path& filepath) override;
  bool Unload(VertexShaderID key) override;
};
}  // namespace resource
}  // namespace util
}  // namespace legend

#endif  //! LEGEND_UTIL_RESOURCE_VERTEX_SHADER_H_

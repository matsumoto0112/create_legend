#ifndef LEGEND_UTIL_RESOURCE_PIXEL_SHADER_H_
#define LEGEND_UTIL_RESOURCE_PIXEL_SHADER_H_

/**
 * @file pixel_shader.h
 */

#include "src/directx/shader/pixel_shader.h"
#include "src/util/resource/resource_manager.h"

namespace legend {
namespace util {
namespace resource {

enum class PixelShaderID {
  MODEL_VIEW,
  SPRITE,
  MULTI_RENDER_TARGET,
  MULTI_RENDER_TARGET_POST_PROCESS,
  OBB,
};

class PixelShader final
    : public ResourceManager<PixelShaderID,
                             std::shared_ptr<directx::shader::PixelShader>> {
 public:
  PixelShader();
  ~PixelShader();
  bool Init() override;
  bool Load(PixelShaderID key, const std::filesystem::path& filepath) override;
  bool Unload(PixelShaderID key) override;
};

}  // namespace resource
}  // namespace util
}  // namespace legend
#endif  //! LEGEND_UTIL_RESOURCE_PIXEL_SHADER_H_

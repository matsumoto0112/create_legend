#ifndef LEGEND_UTIL_RESOURCE_RESOURCE_H_
#define LEGEND_UTIL_RESOURCE_RESOURCE_H_

/**
 * @file resource.h
 */

#include "src/util/resource/pixel_shader.h"
#include "src/util/resource/vertex_shader.h"

namespace legend {
namespace util {
namespace resource {
class Resource {
 public:
  Resource();
  ~Resource();
  bool Init();
  resource::PixelShader& GetPixelShader() { return pixel_shader_; }
  resource::VertexShader& GetVertexShader() { return vertex_shader_; }

 private:
  resource::PixelShader pixel_shader_;
  resource::VertexShader vertex_shader_;
};
}  // namespace resource
}  // namespace util
}  // namespace legend

#endif  //! LEGEND_UTIL_RESOURCE_RESOURCE_H_

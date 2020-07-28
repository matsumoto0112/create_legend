#ifndef LEGEND_UTIL_RESOURCE_H_
#define LEGEND_UTIL_RESOURCE_H_

/**
 * @file resource.h
 */

#include "src/util/resource_manager/vertex_shader_resource_manager.h"

namespace legend {
namespace util {
class Resource {
 public:
  Resource();
  ~Resource();
  bool Init();
  resource_manager::VertexShaderResourceManager& GetVertexShader() {
    return vertex_shader_;
  }

 private:
  resource_manager::VertexShaderResourceManager vertex_shader_;
};
}  // namespace util
}  // namespace legend

#endif  //! LEGEND_UTIL_RESOURCE_H_

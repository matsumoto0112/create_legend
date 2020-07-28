#include "src/util/resource/resource.h"

namespace legend {
namespace util {
namespace resource {

Resource::Resource() {}

Resource::~Resource() {}
bool Resource::Init() {
  if (!pixel_shader_.Init()) return false;
  if (!vertex_shader_.Init()) return false;
  return true;
}
}  // namespace resource
}  // namespace util
}  // namespace legend
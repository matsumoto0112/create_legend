#include "src/util/resource.h"

namespace legend {
namespace util {

Resource::Resource() {}

Resource::~Resource() {}
bool Resource::Init() {
  if (!vertex_shader_.Init()) return false;
  return true;
}
}  // namespace util
}  // namespace legend
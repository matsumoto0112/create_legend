#ifndef LEGEND_UTIL_RESOURCE_LOADER_H
#define LEGEND_UTIL_RESOURCE_LOADER_H

/**
 * @file loader.h
 * @brief
 */

#include "src/util/resource/file.h"

namespace legend {
namespace util {
namespace resource {
class Loader {
 public:
  Loader();
  ~Loader();

  File* LoadFile(const std::wstring& name);
};

}  // namespace resource
}  // namespace util
}  // namespace legend
#endif  //! LEGEND_UTIL_RESOURCE_LOADER_H

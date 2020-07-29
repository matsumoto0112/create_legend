#ifndef LEGEND_UTIL_RESOURCE_MODEL_H_
#define LEGEND_UTIL_RESOURCE_MODEL_H_

/**
 * @file model.h
 */

#include "src/draw/model.h"
#include "src/util/resource/resource_manager.h"

namespace legend {
namespace util {
namespace resource {

enum class ModelID {
  CHECK_XYZ,
  KARI,
  OBJECT_1000CM,
};

class Model : public ResourceManager<ModelID, std::shared_ptr<draw::Model>> {
 public:
  Model();
  ~Model();
  bool Init() override;
  bool Load(ModelID key, const std::filesystem::path& filepath) override;
  bool Unload(ModelID key) override;
};

}  // namespace resource
}  // namespace util
}  // namespace legend
#endif  //! LEGEND_UTIL_RESOURCE_MODEL_H_

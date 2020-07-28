#ifndef LEGEND_UTIL_RESOURCE_MANAGER_RESOURCEMANAGER_H_
#define LEGEND_UTIL_RESOURCE_MANAGER_RESOURCE_MANAGER_H_

/**
 * @file resource_manager.h
 */
#include "src/util/stl_extend.h"

namespace legend {
namespace util {
namespace resource_manager {
template <typename TKey, typename TResource>
class ResourceManager {
 public:
  virtual ~ResourceManager() = default;
  virtual bool Init() = 0;

  virtual bool Load(TKey key, const std::filesystem::path& filepath) = 0;
  virtual bool Unload(TKey key) = 0;
  virtual bool IsLoaded(TKey key) const { return util::Exist(resources_, key); }
  virtual TResource Get(TKey key) const {
    MY_ASSERTION(IsLoaded(key), L"未読み込みのリソースキーが選択されました。");
    return resources_.at(key);
  }

 protected:
  std::unordered_map<TKey, TResource> resources_;
};

}  // namespace resource_manager
}  // namespace util
}  // namespace legend
#endif  //! LEGEND_UTIL_RESOURCE_MANAGER_RESOURCE_MANAGER_H_

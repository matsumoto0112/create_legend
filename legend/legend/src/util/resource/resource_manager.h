#ifndef LEGEND_UTIL_RESOURCE_RESOURCE_MANAGER_H_
#define LEGEND_UTIL_RESOURCE_RESOURCE_MANAGER_H_

/**
 * @file resource_manager.h
 * @brief リソース管理システム基底クラス定義
 */

#include "src/util/stl_extend.h"

namespace legend {
namespace util {
namespace resource {

/**
 * @class ResourceManager
 * @brief リソース管理システム基底クラス
 * @tparam TResource 管理するリソース
 */
template <typename TResource>
class ResourceManager {
 protected:
  using ResourceMap = std::unordered_map<std::wstring, TResource>;

 public:
  /**
   * @brief デストラクタ
   */
  virtual ~ResourceManager() = default;
  /**
   * @brief 初期化
   * @return 初期化に成功したらtrueを返す
   */
  virtual bool Init();
  /**
   * @brief リソースをロードする
   * @param name リソース名
   * @return 読み込みに成功したらtrueを返す
   */
  virtual bool Load(const std::wstring& name);
  /**
   * @brief リソースを登録する
   * @param name リソース名
   * @param resource 登録するリソース
   */
  virtual bool Register(const std::wstring& name, TResource resource);
  /**
   * @brief リソースを取得する
   * @param name リソース名
   */
  virtual TResource Get(const std::wstring& name);
  /**
   * @brief すでに読み込んでいるかどうか
   * @param name リソース名
   * @return 読み込んでいたらtrueを返す
   */
  virtual bool IsLoaded(const std::wstring& name) const;

 protected:
  //! 読み込んだリソースマップ
  ResourceMap resources_;
};

template <typename TResource>
inline bool ResourceManager<TResource>::Init() {
  return true;
}

template <typename TResource>
inline bool ResourceManager<TResource>::Load(const std::wstring& name) {
  return false;
}

template <typename TResource>
inline bool ResourceManager<TResource>::Register(const std::wstring& name,
                                                 TResource resource) {
  if (IsLoaded(name)) {
    MY_LOG(L"登録済みのリソースを上書きしようとしました。");
    resources_[name] = resource;
    return true;
  }
  resources_.emplace(name, resource);
  return true;
}

template <typename TResource>
inline TResource ResourceManager<TResource>::Get(const std::wstring& name) {
  //読み込まれていたらそのまま返す
  if (IsLoaded(name)) {
    return resources_.at(name);
  }

  //読み込まれていなければ読み込む
  if (Load(name)) {
    return resources_.at(name);
  }
  return TResource();
}

template <typename TResource>
inline bool ResourceManager<TResource>::IsLoaded(
    const std::wstring& name) const {
  return util::Exist(resources_, name);
}

}  // namespace resource
}  // namespace util
}  // namespace legend

#endif  //! LEGEND_UTIL_RESOURCE_RESOURCE_MANAGER_H_
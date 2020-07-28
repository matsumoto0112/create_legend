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
 * @brief リソース管理システム基底クラス
 * @tparam TKey リソースを特定するキー
 * @tparam TResource 管理するリソース
 */
template <typename TKey, typename TResource>
class ResourceManager {
 public:
  /**
   * @brief デストラクタ
   */
  virtual ~ResourceManager() = default;
  /**
   * @brief 初期化
   * @return 初期化に成功したらtrueを返す
   */
  virtual bool Init() = 0;
  /**
   * @brief リソースを読み込む
   * @param key リソースを特定するキー
   * @param filepath ファイルへのパス
   * @return 読み込みに成功したらtrueを返す
   */
  virtual bool Load(TKey key, const std::filesystem::path& filepath) = 0;
  /**
   * @brief リソースを登録する
   * @param key リソースを特定するキー
   * @param resource リソース
   * @return 登録に成功したらtrueを返す
   */
  virtual bool Register(TKey key, TResource resource);
  /**
   * @brief リソースを破棄する
   * @param key リソースを特定するキー
   * @return 破棄に成功したらtrueを返す
   */
  virtual bool Unload(TKey key) = 0;
  /**
   * @brief すでに読み込んでいるかどうか
   * @param key リソースを特定するキー
   * @return 読み込んでいたらtrueを返す
   */
  virtual bool IsLoaded(TKey key) const { return util::Exist(resources_, key); }
  /**
   * @brief リソースを取得する
   * @param key リソースを特定するキー
   */
  virtual TResource Get(TKey key) const {
    MY_ASSERTION(IsLoaded(key), L"未読み込みのリソースキーが選択されました。");
    return resources_.at(key);
  }

 protected:
  //! 読み込んだリソースマップ
  std::unordered_map<TKey, TResource> resources_;
};

//リソースを登録する
template <typename TKey, typename TResource>
inline bool ResourceManager<TKey, TResource>::Register(TKey key,
                                                       TResource resource) {
  MY_ASSERTION(!IsLoaded(key), L"登録済みのキーが再選択されました。");
  resources_.emplace(key, resource);
  return true;
}
}  // namespace resource
}  // namespace util
}  // namespace legend

#endif  //! LEGEND_UTIL_RESOURCE_RESOURCE_MANAGER_H_
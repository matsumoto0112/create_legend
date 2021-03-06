#ifndef LEGEND_UTIL_STL_EXTEND_H_
#define LEGEND_UTIL_STL_EXTEND_H_

/**
 * @file stl_extend.h
 * @brief stlの拡張機能定義
 */
namespace legend {
namespace util {

/**
 * @brief Keyが存在するか判定する
 * @return 存在したらtrueを返す
 */
template <typename TKey, typename TValue>
bool Exist(const std::unordered_map<TKey, TValue>& m, const TKey& key) {
  return m.find(key) != m.end();
}

/**
 * @brief Keyが存在するか判定する
 * @return 存在したらtrueを返す
 */
template <typename TKey, typename TValue>
bool Exist(const std::map<TKey, TValue>& m, const TKey& key) {
  return m.find(key) != m.end();
}

}  // namespace util
}  // namespace legend
#endif  //! LEGEND_UTIL_STL_EXTEND_H_

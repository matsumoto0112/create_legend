#ifndef LEGEND_UTIL_STL_EXTEND_H_
#define LEGEND_UTIL_STL_EXTEND_H_

/**
 * @file stl_extend.h
 * @brief stl�̊g���@�\��`
 */
namespace legend {
namespace util {

/**
 * @brief Key�����݂��邩���肷��
 * @return ���݂�����true��Ԃ�
 */
template <typename TKey, typename TValue>
bool Exist(const std::unordered_map<TKey, TValue>& m, const TKey& key) {
  return m.find(key) != m.end();
}

/**
 * @brief Key�����݂��邩���肷��
 * @return ���݂�����true��Ԃ�
 */
template <typename TKey, typename TValue>
bool Exist(const std::map<TKey, TValue>& m, const TKey& key) {
  return m.find(key) != m.end();
}

}  // namespace util
}  // namespace legend
#endif  //! LEGEND_UTIL_STL_EXTEND_H_

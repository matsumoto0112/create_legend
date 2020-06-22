#ifndef LEGEND_UTIL_STRING_UTIL_H_
#define LEGEND_UTIL_STRING_UTIL_H_

/**
 * @file string_util.h
 * @brief Stringのユーティリティ系定義
 */


namespace dx12_lib {
namespace util {
namespace string_util {

/**
 * @brief 文字列のフォーマット
 */
template <typename... Args>
std::wstring Format(const std::wstring& format, Args... args) {
  size_t len = std::swprintf(nullptr, 0, format.c_str(), args...);
  std::vector<wchar_t> buf(len + 1);
  std::swprintf(&buf[0], len + 1, format.c_str(), args...);
  return std::wstring(&buf[0], &buf[0] + len);
}

}  // namespace string_util
}  // namespace util
}  // namespace dx12_lib
#endif  // !LEGEND_UTIL_STRING_UTIL_H_

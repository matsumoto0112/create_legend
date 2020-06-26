#ifndef LEGEND_UTIL_STRING_UTIL_H_
#define LEGEND_UTIL_STRING_UTIL_H_

/**
 * @file string_util.h
 * @brief Stringのユーティリティ系定義
 */

namespace legend {
namespace util {
namespace string_util {

/**
 * @brief 文字列のフォーマット
 */
template <typename... Args>
inline std::wstring Format(const std::wstring& format, Args... args) {
  size_t len = std::swprintf(nullptr, 0, format.c_str(), args...);
  std::vector<wchar_t> buf(len + 1);
  std::swprintf(&buf[0], len + 1, format.c_str(), args...);
  return std::wstring(&buf[0], &buf[0] + len);
}

/**
 * @brief 指定されたコードページによってstringをwstringに変換する
 * @param s 元文字列
 * @param codepage 指定するコードページ
 * @return 変換された文字列
 */
static inline std::wstring CP_2_Wide(const std::string& s, UINT codepage) {
  const int in_length = (int)s.length();
  const int out_length =
      MultiByteToWideChar(codepage, 0, s.c_str(), in_length, 0, 0);
  std::wstring result(out_length, L'\0');
  if (out_length)
    MultiByteToWideChar(codepage, 0, s.c_str(), in_length, &result[0],
                        out_length);
  return result;
}

/**
 * @brief 指定されたコードページによってwstringをstringに変換する
 * @param s 元文字列
 * @param codepage 指定するコードページ
 * @return 変換された文字列
 */
static inline std::string Wide_2_CP(const std::wstring& s, UINT codepage) {
  const int in_length = (int)s.length();
  const int out_length =
      WideCharToMultiByte(codepage, 0, s.c_str(), in_length, 0, 0, 0, 0);
  std::string result(out_length, '\0');
  if (out_length)
    WideCharToMultiByte(codepage, 0, s.c_str(), in_length, &result[0],
                        out_length, 0, 0);
  return result;
}

/**
 * @brief stringをwstringに変換する
 * @param s 元文字列
 * @return 変換された文字列
 */
static inline std::wstring String_2_WString(const std::string& s) {
  return CP_2_Wide(s, CP_ACP);
}

/**
 * @brief stringをwstringに変換する
 * @param s 元文字列
 * @return 変換された文字列
 */
static inline std::string WString_2_String(const std::wstring& s) {
  return Wide_2_CP(s, CP_ACP);
}

}  // namespace string_util
}  // namespace util
}  // namespace legend
#endif  // !LEGEND_UTIL_STRING_UTIL_H_

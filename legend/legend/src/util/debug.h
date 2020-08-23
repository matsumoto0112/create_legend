#ifndef LEGEND_UTIL_DEBUG_H_
#define LEGEND_UTIL_DEBUG_H_

/**
 * @file debug.h
 * @brief デバッグ関係ユーティリティ関数定義
 */

#include <assert.h>

#include "src/util/string_util.h"

//デバッグ時のみデバッグ関数定義
#if defined(_DEBUG)
#define MY_ASSERTION(expr, format, ...)                                    \
  legend::util::debug::Assertion(expr,                                     \
                                 std::wstring(L"ERROR: %s %d\n") + format, \
                                 __FUNCTIONW__, __LINE__, __VA_ARGS__)
#define MY_LOG(format, ...)                                        \
  legend::util::debug::Log(std::wstring(L"Log %s %d:\n") + format, \
                           __FUNCTIONW__, __LINE__, __VA_ARGS__)
//リリース時には定義しない
#else
#define MY_ASSERTION(expr, format, ...)
#define MY_LOG(format, ...)
#endif

namespace legend {
namespace util {
namespace debug {

//デバッグ機能を有効にするか
#if defined(_DEBUG)
static constexpr bool ENABLE_DEBUG = true;
#else
static constexpr bool ENABLE_DEBUG = false;
#endif

/**
 * @brief アサーション関数
 * @param expr 条件
 * @param format メッセージのフォーマット(printf形式)
 */
template <typename... Args>
void Assertion(bool expr, const std::wstring& format, Args... args);

/**
 * @brief ログを出力する
 * @param format メッセージのフォーマット(printf形式)
 */
template <typename... Args>
void Log(const std::wstring& format, Args... args);

//アサーション関数
template <typename... Args>
void Assertion(bool expr, const std::wstring& format, Args... args) {
  const std::wstring msg = string_util::Format(format, args...);
  _ASSERT_EXPR(expr, msg.c_str());
}

//ログ出力関数
template <typename... Args>
void Log(const std::wstring& format, Args... args) {
  const std::wstring msg = string_util::Format(format + L"\n", args...);
  OutputDebugStringW(msg.c_str());
}

}  // namespace debug
}  // namespace util
}  // namespace legend
#endif  // !LEGEND_UTIL_DEBUG_H_

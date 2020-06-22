#ifndef LEGEND_UTIL_DEBUG_H_
#define LEGEND_UTIL_DEBUG_H_

/**
 * @file debug.h
 * @brief デバッグ関係ユーティリティ関数定義
 */

#include <assert.h>


namespace dx12_lib {
namespace util {
namespace debug {

/**
 * @brief アサーション関数
 * @param expr 条件
 * @param format メッセージのフォーマット(printf形式)
 */
void Assertion(bool expr, const std::wstring& format, ...);

/**
 * @brief ログを出力する
 * @param format メッセージのフォーマット(printf形式)
 */
void Log(const std::wstring& format, ...);

}  // namespace debug
}  // namespace util
}  // namespace dx12_lib
#endif  // !LEGEND_UTIL_DEBUG_H_

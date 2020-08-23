#ifndef LEGEND_UTIL_DEBUG_H_
#define LEGEND_UTIL_DEBUG_H_

/**
 * @file debug.h
 * @brief �f�o�b�O�֌W���[�e�B���e�B�֐���`
 */

#include <assert.h>

#include "src/util/string_util.h"

//�f�o�b�O���̂݃f�o�b�O�֐���`
#if defined(_DEBUG)
#define MY_ASSERTION(expr, format, ...)                                    \
  legend::util::debug::Assertion(expr,                                     \
                                 std::wstring(L"ERROR: %s %d\n") + format, \
                                 __FUNCTIONW__, __LINE__, __VA_ARGS__)
#define MY_LOG(format, ...)                                        \
  legend::util::debug::Log(std::wstring(L"Log %s %d:\n") + format, \
                           __FUNCTIONW__, __LINE__, __VA_ARGS__)
//�����[�X���ɂ͒�`���Ȃ�
#else
#define MY_ASSERTION(expr, format, ...)
#define MY_LOG(format, ...)
#endif

namespace legend {
namespace util {
namespace debug {

//�f�o�b�O�@�\��L���ɂ��邩
#if defined(_DEBUG)
static constexpr bool ENABLE_DEBUG = true;
#else
static constexpr bool ENABLE_DEBUG = false;
#endif

/**
 * @brief �A�T�[�V�����֐�
 * @param expr ����
 * @param format ���b�Z�[�W�̃t�H�[�}�b�g(printf�`��)
 */
template <typename... Args>
void Assertion(bool expr, const std::wstring& format, Args... args);

/**
 * @brief ���O���o�͂���
 * @param format ���b�Z�[�W�̃t�H�[�}�b�g(printf�`��)
 */
template <typename... Args>
void Log(const std::wstring& format, Args... args);

//�A�T�[�V�����֐�
template <typename... Args>
void Assertion(bool expr, const std::wstring& format, Args... args) {
  const std::wstring msg = string_util::Format(format, args...);
  _ASSERT_EXPR(expr, msg.c_str());
}

//���O�o�͊֐�
template <typename... Args>
void Log(const std::wstring& format, Args... args) {
  const std::wstring msg = string_util::Format(format + L"\n", args...);
  OutputDebugStringW(msg.c_str());
}

}  // namespace debug
}  // namespace util
}  // namespace legend
#endif  // !LEGEND_UTIL_DEBUG_H_

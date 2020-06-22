#ifndef LEGEND_UTIL_DEBUG_H_
#define LEGEND_UTIL_DEBUG_H_

/**
 * @file debug.h
 * @brief �f�o�b�O�֌W���[�e�B���e�B�֐���`
 */

#include <assert.h>


namespace dx12_lib {
namespace util {
namespace debug {

/**
 * @brief �A�T�[�V�����֐�
 * @param expr ����
 * @param format ���b�Z�[�W�̃t�H�[�}�b�g(printf�`��)
 */
void Assertion(bool expr, const std::wstring& format, ...);

/**
 * @brief ���O���o�͂���
 * @param format ���b�Z�[�W�̃t�H�[�}�b�g(printf�`��)
 */
void Log(const std::wstring& format, ...);

}  // namespace debug
}  // namespace util
}  // namespace dx12_lib
#endif  // !LEGEND_UTIL_DEBUG_H_

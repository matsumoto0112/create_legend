#ifndef LEGEND_UTIL_ENUM_UTIL_H_
#define LEGEND_UTIL_ENUM_UTIL_H_

/**
 * @file enum_util.h
 * @brief Enum�^�̃��[�e�B���e�B�n��`
 */

#include <type_traits>

namespace dx12_lib {
namespace util {
namespace enum_util {

template <typename Enum, typename T = Enum>
struct provides_bitwise_operators : std::false_type {};

namespace provides_bitwise_operators_detail {
enum class fallback_enum {};

//���S��enum�̃x�[�X�^���擾����
// T��enum�o�Ȃ�������T���A
// enum�ł���Ȃ�A���̃x�[�X�^���擾����
//���ꉻ�ɍۂ��AT��enum�łȂ���Ή���fallback_enum���g�p���邱�ƂŁA�G���[���������i���ۂɂ�fallback_enum�͎g�p����Ȃ��j
template <typename T>
using safe_underlying_t = typename std::conditional_t<
    !std::is_enum_v<T>, T,
    typename std::underlying_type_t<
        typename std::conditional_t<std::is_enum_v<T>, T, fallback_enum>>>;

// T1��T2�������𖞂����Ƃ��̂ݎ��̉��ɐ�������
//������provides_bitwise_operators<T>�ɐ������邱�ƁAT�̃x�[�X�^����v���Ă��邱�Ƃ̓��
//���̉��������̌^��T1����������ꍇ��T1��enum�Ȃ�T1�A����ȊO��T2�ƂȂ�
template <typename T1, typename T2, bool ForceLHS = false>
using result_t = typename std::enable_if_t<
    (provides_bitwise_operators<T1, T2>::value ||
     provides_bitwise_operators<T2, T1>::value) &&
        std::is_same_v<safe_underlying_t<T1>, safe_underlying_t<T2>>,
    typename std::conditional_t<ForceLHS || std::is_enum_v<T1>, T1, T2>>;

/**
 * @brief enum�̌��̌^���擾����
 */
template <typename T>
inline constexpr safe_underlying_t<T> Peel(T flags) {
  return static_cast<safe_underlying_t<T>>(flags);
}

/**
 * @brief �r�b�g�ے�
 */
template <typename T>
inline constexpr result_t<T, T> operator~(T flag) {
  return result_t<T>(~Peel(flag));
}

/**
 * @brief �r�b�g�a���Z
 */
template <typename T1, typename T2>
inline constexpr result_t<T1, T2> operator|(T1 flags1, T2 flags2) {
  return result_t<T1, T2>(Peel(flags1) | Peel(flags2));
}

/**
 * @brief �r�b�g�a���Z
 */
template <typename T1, typename T2>
inline constexpr result_t<T1, T2, true>& operator|=(T1& flags1, T2 flags2) {
  return flags1 = static_cast<T1>(flags1 | flags2);
}

/**
 * @brief �r�b�g�ω��Z
 */
template <typename T1, typename T2>
inline constexpr result_t<T1, T2> operator&(T1 flags1, T2 flags2) {
  return result_t<T1, T2>(Peel(flags1) & Peel(flags2));
}

/**
 * @brief �r�b�g�ω��Z
 */
template <typename T1, typename T2>
inline constexpr result_t<T1, T2, true>& operator&=(T1& flags1, T2 flags2) {
  return flags1 = static_cast<T1>(flags1 & flags2);
}

/**
 * @brief �r�b�g�r���I�_���a
 */
template <typename T1, typename T2>
inline constexpr result_t<T1, T2> operator^(T1 flags1, T2 flags2) {
  return result_t<T1, T2>(Peel(flags1) ^ Peel(flags2));
}

/**
 * @brief �r�b�g�r���I�_���a
 */
template <typename T1, typename T2>
inline constexpr result_t<T1, T2, true>& operator^=(T1& flags1, T2 flags2) {
  return flags1 = static_cast<T1>(flags1 ^ flags2);
}

}  // namespace provides_bitwise_operators_detail

/**
 * @brief �r�b�g�������Ă��邩
 */
template <typename T>
inline constexpr bool is_bitpop(T flags) {
  return provides_bitwise_operators_detail::Peel(flags) > 0;
}

}  // namespace enum_util
}  // namespace util
}  // namespace dx12_lib

using dx12_lib::util::enum_util::provides_bitwise_operators_detail::operator~;
using dx12_lib::util::enum_util::provides_bitwise_operators_detail::operator|;
using dx12_lib::util::enum_util::provides_bitwise_operators_detail::operator&;
using dx12_lib::util::enum_util::provides_bitwise_operators_detail::operator^;
using dx12_lib::util::enum_util::provides_bitwise_operators_detail::operator|=;
using dx12_lib::util::enum_util::provides_bitwise_operators_detail::operator&=;
using dx12_lib::util::enum_util::provides_bitwise_operators_detail::operator^=;
#endif  // !LEGEND_UTIL_ENUM_UTIL_H_

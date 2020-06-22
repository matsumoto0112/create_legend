#ifndef LEGEND_UTIL_ENUM_UTIL_H_
#define LEGEND_UTIL_ENUM_UTIL_H_

/**
 * @file enum_util.h
 * @brief Enum型のユーティリティ系定義
 */

#include <type_traits>

namespace dx12_lib {
namespace util {
namespace enum_util {

template <typename Enum, typename T = Enum>
struct provides_bitwise_operators : std::false_type {};

namespace provides_bitwise_operators_detail {
enum class fallback_enum {};

//安全にenumのベース型を取得する
// Tがenum出なかったらTを、
// enumであるなら、そのベース型を取得する
//特殊化に際し、Tがenumでなければ仮のfallback_enumを使用することで、エラーを回避する（実際にはfallback_enumは使用されない）
template <typename T>
using safe_underlying_t = typename std::conditional_t<
    !std::is_enum_v<T>, T,
    typename std::underlying_type_t<
        typename std::conditional_t<std::is_enum_v<T>, T, fallback_enum>>>;

// T1とT2が条件を満たすときのみ実体化に成功する
//条件はprovides_bitwise_operators<T>に成功すること、Tのベース型が一致していることの二つ
//実体化した時の型はT1を強制する場合かT1がenumならT1、それ以外はT2となる
template <typename T1, typename T2, bool ForceLHS = false>
using result_t = typename std::enable_if_t<
    (provides_bitwise_operators<T1, T2>::value ||
     provides_bitwise_operators<T2, T1>::value) &&
        std::is_same_v<safe_underlying_t<T1>, safe_underlying_t<T2>>,
    typename std::conditional_t<ForceLHS || std::is_enum_v<T1>, T1, T2>>;

/**
 * @brief enumの元の型を取得する
 */
template <typename T>
inline constexpr safe_underlying_t<T> Peel(T flags) {
  return static_cast<safe_underlying_t<T>>(flags);
}

/**
 * @brief ビット否定
 */
template <typename T>
inline constexpr result_t<T, T> operator~(T flag) {
  return result_t<T>(~Peel(flag));
}

/**
 * @brief ビット和演算
 */
template <typename T1, typename T2>
inline constexpr result_t<T1, T2> operator|(T1 flags1, T2 flags2) {
  return result_t<T1, T2>(Peel(flags1) | Peel(flags2));
}

/**
 * @brief ビット和演算
 */
template <typename T1, typename T2>
inline constexpr result_t<T1, T2, true>& operator|=(T1& flags1, T2 flags2) {
  return flags1 = static_cast<T1>(flags1 | flags2);
}

/**
 * @brief ビット積演算
 */
template <typename T1, typename T2>
inline constexpr result_t<T1, T2> operator&(T1 flags1, T2 flags2) {
  return result_t<T1, T2>(Peel(flags1) & Peel(flags2));
}

/**
 * @brief ビット積演算
 */
template <typename T1, typename T2>
inline constexpr result_t<T1, T2, true>& operator&=(T1& flags1, T2 flags2) {
  return flags1 = static_cast<T1>(flags1 & flags2);
}

/**
 * @brief ビット排他的論理和
 */
template <typename T1, typename T2>
inline constexpr result_t<T1, T2> operator^(T1 flags1, T2 flags2) {
  return result_t<T1, T2>(Peel(flags1) ^ Peel(flags2));
}

/**
 * @brief ビット排他的論理和
 */
template <typename T1, typename T2>
inline constexpr result_t<T1, T2, true>& operator^=(T1& flags1, T2 flags2) {
  return flags1 = static_cast<T1>(flags1 ^ flags2);
}

}  // namespace provides_bitwise_operators_detail

/**
 * @brief ビットが立っているか
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

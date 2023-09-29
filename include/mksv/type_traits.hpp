#pragma once

namespace mksv {
namespace traits {

template <typename T, T v>
struct integral_constant {
    static constexpr bool value = v;
    using value_type = T;
    using type = integral_constant;
};

template <bool V>
struct bool_constant : public integral_constant<bool, V> {};

struct true_type : public bool_constant<true> {};

struct false_type : public bool_constant<false> {};

template <typename T, typename U>
struct is_same : public false_type {};

template <typename T>
struct is_same<T, T> : public true_type {};

template <typename T, typename U>
inline constexpr bool is_same_v = is_same<T, U>::value;

} // namespace traits
} // namespace mksv

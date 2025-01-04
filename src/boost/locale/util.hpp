//
// Copyright (c) 2009-2011 Artyom Beilis (Tonkikh)
//
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_LOCALE_UTIL_HPP
#define BOOST_LOCALE_UTIL_HPP

#include <string>

namespace boost {
namespace locale {
namespace util {

template <typename Char> Char* str_end(Char* str)
{
  while (*str)
    ++str;
  return str;
}
#ifdef __cpp_char8_t
template <typename T> struct is_char8_t : std::is_same<T, char8_t> {};
#else
template <typename T> struct is_char8_t : std::false_type {};
#endif

inline constexpr bool is_upper_ascii(const char c)
{
  return 'A' <= c && c <= 'Z';
}

inline constexpr bool is_lower_ascii(const char c)
{
  return 'a' <= c && c <= 'z';
}

inline constexpr bool is_numeric_ascii(const char c)
{
  return '0' <= c && c <= '9';
}
std::string normalize_encoding(nonstd::string_view encoding);
inline bool are_encodings_equal(const std::string& l, const std::string& r)
{
  return normalize_encoding(l) == normalize_encoding(r);
}
} // namespace util
} // namespace locale
} // namespace boost

#endif

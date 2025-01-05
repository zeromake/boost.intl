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

std::locale create_info(const std::locale& in, const std::string& name);
std::string get_system_locale(bool use_utf8_on_windows = false);

inline bool try_to_int(const std::string& s, int& res)
{
  if (s.empty())
    return false;
  errno = 0;
  char* end_char{};
  const auto v = std::strtol(s.c_str(), &end_char, 10);
  if (errno == ERANGE || end_char != s.c_str() + s.size())
    return false;
  if (v < std::numeric_limits<int>::min() ||
      v > std::numeric_limits<int>::max())
    return false;
  res = v;
  return true;
}

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
#ifdef _WIN32
std::wstring toNamespacedPath(const std::wstring& path);
#endif
} // namespace util
} // namespace locale
} // namespace boost

#endif

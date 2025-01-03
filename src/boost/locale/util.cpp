//
// Copyright (c) 2009-2011 Artyom Beilis (Tonkikh)
//
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt
#include "util.hpp"

namespace boost {
namespace locale {
namespace util {
std::string normalize_encoding(const nonstd::string_view encoding)
{
  std::string result;
  result.reserve(encoding.length());
  for (char c : encoding) {
    if (is_lower_ascii(c) || is_numeric_ascii(c))
      result += c;
    else if (is_upper_ascii(c))
      result += char(c - 'A' + 'a');
  }
  return result;
}
} // namespace util
} // namespace locale
} // namespace boost

//
// Copyright (c) 2009-2011 Artyom Beilis (Tonkikh)
//
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_LOCALE_CONFIG_HPP
#define BOOST_LOCALE_CONFIG_HPP

#include <string>

#if defined(NDEBUG)
#  define BOOST_ASSERT(expr) ((void)0)
#  define BOOST_ASSERT_MSG(expr, msg) ((void)0)
#else
#  include <assert.h>
#  define BOOST_ASSERT(expr) assert(expr)
#  define BOOST_ASSERT_MSG(expr, msg) assert((expr) && (msg))
#endif

#if !defined(__c2__) && defined(__has_builtin)
#  if __has_builtin(__builtin_expect)
#    define BOOST_LIKELY(x) __builtin_expect(x, 1)
#    define BOOST_UNLIKELY(x) __builtin_expect(x, 0)
#  endif
#endif

#if defined(__clang__) || defined(__GNUC__)
#  define CPP_STANDARD __cplusplus
#elif defined(_MSC_VER)
#  define CPP_STANDARD _MSVC_LANG
#endif

#if CPP_STANDARD < 201703L
#  include <nonstd/string_view.hpp>
#else
namespace nonstd {
template <typename _CharT, typename _Traits = std::char_traits<_CharT>>
using basic_string_view = std::basic_string_view<_CharT>;
using string_view = std::string_view;
using u16string_view = std::u16string_view;
using u32string_view = std::u32string_view;
using wstring_view = std::wstring_view;
} // namespace nonstd
#endif

#endif

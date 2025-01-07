//
// Copyright (c) 2009-2011 Artyom Beilis (Tonkikh)
//
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_LOCALE_CONFIG_HPP
#define BOOST_LOCALE_CONFIG_HPP

#include <string>
#include <boost/locale/nonstd_string_view.hpp>

#if defined(NDEBUG)
#  define BOOST_ASSERT(expr) ((void)0)
#  define BOOST_ASSERT_MSG(expr, msg) ((void)0)
#else
#  include <cassert>
#  define BOOST_ASSERT(expr) assert(expr)
#  define BOOST_ASSERT_MSG(expr, msg) assert((expr) && (msg))
#endif

#if !defined(__c2__) && defined(__has_builtin)
#  if __has_builtin(__builtin_expect)
#    define BOOST_LIKELY(x) __builtin_expect(x, 1)
#    define BOOST_UNLIKELY(x) __builtin_expect(x, 0)
#  else
#    define BOOST_LIKELY(x) x
#    define BOOST_UNLIKELY(x) x
#  endif
#else
#  define BOOST_LIKELY(x) x
#  define BOOST_UNLIKELY(x) x
#endif


#endif

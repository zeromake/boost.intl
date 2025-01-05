//
// Copyright (c) 2009-2011 Artyom Beilis (Tonkikh)
//
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_LOCALE_FACET_ID_HPP
#define BOOST_LOCALE_FACET_ID_HPP
#include <locale>

namespace boost {
namespace locale {
namespace detail {
template <class Derived> struct facet_id {
  static std::locale::id id;
};
} // namespace detail
} // namespace locale
} // namespace boost

#endif

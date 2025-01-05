//
// Copyright (c) 2023 Alexander Grund
//
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_LOCALE_MESSAGE_DETAIL_HPP
#define BOOST_LOCALE_MESSAGE_DETAIL_HPP

#include <boost/locale/generator.hpp>
#include "data.hpp"
#include <vector>

namespace boost {
namespace locale {
namespace detail {
std::locale install_message_facet(const std::locale& in, char_facet_t type,
                                  const util::locale_data& data,
                                  const std::vector<std::string>& domains,
                                  const std::vector<std::string>& paths);
}
} // namespace locale
} // namespace boost

#endif // BOOST_LOCALE_MESSAGE_DETAIL_HPP

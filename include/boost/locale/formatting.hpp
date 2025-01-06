//
// Copyright (c) 2009-2011 Artyom Beilis (Tonkikh)
// Copyright (c) 2022-2023 Alexander Grund
//
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_LOCALE_FORMATTING_HPP_INCLUDED
#define BOOST_LOCALE_FORMATTING_HPP_INCLUDED

#include <ios>

namespace boost {
namespace locale {
class ios_info {
public:
  ios_info();
  ios_info(const ios_info&);
  ios_info& operator=(const ios_info&);
  ~ios_info();
  static ios_info& get(std::ios_base& ios);
  /// Set special message domain identification
  void domain_id(int);
  /// Get special message domain identification
  int domain_id() const;
  void on_imbue();

private:
  int domain_id_;
};
} // namespace locale
} // namespace boost

#endif // BOOST_LOCALE_FORMATTING_HPP_INCLUDED

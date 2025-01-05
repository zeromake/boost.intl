//
// Copyright (c) 2009-2011 Artyom Beilis (Tonkikh)
//
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include "config.hpp"
#include "formatting.hpp"
#include "ios_prop.hpp"

namespace boost {
namespace locale {
ios_info::ios_info() : domain_id_(0) {}
ios_info::~ios_info() = default;

ios_info::ios_info(const ios_info&) = default;
ios_info& ios_info::operator=(const ios_info&) = default;
void ios_info::domain_id(int id) { domain_id_ = id; }
int ios_info::domain_id() const { return domain_id_; }
ios_info& ios_info::get(std::ios_base& ios)
{
  return impl::ios_prop<ios_info>::get(ios);
}

void ios_info::on_imbue() {}

namespace {
struct initializer {
  initializer() { impl::ios_prop<ios_info>::global_init(); }
} initializer_instance;
} // namespace
} // namespace locale
} // namespace boost

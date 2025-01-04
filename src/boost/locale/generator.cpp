//
// Copyright (c) 2009-2011 Artyom Beilis (Tonkikh)
//
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include "config.hpp"
#include <boost/locale/generator.hpp>
#include "message_detail.hpp"
#include <map>

namespace boost {
namespace locale {
struct generator::data {
  data() : caching_enabled(false), use_ansi_encoding(false) {}
  mutable std::map<std::string, std::locale> cached;
  bool caching_enabled;
  bool use_ansi_encoding;
  std::vector<std::string> paths;
  std::vector<std::string> domains;
  std::map<std::string, std::vector<std::string>> options;
};
generator::generator() : d(std::move(new generator::data())) {}
generator::~generator() = default;

void generator::add_messages_domain(const std::string& domain)
{
  if (std::find(d->domains.begin(), d->domains.end(), domain) ==
      d->domains.end())
    d->domains.push_back(domain);
}

void generator::set_default_messages_domain(const std::string& domain)
{
  const auto p = std::find(d->domains.begin(), d->domains.end(), domain);
  if (p != d->domains.end())
    d->domains.erase(p);
  d->domains.insert(d->domains.begin(), domain);
}

void generator::clear_domains() { d->domains.clear(); }
void generator::add_messages_path(const std::string& path)
{
  d->paths.push_back(path);
}
void generator::clear_paths() { d->paths.clear(); }
void generator::clear_cache() { d->cached.clear(); }
std::locale generator::generate(const std::string& id) const
{
  std::locale base = std::locale::classic();
  return generate(base, id);
}

std::locale generator::generate(const std::locale& base,
                                const std::string& id) const
{
  if (d->caching_enabled) {
    const auto p = d->cached.find(id);
    if (p != d->cached.end())
      return p->second;
  }
  std::locale result = base;
  result = detail::install_message_facet(result, char_facet_t::char_f,
                                         util::locale_data(id), d->domains,
                                         d->paths);
  if (d->caching_enabled) {
    const auto p = d->cached.find(id);
    if (p == d->cached.end())
      d->cached[id] = result;
  }
  return result;
}
bool generator::use_ansi_encoding() const { return d->use_ansi_encoding; }

void generator::use_ansi_encoding(bool v) { d->use_ansi_encoding = v; }

bool generator::locale_cache_enabled() const { return d->caching_enabled; }
void generator::locale_cache_enabled(bool enabled)
{
  d->caching_enabled = enabled;
}

} // namespace locale
} // namespace boost

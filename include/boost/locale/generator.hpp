//
// Copyright (c) 2009-2011 Artyom Beilis (Tonkikh)
//
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_LOCALE_GENERATOR_HPP
#define BOOST_LOCALE_GENERATOR_HPP

#include <string>
#include <locale>
#include <memory>

namespace boost {
namespace locale {
enum class char_facet_t : uint32_t {
  nochar =
      0, ///< Unspecified character category for character independent facets
  char_f = 1 << 0,  ///< 8-bit character facets
  wchar_f = 1 << 1, ///< wide character facets
};
enum class category_t : uint32_t {
  message = 1 << 4, ///< Generate message facets
};
class generator {
public:
  generator();
  ~generator();

  void categories(category_t cats);
  category_t categories() const;
  void characters(char_facet_t chars);
  char_facet_t characters() const;
  void add_messages_domain(const std::string& domain);
  void set_default_messages_domain(const std::string& domain);
  void clear_domains();
  void add_messages_path(const std::string& path);
  void clear_paths();
  void clear_cache();
  void locale_cache_enabled(bool on);
  bool locale_cache_enabled() const;
  bool use_ansi_encoding() const;
  void use_ansi_encoding(bool enc);
  const std::string& lid() const;
  std::locale generate(const std::string& id) const;
  std::locale generate(const std::locale& base, const std::string& id) const;
  std::locale operator()(const std::string& id) const { return generate(id); }
  std::locale operator()(const std::locale& base, const std::string& id) const
  {
    return generate(base, id);
  }

private:
  generator(const generator&);
  void operator=(const generator&);
  void prepare_data() const;
  struct data;
  std::unique_ptr<struct data> d;
};
} // namespace locale
} // namespace boost

#endif // BOOST_LOCALE_GENERATOR_HPP

//
// Copyright (c) 2009-2011 Artyom Beilis (Tonkikh)
//
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_LOCALE_MESSAGE_HPP
#define BOOST_LOCALE_MESSAGE_HPP

#include <string>
#include <locale>
#include <functional>
#include <cstring>
#include <boost/locale/generator.hpp>
#include <boost/locale/facet_id.hpp>
#include <boost/locale/formatting.hpp>
#include <boost/locale/nonstd_string_view.hpp>

namespace boost {
namespace locale {
using count_type = long long;
namespace detail {
inline bool is_us_ascii_char(char c)
{
  // works for null terminated strings regardless char "signedness"
  return 0 < c && c < 0x7F;
}
inline bool is_us_ascii_string(const char* msg)
{
  while (*msg) {
    if (!is_us_ascii_char(*msg++))
      return false;
  }
  return true;
}
template <typename CharType> struct string_cast_traits {
  static const CharType* cast(const CharType* msg,
                              std::basic_string<CharType>& /*unused*/)
  {
    return msg;
  }
};

template <> struct string_cast_traits<char> {
  static const char* cast(const char* msg, std::string& buffer)
  {
    if (is_us_ascii_string(msg))
      return msg;
    buffer.reserve(strlen(msg));
    char c;
    while ((c = *msg++) != 0) {
      if (is_us_ascii_char(c))
        buffer += c;
    }
    return buffer.c_str();
  }
};
} // namespace detail

template <typename CharType>
class message_format : public std::locale::facet,
                       public detail::facet_id<message_format<CharType>> {
public:
  typedef CharType char_type;
  typedef std::basic_string<char_type> string_type;
  using string_view_type = nonstd::basic_string_view<char_type>;

  message_format(size_t refs = 0) : std::locale::facet(refs) {}
  virtual const string_view_type get(int domain_id, const char_type* context,
                               const char_type* id) const = 0;
  virtual const string_view_type get(int domain_id, const char_type* context,
                               const char_type* single_id,
                               count_type n) const = 0;
  virtual int domain(const std::string& domain) const = 0;
  virtual const char_type* convert(const char_type* msg,
                                   string_type& buffer) const = 0;
};

template <typename CharType> class basic_message {
public:
  typedef CharType char_type;
  typedef std::basic_string<char_type> string_type;
  typedef message_format<char_type> facet_type;
  using string_view_type = nonstd::basic_string_view<char_type>;
  basic_message()
      : n_(0), c_id_(nullptr), c_context_(nullptr), c_plural_(nullptr)
  {
  }

  explicit basic_message(const char_type* id)
      : n_(0), c_id_(id), c_context_(nullptr), c_plural_(nullptr)
  {
  }

  explicit basic_message(const char_type* single, const char_type* plural,
                         count_type n)
      : n_(n), c_id_(single), c_context_(nullptr), c_plural_(plural)
  {
  }
  explicit basic_message(const char_type* context, const char_type* id)
      : n_(0), c_id_(id), c_context_(context), c_plural_(nullptr)
  {
  }

  explicit basic_message(const char_type* context, const char_type* single,
                         const char_type* plural, count_type n)
      : n_(n), c_id_(single), c_context_(context), c_plural_(plural)
  {
  }
  explicit basic_message(const string_type& id)
      : n_(0), c_id_(nullptr), c_context_(nullptr), c_plural_(nullptr), id_(id)
  {
  }

  explicit basic_message(const string_type& single, const string_type& plural,
                         count_type number)
      : n_(number),
        c_id_(nullptr),
        c_context_(nullptr),
        c_plural_(nullptr),
        id_(single),
        plural_(plural)
  {
  }

  explicit basic_message(const string_type& context, const string_type& id)
      : n_(0),
        c_id_(nullptr),
        c_context_(nullptr),
        c_plural_(nullptr),
        id_(id),
        context_(context)
  {
  }
  explicit basic_message(const string_type& context, const string_type& single,
                         const string_type& plural, count_type number)
      : n_(number),
        c_id_(nullptr),
        c_context_(nullptr),
        c_plural_(nullptr),
        id_(single),
        context_(context),
        plural_(plural)
  {
  }

  basic_message(const basic_message&) = default;
  basic_message(basic_message&&) noexcept = default;

  basic_message& operator=(const basic_message&) = default;
  basic_message& operator=(basic_message&&) noexcept(
      std::is_nothrow_move_assignable<string_type>::value) = default;

  void swap(basic_message& other) noexcept(
      noexcept(std::declval<string_type&>().swap(std::declval<string_type&>())))
  {
    using std::swap;
    swap(n_, other.n_);
    swap(c_id_, other.c_id_);
    swap(c_context_, other.c_context_);
    swap(c_plural_, other.c_plural_);
    swap(id_, other.id_);
    swap(context_, other.context_);
    swap(plural_, other.plural_);
  }
  friend void swap(basic_message& x,
                   basic_message& y) noexcept(noexcept(x.swap(y)))
  {
    x.swap(y);
  }

  operator string_type() const { return str(); }
  string_type str() const { return str(std::locale()); }
  string_type str(const std::locale& locale) const { return str(locale, 0); }
  string_type str(const std::locale& locale, const std::string& domain_id) const
  {
    int id = 0;
    if (std::has_facet<facet_type>(locale))
      id = std::use_facet<facet_type>(locale).domain(domain_id);
    return str(locale, id);
  }
  string_type str(const std::string& domain_id) const
  {
    return str(std::locale(), domain_id);
  }
  string_type str(const std::locale& loc, int id) const
  {
    string_type buffer;
    const char_type* ptr = write(loc, id, buffer);
    if (ptr != buffer.c_str())
      buffer = ptr;
    return buffer;
  }
  void write(std::basic_ostream<char_type>& out) const
  {
    const std::locale& loc = out.getloc();
    int id = ios_info::get(out).domain_id();
    string_type buffer;
    out << write(loc, id, buffer);
  }

  operator string_view_type() const { return str_view(); }
  string_view_type str_view() const { return str_view(std::locale()); }
  string_view_type str_view(const std::locale& locale) const { return str_view(locale, 0); }
  string_view_type str_view(const std::locale& locale, const std::string& domain_id) const
  {
    int id = 0;
    if (std::has_facet<facet_type>(locale))
      id = std::use_facet<facet_type>(locale).domain(domain_id);
    return str_view(locale, id);
  }
  string_view_type str_view(const std::string& domain_id) const
  {
    return str_view(std::locale(), domain_id);
  }
  string_view_type str_view(const std::locale& loc, int id) const
  {
    return write_view(loc, id);
  }
  void write_view(std::basic_ostream<char_type>& out) const
  {
    const std::locale& loc = out.getloc();
    int id = ios_info::get(out).domain_id();
    out << write_view(loc, id);
  }

private:
  const char_type* plural() const
  {
    if (c_plural_)
      return c_plural_;
    if (plural_.empty())
      return nullptr;
    return plural_.c_str();
  }
  const char_type* context() const
  {
    if (c_context_)
      return c_context_;
    if (context_.empty())
      return nullptr;
    return context_.c_str();
  }

  const char_type* id() const { return c_id_ ? c_id_ : id_.c_str(); }

  static const char_type* get_string_view_data(const string_view_type& sv)
  {
    return sv.empty() ? nullptr : sv.data();
  }

  const string_view_type write_view(const std::locale& loc, int domain_id) const {
    const char_type* id = this->id();
    const char_type* context = this->context();
    const char_type* plural = this->plural();
    if (*id == 0)
      return {};

    const facet_type* facet = nullptr;
    if (std::has_facet<facet_type>(loc))
      facet = &std::use_facet<facet_type>(loc);

    string_view_type translated = {};
    if (facet) {
      if (!plural)
        translated = facet->get(domain_id, context, id);
      else
        translated = facet->get(domain_id, context, id, n_).data();
    }
    if (translated.empty()) {
      const char_type* msg = plural ? (n_ == 1 ? id : plural) : id;
      translated = msg;
    }
    return translated;
  }

  const char_type* write(const std::locale& loc, int domain_id,
                         string_type& buffer) const
  {
    static const char_type empty_string[1] = {0};

    const char_type* id = this->id();
    const char_type* context = this->context();
    const char_type* plural = this->plural();

    if (*id == 0)
      return empty_string;

    const facet_type* facet = nullptr;
    if (std::has_facet<facet_type>(loc))
      facet = &std::use_facet<facet_type>(loc);

    const char_type* translated = nullptr;
    if (facet) {
      if (!plural)
        translated = get_string_view_data(facet->get(domain_id, context, id));
      else
        translated = get_string_view_data(facet->get(domain_id, context, id, n_));
    }

    if (!translated) {
      const char_type* msg = plural ? (n_ == 1 ? id : plural) : id;

      if (facet)
        translated = facet->convert(msg, buffer);
      else
        translated = detail::string_cast_traits<char_type>::cast(msg, buffer);
    }
    return translated;
  }

  count_type n_;
  const char_type* c_id_;
  const char_type* c_context_;
  const char_type* c_plural_;
  string_type id_;
  string_type context_;
  string_type plural_;
};

typedef basic_message<char> message;

template <typename CharType>
std::basic_ostream<CharType>& operator<<(std::basic_ostream<CharType>& out,
                                         const basic_message<CharType>& msg)
{
  msg.write_view(out);
  return out;
}

template <typename CharType>
inline basic_message<CharType> translate(const CharType* msg)
{
  return basic_message<CharType>(msg);
}

template <typename CharType>
inline basic_message<CharType> translate(const CharType* context,
                                         const CharType* msg)
{
  return basic_message<CharType>(context, msg);
}

template <typename CharType>
inline basic_message<CharType> translate(const CharType* single,
                                         const CharType* plural, count_type n)
{
  return basic_message<CharType>(single, plural, n);
}

template <typename CharType>
inline basic_message<CharType> translate(const CharType* context,
                                         const CharType* single,
                                         const CharType* plural, count_type n)
{
  return basic_message<CharType>(context, single, plural, n);
}

template <typename CharType>
inline basic_message<CharType> translate(const std::basic_string<CharType>& msg)
{
  return basic_message<CharType>(msg);
}

template <typename CharType>
inline basic_message<CharType>
translate(const std::basic_string<CharType>& context,
          const std::basic_string<CharType>& msg)
{
  return basic_message<CharType>(context, msg);
}

template <typename CharType>
inline basic_message<CharType>
translate(const std::basic_string<CharType>& context,
          const std::basic_string<CharType>& single,
          const std::basic_string<CharType>& plural, count_type n)
{
  return basic_message<CharType>(context, single, plural, n);
}

template <typename CharType>
inline basic_message<CharType>
translate(const std::basic_string<CharType>& single,
          const std::basic_string<CharType>& plural, count_type n)
{
  return basic_message<CharType>(single, plural, n);
}

template <typename CharType>
std::basic_string<CharType> gettext(const CharType* id,
                                    const std::locale& loc = std::locale())
{
  return basic_message<CharType>(id).str(loc);
}

template <typename CharType>
std::basic_string<CharType> ngettext(const CharType* s, const CharType* p,
                                     count_type n,
                                     const std::locale& loc = std::locale())
{
  return basic_message<CharType>(s, p, n).str(loc);
}

template <typename CharType>
std::basic_string<CharType> dgettext(const char* domain, const CharType* id,
                                     const std::locale& loc = std::locale())
{
  return basic_message<CharType>(id).str(loc, domain);
}

template <typename CharType>
std::basic_string<CharType> dngettext(const char* domain, const CharType* s,
                                      const CharType* p, count_type n,
                                      const std::locale& loc = std::locale())
{
  return basic_message<CharType>(s, p, n).str(loc, domain);
}

template <typename CharType>
std::basic_string<CharType> pgettext(const CharType* context,
                                     const CharType* id,
                                     const std::locale& loc = std::locale())
{
  return basic_message<CharType>(context, id).str(loc);
}

template <typename CharType>
std::basic_string<CharType>
npgettext(const CharType* context, const CharType* s, const CharType* p,
          count_type n, const std::locale& loc = std::locale())
{
  return basic_message<CharType>(context, s, p, n).str(loc);
}

template <typename CharType>
std::basic_string<CharType>
dpgettext(const char* domain, const CharType* context, const CharType* id,
          const std::locale& loc = std::locale())
{
  return basic_message<CharType>(context, id).str(loc, domain);
}

template <typename CharType>
std::basic_string<CharType>
dnpgettext(const char* domain, const CharType* context, const CharType* s,
           const CharType* p, count_type n,
           const std::locale& loc = std::locale())
{
  return basic_message<CharType>(context, s, p, n).str(loc, domain);
}

} // namespace locale
} // namespace boost

#endif

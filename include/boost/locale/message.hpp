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
#include <boost/locale/generator.hpp>
#include <boost/locale/formatting.hpp>

namespace boost {
namespace locale {
using count_type = long long;
namespace detail {
template <class Derived> struct facet_id {
  static std::locale::id id;
};

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

  message_format(size_t refs = 0) : std::locale::facet(refs) {}
  virtual const char_type* get(int domain_id, const char_type* context,
                               const char_type* id) const = 0;
  virtual const char_type* get(int domain_id, const char_type* context,
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
        translated = facet->get(domain_id, context, id);
      else
        translated = facet->get(domain_id, context, id, n_);
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
  msg.write(out);
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
/// Translate plural form according to locale \a loc
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

namespace gnu_gettext {

struct messages_info {
  messages_info() : language("C"), locale_category("LC_MESSAGES") {}
  std::string
      language; ///< The language we load the catalog for, like "ru", "en", "de"
  std::string country; ///< The country we load the catalog for, like "US", "IL"
  std::string variant; ///< Language variant, like "euro" so it would look for
                       ///< catalog like de_DE\@euro
  std::string encoding; ///< Required target charset encoding. Ignored for wide
                        ///< characters. For narrow, should specify the correct
                        ///< encoding required for this catalog
  std::string locale_category; ///< Locale category, is set by default to
                               ///< LC_MESSAGES, but may be changed
                               ///
  /// \brief This type represents GNU Gettext domain name for the messages.
  ///
  /// It consists of two parameters:
  ///
  /// - name - the name of the domain - used for opening the file name
  /// - encoding - the encoding of the keys in the sources, default - UTF-8
  ///
  struct domain {
    std::string name;     ///< The name of the domain
    std::string encoding; ///< The character encoding for the domain
    domain() = default;

    /// Create a domain object from the name that can hold an encoding after
    /// symbol "/" such that if n is "hello/cp1255" then the name would be
    /// "hello" and "encoding" would be "cp1255" and if n is "hello" then the
    /// name would be the same but encoding would be "UTF-8"
    domain(const std::string& n)
    {
      const size_t pos = n.find('/');
      if (pos == std::string::npos) {
        name = n;
        encoding = "UTF-8";
      }
      else {
        name = n.substr(0, pos);
        encoding = n.substr(pos + 1);
      }
    }

    /// Check whether two objects are equivalent, only names are compared,
    /// encoding is ignored
    bool operator==(const domain& other) const { return name == other.name; }
    /// Check whether two objects are distinct, only names are compared,
    /// encoding is ignored
    bool operator!=(const domain& other) const { return !(*this == other); }
  };

  typedef std::vector<domain>
      domains_type;     ///< Type that defines a list of domains that are loaded
                        ///< The first one is the default one
  domains_type domains; ///< Message domains - application name, like my_app. So
                        ///< files named my_app.mo would be loaded
  std::vector<std::string>
      paths; ///< Paths to search files in. Under MS Windows it uses encoding
             ///< parameter to convert them to wide OS specific paths.

  /// The callback for custom file system support. This callback should read the
  /// file named \a file_name encoded in \a encoding character set into
  /// std::vector<char> and return it.
  ///
  /// - If the file does not exist, it should return an empty vector.
  /// - If an error occurs during file read it should throw an exception.
  ///
  /// \note The user should support only the encodings the locales are created
  /// for. So if the user uses only one encoding or the file system is encoding
  /// agnostic, he may ignore the \a encoding parameter.
  typedef std::function<std::vector<char>(const std::string& file_name,
                                          const std::string& encoding)>
      callback_type;

  /// The callback for handling custom file systems, if it is empty, the real OS
  /// file-system is being used.
  callback_type callback;

  /// Get paths to folders which may contain catalog files
  std::vector<std::string> get_catalog_paths() const;

private:
  /// Get a list of folder names for the language, country and variant
  std::vector<std::string> get_lang_folders() const;
};

template <typename CharType>
message_format<CharType>* create_messages_facet(const messages_info& info);
} // namespace gnu_gettext

} // namespace locale
} // namespace boost

#endif

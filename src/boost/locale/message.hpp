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
#include "config.hpp"

namespace boost {
namespace locale {
enum class char_facet_t : uint32_t {
  nochar =
      0, ///< Unspecified character category for character independent facets
  char_f = 1 << 0, ///< 8-bit character facets
                   // wchar_f = 1 << 1, ///< wide character facets
#ifdef __cpp_char8_t
// char8_f = 1 << 2, ///< C++20 char8_t facets
#endif
};
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


#ifndef BOOST_LOCALE_NONSTD_STRING_VIEW_HPP
#define BOOST_LOCALE_NONSTD_STRING_VIEW_HPP 1

#if defined(__clang__) || defined(__GNUC__)
#  define CPP_STANDARD __cplusplus
#elif defined(_MSC_VER)
#  define CPP_STANDARD _MSVC_LANG
#endif

#if CPP_STANDARD < 201703L
#  include <nonstd/string_view.hpp>
#else
#   include <string_view>
namespace nonstd {
template <typename _CharT, typename _Traits = std::char_traits<_CharT>>
using basic_string_view = std::basic_string_view<_CharT>;
using string_view = std::string_view;
using u16string_view = std::u16string_view;
using u32string_view = std::u32string_view;
using wstring_view = std::wstring_view;
} // namespace nonstd
#endif

#endif

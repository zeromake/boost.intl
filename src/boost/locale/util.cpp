//
// Copyright (c) 2009-2011 Artyom Beilis (Tonkikh)
//
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include "config.hpp"
#include "util.hpp"

#include <algorithm>
#include <vector>

#ifdef _WIN32
#  define LOCALE_SISO639LANGNAME 0x00000059
#  define LOCALE_SISO3166CTRYNAME 0x0000005A
#  define LOCALE_IDEFAULTANSICODEPAGE 0x00001004
#  define LOCALE_USER_DEFAULT 0x0400
extern "C" {
__declspec(dllimport) int __stdcall GetLocaleInfoA(unsigned long Locale,
                                                   unsigned long LCType,
                                                   char* lpLCData, int cchData);
__declspec(dllimport) unsigned long __stdcall GetCurrentDirectoryW(
    unsigned long nBufferLength, wchar_t* lpBuffer);
}
template <size_t N>
static bool get_user_default_locale_info(unsigned long lcType, char (&buf)[N])
{
  return ::GetLocaleInfoA(LOCALE_USER_DEFAULT, lcType, buf, N) != 0;
}
#endif

namespace boost {
namespace locale {
namespace util {
std::string normalize_encoding(const nonstd::string_view encoding)
{
  std::string result;
  result.reserve(encoding.length());
  for (char c : encoding) {
    if (is_lower_ascii(c) || is_numeric_ascii(c))
      result += c;
    else if (is_upper_ascii(c))
      result += char(c - 'A' + 'a');
  }
  return result;
}

std::string get_system_locale(bool use_utf8_on_windows)
{
  const char* lang = nullptr;
  if (!lang || !*lang)
    lang = getenv("LC_ALL");
  if (!lang || !*lang)
    lang = getenv("LC_CTYPE");
  if (!lang || !*lang)
    lang = getenv("LANG");
#ifndef _WIN32
  if (!lang || !*lang)
    lang = "C";
  return lang;
#else
  if (lang && *lang)
    return lang;

  char buf[10]{};
  if (!get_user_default_locale_info(LOCALE_SISO639LANGNAME, buf))
    return "C";
  std::string lc_name = buf;
  if (get_user_default_locale_info(LOCALE_SISO3166CTRYNAME, buf)) {
    lc_name += "_";
    lc_name += buf;
  }
  if (use_utf8_on_windows ||
      !get_user_default_locale_info(LOCALE_IDEFAULTANSICODEPAGE, buf))
    lc_name += ".UTF-8";
  else {
    if (std::find_if_not(buf, str_end(buf), is_numeric_ascii) != str_end(buf))
      lc_name += ".UTF-8";
    else
      lc_name.append(".windows-").append(buf);
  }
  return lc_name;

#endif
}

#ifdef _WIN32

static inline bool isAbsolute(const std::wstring path)
{
  return path.size() > 2 &&
         ((path[0] >= L'A' && path[0] <= L'Z') ||
          (path[0] >= L'a' && path[0] <= L'z')) &&
         path[1] == L':';
}

static std::wstring getCurrentPath()
{
  auto size = ::GetCurrentDirectoryW(0, NULL);
  std::vector<wchar_t> buf(size);
  ::GetCurrentDirectoryW(size, buf.data());
  auto currentPath = std::wstring(buf.data());
  while (currentPath.back() == L'\\') {
    currentPath.pop_back();
  }
  return std::move(currentPath);
}

static std::wstring joinPath(const std::wstring& path1, std::wstring path2)
{
  while (!path2.empty() && path2.front() == L'\\') {
    path2 = path2.substr(1);
  }
  if (path2.size() >= 2 && path2[0] == L'.' && path2[1] == L'\\') {
    path2 = path2.substr(2);
  }
  if (path1.empty() || path2.empty()) {
    return std::move(path1 + path2);
  }
  auto result = (!path1.empty() && path1.back() == L'\\')
                    ? path1 + path2
                    : path1 + L"\\" + path2;
  return std::move(result);
}

std::wstring toNamespacedPath(const std::wstring& path)
{
  std::wstring originPath(path);
  if (originPath.size() >= 260) {
    static const std::wstring currentPath = getCurrentPath();
    for (std::wstring::iterator i = originPath.begin(), eoi = originPath.end();
         i != eoi; ++i) {
      if (*i == L'/') {
        *i = L'\\';
      }
    }
    if (!isAbsolute(originPath)) {
      originPath = joinPath(currentPath, originPath);
    }
    originPath = L"\\\\?\\" + originPath;
  }
  else {
    for (std::wstring::iterator i = originPath.begin(), eoi = originPath.end();
         i != eoi; ++i) {
      if (*i == L'/') {
        *i = L'\\';
      }
    }
  }
  return std::move(originPath);
}
#endif
} // namespace util
} // namespace locale
} // namespace boost

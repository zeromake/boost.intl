//
// Copyright (c) 2025 zeromake<a390720046@gmail.com>
//
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_LOCALE_LIBINTL_H
#define BOOST_LOCALE_LIBINTL_H 1

#define __FUNCTION_NAME(name) boost_locale_##name
#ifdef __cplusplus
extern "C" {
#endif
const char* __FUNCTION_NAME(gettext)(const char* msgid);
const char* __FUNCTION_NAME(dgettext)(const char* domain, const char* msgid);
const char* __FUNCTION_NAME(dcgettext)(const char* domain, const char* msgid, int category);
const char* __FUNCTION_NAME(ngettext)(const char* msgid1, const char* msgid2, unsigned long int n);
const char* __FUNCTION_NAME(dngettext)(const char* domain, const char* msgid1, const char* msgid2,
                unsigned long int n);
const char* __FUNCTION_NAME(dcngettext)(const char* domain, const char* msgid1, const char* msgid2,
                 unsigned long int n, int category);
const char* __FUNCTION_NAME(textdomain)(const char* domain);
const char* __FUNCTION_NAME(bindtextdomain)(const char* domain, const char* dirname);
const char* __FUNCTION_NAME(bind_textdomain_codeset)(const char* domain, const char* codeset);
void __FUNCTION_NAME(textdomain_generate)(const char* lid);
#ifdef __cplusplus
};
#endif

#ifndef __DIABLE_COMPATIBLE_GETTEXT
#define gettext(msgid) __FUNCTION_NAME(gettext)(msgid)
#define dgettext(domain, msgid) __FUNCTION_NAME(dgettext)(domain, msgid)
#define dcgettext(domain, msgid, category) __FUNCTION_NAME(dcgettext)(domain, msgid, category)
#define ngettext(msgid1, msgid2, n) __FUNCTION_NAME(ngettext)(msgid1, msgid2, n)
#define dngettext(domain, msgid1, msgid2, n) __FUNCTION_NAME(dngettext)(domain, msgid1, msgid2, n)
#define dcngettext(domain, msgid1, msgid2, n, category) __FUNCTION_NAME(dcngettext)(domain, msgid1, msgid2, n, category)
#define textdomain(domain) __FUNCTION_NAME(textdomain)(domain)
#define bindtextdomain(domain, dirname) __FUNCTION_NAME(bindtextdomain)(domain, dirname)
#define bind_textdomain_codeset(domain, codeset) __FUNCTION_NAME(bind_textdomain_codeset)(domain, codeset)
#define textdomain_generate(lid) __FUNCTION_NAME(textdomain_generate)(lid)
#endif

#endif

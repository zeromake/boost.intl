#define __DIABLE_COMPATIBLE_GETTEXT
#include <boost/locale/libintl.h>
#include <boost/locale/message.hpp>
#include <locale>
#include <memory>

static std::unique_ptr<std::locale> loc = nullptr;
static boost::locale::generator gen;

namespace {
inline static const std::locale& get_locale()
{
  return (loc != nullptr) ? *loc : std::locale::classic();
}
} // namespace

extern "C" {
const char* __FUNCTION_NAME(gettext)(const char* msgid)
{
  return boost::locale::basic_message<char>(msgid)
      .str_view(get_locale())
      .data();
}

const char* __FUNCTION_NAME(dgettext)(const char* domain, const char* msgid)
{
  return boost::locale::basic_message<char>(msgid)
      .str_view(get_locale(), domain)
      .data();
}
const char* __FUNCTION_NAME(dcgettext)(const char* domain, const char* msgid,
                                       int category)
{
  return boost::locale::basic_message<char>(msgid)
      .str_view(get_locale(), domain)
      .data();
}
const char* __FUNCTION_NAME(ngettext)(const char* msgid1, const char* msgid2,
                                      unsigned long int n)
{
  return boost::locale::basic_message<char>(msgid1, msgid2, n)
      .str_view(get_locale())
      .data();
}
const char* __FUNCTION_NAME(dngettext)(const char* domain, const char* msgid1,
                                       const char* msgid2, unsigned long int n)
{
  return boost::locale::basic_message<char>(msgid1, msgid2, n)
      .str_view(get_locale(), domain)
      .data();
}
const char* __FUNCTION_NAME(dcngettext)(const char* domain, const char* msgid1,
                                        const char* msgid2, unsigned long int n,
                                        int category)
{
  return boost::locale::basic_message<char>(msgid1, msgid2, n)
      .str_view(get_locale(), domain)
      .data();
}
const char* __FUNCTION_NAME(textdomain)(const char* domain)
{
  if (domain != NULL)
    gen.add_messages_domain(domain);
  return NULL;
}
const char* __FUNCTION_NAME(bindtextdomain)(const char* domain,
                                            const char* dirname)
{

  if (domain != NULL)
    gen.add_messages_domain(domain);
  if (dirname != NULL)
    gen.add_messages_path(dirname);
  return NULL;
}
const char* __FUNCTION_NAME(bind_textdomain_codeset)(const char* domain,
                                                     const char* codeset)
{
  if (domain != NULL)
    gen.add_messages_domain(domain);
  return NULL;
}

const char* __FUNCTION_NAME(generate)(const char* lid) {
  loc = std::unique_ptr<std::locale>(new std::locale(gen.generate(lid)));
  return nonstd::string_view(gen.lid()).data();
}
void __FUNCTION_NAME(add_domain)(const char* domain) {
  if (domain != NULL)
    gen.add_messages_domain(domain);
}
void __FUNCTION_NAME(add_path)(const char* dirname) {
  if (dirname != NULL)
    gen.add_messages_path(dirname);
}
} // extern "C"

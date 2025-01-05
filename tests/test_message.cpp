#include <gtest/gtest.h>
#include <boost/locale/message.hpp>

const char* message_path = BOOST_LOCALE_TEST_DATA;

namespace impl {
template <typename Char>
void test_translate(const std::string& sOriginal, const std::string& sExpected,
                    const std::locale& l, const std::string& domain)
{
  EXPECT_EQ(boost::locale::translate(sOriginal).str(l, domain), sExpected);
}
} // namespace impl

void test_translate(const std::string& original, const std::string& expected,
                    const std::locale& l, const std::string& domain)
{
  impl::test_translate<char>(original, expected, l, domain);
  impl::test_translate<wchar_t>(original, expected, l, domain);
}

TEST(message, translate)
{
  boost::locale::generator g;
  g.add_messages_domain("simple");
  g.add_messages_domain("full");
  g.add_messages_domain("fall/ISO-8859-1");
  g.add_messages_path(message_path);
  g.set_default_messages_domain("default");

  auto locale_name = "he_IL.UTF-8";
  std::locale l = g("he_IL.UTF-8");
  std::cout << "  Testing " << locale_name << std::endl;
  std::cout << "    single forms" << std::endl;

  test_translate("hello", "שלום", l, "default");
  test_translate("hello", "היי", l, "simple");
  test_translate("hello", "hello", l, "undefined");
  test_translate("untranslated", "untranslated", l, "default");
  // Check removal of old "context" information
  test_translate("#untranslated", "#untranslated", l, "default");
  test_translate("##untranslated", "##untranslated", l, "default");
  test_translate("#hello", "#שלום", l, "default");
}

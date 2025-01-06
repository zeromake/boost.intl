#include <gtest/gtest.h>
#include <boost/locale/message.hpp>

extern const char* message_path;

void test_translate(const std::string& original, const std::string& expected,
                    const std::locale& l, const std::string& domain)
{
  EXPECT_EQ(boost::locale::translate(original).str(l, domain), expected);
}

void test_wtranslate(const std::wstring& original, const std::wstring& expected,
                     const std::locale& l, const std::string& domain)
{
  EXPECT_EQ(boost::locale::translate(original).str(l, domain), expected);
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

  std::cout << "    wsingle forms" << std::endl;
  test_wtranslate(L"hello", L"שלום", l, "default");
  test_wtranslate(L"hello", L"היי", l, "simple");
  test_wtranslate(L"hello", L"hello", l, "undefined");
  test_wtranslate(L"untranslated", L"untranslated", l, "default");
  // Check removal of old "context" information
  test_wtranslate(L"#untranslated", L"#untranslated", l, "default");
  test_wtranslate(L"##untranslated", L"##untranslated", l, "default");
  test_wtranslate(L"#hello", L"#שלום", l, "default");
}

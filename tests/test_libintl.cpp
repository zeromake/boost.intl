#include <gtest/gtest.h>
#include <boost/locale/libintl.h>

extern const char* message_path;

TEST(libintl, gettext)
{
  bindtextdomain("default", message_path);
  textdomain("simple");
  textdomain("full");
  printf("%s\n", boost_locale_generate("he_IL.UTF-8"));

  EXPECT_STREQ(gettext("hello"),  "שלום");
}

#include <gtest/gtest.h>

const char* message_path = BOOST_LOCALE_TEST_DATA;

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

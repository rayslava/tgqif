#include <gtest/gtest.h>

int _argc;
char** _argv;

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  _argc = argc;
  _argv = argv;
  return RUN_ALL_TESTS();
}

#include "json/json.hpp"

#include <gtest/gtest.h>

int _argc;
char** _argv;

namespace json {

  TEST(json, priceConvert)
  {
    auto conv = JSONReceipt::priceFromText;
    EXPECT_THROW({conv("a");
                 }, std::runtime_error);
    EXPECT_NO_THROW({conv("1000");
                    });
    ASSERT_EQ(conv("1000"), 10.00);
  }

  TEST(json, parse)
  {
    fs::path test_file = fs::canonical(fs::path(_argv[0]).parent_path()) / fs::path("test.json");
    JSONReceipt jf(test_file);
    ASSERT_TRUE(jf.isValidReceipt());
  }
}

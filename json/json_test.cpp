#include "json/json.hpp"

#include <gtest/gtest.h>

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

  TEST(json, validReceipt)
  {
    const char* valid_json{"{\"document\" : {\"receipt\" : {}}}"};
    const char* wrong_json{"{\"document\" : {\"nothing\" : {}}}"};
    const char* wrong_string{"ERROR ERROR NOT A JSON"};

    {
      JSONReceipt rec{};
      rec._doc->Parse(valid_json);
      EXPECT_NO_THROW({rec.isValidReceipt();
                      });
      ASSERT_EQ(rec.isValidReceipt(), true);
    }
    {
      JSONReceipt rec{};
      rec._doc->Parse(wrong_json);
      EXPECT_NO_THROW({rec.isValidReceipt();
                      });
      ASSERT_EQ(rec.isValidReceipt(), false);
    }
    {
      JSONReceipt rec{};
      rec._doc->Parse(wrong_string);
      EXPECT_NO_THROW({rec.isValidReceipt();
                      });
      ASSERT_EQ(rec.isValidReceipt(), false);
    }
  }

  TEST(json, parse)
  {
    fs::path test_file = fs::current_path() / "json" / "test.json";
    std::cout << test_file.c_str();
    JSONReceipt jf{test_file};
    ASSERT_TRUE(jf.isValidReceipt());
  }
}

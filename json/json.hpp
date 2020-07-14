#pragma once

#include <filesystem>
#include <fstream>
#include <memory>
#include <rapidjson/document.h>

#if defined(_UNIT_TEST_BUILD)
#include <gtest/gtest_prod.h>
#endif

namespace fs = std::filesystem;
namespace rj = rapidjson;

namespace json {

  class JSONReceipt {
    std::unique_ptr<rj::Document> _doc;
    void parse(std::ifstream& input);
    static double priceFromText(const std::string& str);

  public:
    JSONReceipt(std::ifstream& file);
    JSONReceipt(const fs::path& file);
    ~JSONReceipt(){};
    bool isValidReceipt() const;

#if defined(_UNIT_TEST_BUILD)
  private:
    FRIEND_TEST(json, priceConvert);
    FRIEND_TEST(json, parse);
#endif
  };
}

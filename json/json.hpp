#pragma once

#include <filesystem>
#include <fstream>
#include <memory>
#include <vector>
#include <rapidjson/document.h>

#if defined(_UNIT_TEST_BUILD)
#include <gtest/gtest_prod.h>
#endif

namespace fs = std::filesystem;
namespace rj = rapidjson;

namespace json {

  struct Item {
    double qty;
    double price;
    std::string name;
    double sum;
  };

  class JSONReceipt {
    std::unique_ptr<rj::Document> _doc;
    void parse(std::ifstream& input);
    static double priceFromText(const std::string& str);

  public:
    JSONReceipt(std::ifstream& file);
    JSONReceipt(const fs::path& file);
    ~JSONReceipt(){};
    bool isValidReceipt() const;
    std::vector<Item> items() const;

#if defined(_UNIT_TEST_BUILD)
  private:
    JSONReceipt() :
      _doc(std::make_unique<rj::Document>())
    {};
    FRIEND_TEST(json, priceConvert);
    FRIEND_TEST(json, parse);
    FRIEND_TEST(json, validReceipt);
#endif
  };

}

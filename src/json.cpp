#include "json.hpp"

#include <fstream>
#include <iterator>
#include <memory>
#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <charconv>
#include <span>
#include <iostream>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/istreamwrapper.h>

namespace json {

  void JSONReceipt::parse(std::ifstream& input) {
    rj::IStreamWrapper iwr(input);
    _doc->ParseStream(iwr);
  }

  JSONReceipt::JSONReceipt(const fs::path& file) :
    _doc(std::make_unique<rj::Document>())
  {
    std::ifstream input(file);
    parse(input);
  }

  JSONReceipt::JSONReceipt(std::ifstream& input) :
    _doc(std::make_unique<rj::Document>())
  {
    parse(input);
  }

  bool JSONReceipt::isValidReceipt() const {
    return
      _doc->IsObject() &&
      _doc->HasMember("document") &&
      (*_doc)["document"].HasMember("receipt");
  }

  double JSONReceipt::priceFromText(const std::string& str) {
    if (!std::all_of(std::begin(str), std::end(str),
                     [](unsigned char c){
      return std::isdigit(c);
    }))
      throw std::runtime_error("Wrong price format in receipt");
    // First we have to cut last two digits which are cents
    std::span cent_line{std::cend(str) - 2, std::cend(str)};

    int cents = 0;
    if(auto [p, ec] = std::from_chars(cent_line.data(), cent_line.data() + cent_line.size(), cents);
       ec == std::errc())
      std::cout << cents;

    int val = 0;
    std::span val_line{std::cbegin(str), std::cend(str) - 2};
    if(auto [p, ec] = std::from_chars(cent_line.data(), cent_line.data() + cent_line.size(), val);
       ec == std::errc())
      std::cout << val;

    double result = double(val);
    result += double(cents) / 100;
    return result;
  }
}

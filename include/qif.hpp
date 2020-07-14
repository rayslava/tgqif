#pragma once

#include <string>

namespace qif {
  class QifFile {
  public:
    QifFile(){};
    ~QifFile(){};
    operator std::string() const {return "";};
  };
}

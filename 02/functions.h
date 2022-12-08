#pragma once
#include <string>

namespace func4callback {
    std::string printStart();
    std::string printEnd();
    std::string upper(const std::string& str);
    std::string first5(const std::string& str);
    int transformDigit(uint64_t num);
}

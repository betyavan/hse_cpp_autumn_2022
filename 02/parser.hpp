#pragma once
#include <string>
#include <vector>

class TokenParser
{
    using startEndFuncType = std::string(*)();
    using digitFuncType = int(*)(uint64_t);
    using strFuncType = std::string(*)(const std::string&);

public:

    std::vector<std::string> parse_test;
    std::vector<std::string> order_parse_test;

    TokenParser() = default;

    void SetStartCallback(startEndFuncType func) { startCallback = func; }
    void SetEndCallback(startEndFuncType func) { endCallback = func; }
    void SetDigitTokenCallback(digitFuncType func) { digitCallback = func; }
    void SetStrTokenCallback(strFuncType func) { strCallback = func; }

    void Parse(const std::string& line);

private:
    bool isDigit(const std::string& str);
private:
    startEndFuncType startCallback = nullptr;
    startEndFuncType endCallback = nullptr;
    digitFuncType digitCallback = nullptr;  
    strFuncType strCallback = nullptr;

};
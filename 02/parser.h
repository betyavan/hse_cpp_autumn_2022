#pragma once
#include <string>
#include <vector>

class TokenParser
{
    // "маски" для callback функции
    using startEndFuncType = std::string(*)();
    using digitFuncType = int(*)(uint64_t);
    using strFuncType = std::string(*)(const std::string&);

public:
    TokenParser() = default;

    void SetStartCallback(startEndFuncType func) { startCallback = func; }
    void SetEndCallback(startEndFuncType func) { endCallback = func; }
    void SetDigitTokenCallback(digitFuncType func) { digitCallback = func; }
    void SetStrTokenCallback(strFuncType func) { strCallback = func; }

    void ResetStartCallback() { startCallback = nullptr; }
    void ResetEndCallback() { endCallback = nullptr; }
    void ResetDigitTokenCallback() { digitCallback = nullptr; }
    void ResetStrTokenCallback() { strCallback = nullptr; }

    std::vector<std::string> Parse(const std::string& line);

private:
    bool isDigit(const std::string& str);
private:
    startEndFuncType startCallback = nullptr;
    startEndFuncType endCallback = nullptr;
    digitFuncType digitCallback = nullptr;  
    strFuncType strCallback = nullptr;
};
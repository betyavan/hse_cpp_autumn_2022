#include "functions.hpp"
#include <iostream>
#include <algorithm>
#include <sstream>

std::string func4callback::printStart()
{
    return "start";
}

std::string func4callback::printEnd()
{
    return "end";
}

std::string func4callback::upper(const std::string& str)
{
    // return caps str
    std::string tmp = str;
    std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::toupper);
    //std::cout << tmp << '\n';
    return tmp;
}

std::string func4callback::first5(const std::string& str)
{
    std::stringstream out;
    std::string::const_iterator it = str.begin();
    int i = 0;
    while (it != str.end() && i < 5) {
        out << *(it++);
        ++i;
    }
    //std::cout << out.str() << '\n';
    return out.str();
}

int func4callback::transformDigit(uint64_t num)
{
    //std::cout << "num: " << num % 100 << '\n';
    return num % 100;
}

#include "parser.hpp"
#include <sstream>


void TokenParser::Parse(const std::string& line) {
	parse_test.clear();
	order_parse_test.clear();

	// вызов start callback
	order_parse_test.push_back("start_callback");
	parse_test.push_back((startCallback != nullptr) ? startCallback() : "no_start_callback");
	// парсим
	std::stringstream inp(line);
	std::string cur_token;
	while (inp >> cur_token)
	{
		if (isDigit(cur_token)) {
			try { // обработка в случае числа, большего uint64_t
				uint64_t num = std::stoull(cur_token);
				if (digitCallback != nullptr)
					parse_test.push_back(std::to_string(digitCallback(num)));
				else
					parse_test.push_back(std::to_string(num));
				order_parse_test.push_back("num");
			}
			catch (std::out_of_range& ex) { //если большего uint64_t, то относимся как к строке
				parse_test.push_back((strCallback != nullptr) ? strCallback(cur_token) : cur_token);
				order_parse_test.push_back("str");
			}	
		}
		else {
			parse_test.push_back((strCallback != nullptr) ? strCallback(cur_token) : cur_token);
			order_parse_test.push_back("str");
		}
	}
	// вызов end callback
	order_parse_test.push_back("end_callback");
	parse_test.push_back((endCallback != nullptr) ? endCallback() : "no_end_callback");
}

bool TokenParser::isDigit(const std::string& str)
{
	std::string::const_iterator it = str.begin();
	while (it != str.end() && std::isdigit(*it)) ++it;
	return !str.empty() && it == str.end();
}


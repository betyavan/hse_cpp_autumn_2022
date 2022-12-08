#include "parser.h"
#include <sstream>


std::vector<std::string> TokenParser::Parse(const std::string& line) {
	std::vector<std::string> result; // ������ ��� �������� ��������
	// ����� start callback
	result.push_back((startCallback != nullptr) ? startCallback() : "no_start_callback");
	// ������
	std::stringstream inp(line);
	std::string cur_token;
	while (inp >> cur_token)
	{
		if (isDigit(cur_token)) {
			try { // ��������� � ������ �����, �������� uint64_t
				uint64_t num = std::stoull(cur_token);
				if (digitCallback != nullptr)
					result.push_back(std::to_string(digitCallback(num)));
				else
					result.push_back(std::to_string(num));
			}
			catch (std::out_of_range& ex) { // � ���� ������ ��������� ��� � ������
				//std::cout << "number is too big!\n";
				result.push_back((strCallback != nullptr) ? strCallback(cur_token) : cur_token);
			}	
		}
		else 
			result.push_back((strCallback != nullptr) ? strCallback(cur_token) : cur_token);
	}
	// ����� end callback
	result.push_back((endCallback != nullptr) ? endCallback() : "no_end_callback");

	return result;
}

bool TokenParser::isDigit(const std::string& str)
{
	std::string::const_iterator it = str.begin();
	while (it != str.end() && std::isdigit(*it)) ++it;
	return !str.empty() && it == str.end();
}


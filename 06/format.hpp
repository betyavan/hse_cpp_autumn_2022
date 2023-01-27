#pragma once

#include <sstream>
#include <map>

struct Exception : std::exception {
	Exception(const char* str) : std::exception(str) {}
};

struct InvalidException : Exception {
	InvalidException() : Exception("Invalid expression for formating") {}
};

struct IndexException : Exception {
	IndexException() : Exception("There is no parameter with such an index") {}
};


using enumerator = std::map<std::string, std::string>;


template <class T>
static void make_enum_args(enumerator& enumer, T&& val) {
	std::stringstream ss;
	ss << val;
	enumer[std::to_string(enumer.size())] = std::move(ss.str());
}

template <class T, class...Args>
static void make_enum_args(enumerator& enumer, T&& val, Args&&...args) {
	make_enum_args(enumer, val);
	make_enum_args(enumer, std::forward<Args>(args)...);
}

template <class...Args>
std::string format(const std::string& str, Args&&...args) {
	enumerator enum_args;
	make_enum_args(enum_args, args...);
	std::stringstream result;
	std::stringstream num;
	bool is_closed = true;
	for (const auto& symbol : str) {
		if (symbol == '}') {
			is_closed = true;
			if (num.str().size() == 0)
				throw InvalidException();
			else if (enum_args.find(num.str()) == enum_args.end())
				throw IndexException();
			result << enum_args[num.str()];
		}
		else if (symbol == '{') {
			if (!is_closed)
				throw InvalidException();
			num.str("");
			is_closed = false;
		}
		else if (!is_closed) {
			if (std::isdigit(symbol))
				num << symbol;
			else
				throw InvalidException();
		}
		else {
			result << symbol;
		}
	}

	return result.str();

}
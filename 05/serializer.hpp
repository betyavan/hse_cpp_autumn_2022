#pragma once

#include <iostream>
#include <string>

enum class Error {
    NoError,
    CorruptedArchive
};

// Serializer
class Serializer {
    static constexpr char Separator = ' ';
public:
    explicit Serializer(std::ostream& out) : out_(out) {}

    template <class T>
    Error save(T& object) { return object.serialize(*this); }

    template <class... ArgsT>
    Error operator()(ArgsT... args) { return process(args...); }

private:
    Error process(bool& arg) {
        if (arg)
            out_ << "true";
        else
            out_ << "false";
        out_ << Separator;
        return Error::NoError;
    }

    Error process(uint64_t& arg) {
        out_ << arg << Separator;
        return Error::NoError;
    }

    template <class T, class...ArgsT>
    Error process(T&& val, ArgsT&&...args) {
        if (process(val) != Error::NoError)
            return Error::CorruptedArchive;
        return process(std::forward<ArgsT>(args)...);
    }

private:
    std::ostream& out_;
};

// Deserializer
class Deserializer {
    static constexpr char Separator = ' ';
public:
    explicit Deserializer(std::istream& in) : in_(in) {}

    template <class T>
    Error load(T& object) { return object.serialize(*this); }

    template <class... ArgsT>
    Error operator()(ArgsT&... args) { return process(args...); }

private:

    bool isDigit(const std::string& str) {
        std::string::const_iterator it = str.begin();
        while (it != str.end() && std::isdigit(*it)) ++it;
        return !str.empty() && it == str.end();
    }

    Error process(bool& arg) {
        std::string text;
        in_ >> text;

        if (text == "true")
            arg = true;
        else if (text == "false")
            arg = false;
        else
            return Error::CorruptedArchive;

        return Error::NoError;
    }

    Error process(uint64_t& arg) {
        std::string text;
        in_ >> text;
        if (!isDigit(text)) return Error::CorruptedArchive;
        try {
            arg = std::stoull(text);
        }
        catch (...) {
            return Error::CorruptedArchive;
        }
        return Error::NoError;
    }

    template <class T, class...ArgsT>
    Error process(T&& val, ArgsT&&...args) {
        if (process(val) != Error::NoError)
            return Error::CorruptedArchive;
        return process(std::forward<ArgsT>(args)...);
    }

private:
    std::istream& in_;
};
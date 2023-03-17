#include "parser.hpp"
#include "functions.hpp"
#include "gtest/gtest.h"


TEST(ParserTest, EmptyString) {
    TokenParser parser;
    parser.SetStartCallback(func4callback::printStart);
    parser.SetEndCallback(func4callback::printEnd);
    parser.SetDigitTokenCallback(func4callback::transformDigit);
    parser.SetStrTokenCallback(func4callback::upper);

    std::string str = "";
    parser.Parse(str);
    std::vector<std::string> parse_test = { "start", "end" };
    std::vector<std::string> order_test = { "start_callback", "end_callback" };
    ASSERT_EQ(parser.parse_test, parse_test);
    ASSERT_EQ(parser.order_parse_test, order_test);
}

TEST(ParserTest, OnlySpaces) {
    TokenParser parser;
    parser.SetStartCallback(func4callback::printStart);
    parser.SetEndCallback(func4callback::printEnd);
    parser.SetDigitTokenCallback(func4callback::transformDigit);
    parser.SetStrTokenCallback(func4callback::upper);

    std::string str = "             ";
    parser.Parse(str);
    std::vector<std::string> parse_test = { "start", "end" };
    std::vector<std::string> order_test = { "start_callback", "end_callback" };
    ASSERT_EQ(parser.parse_test, parse_test);
    ASSERT_EQ(parser.order_parse_test, order_test);
}

TEST(ParserTest, OneSymbol) {
    TokenParser parser;
    parser.SetStartCallback(func4callback::printStart);
    parser.SetEndCallback(func4callback::printEnd);
    parser.SetDigitTokenCallback(func4callback::transformDigit);
    parser.SetStrTokenCallback(func4callback::upper);

    {
        std::string str = "1";
        parser.Parse(str);
        std::vector<std::string> parse_test = { "start", "1", "end" };
        std::vector<std::string> order_test = { "start_callback", "num", "end_callback" };
        ASSERT_EQ(parser.parse_test, parse_test);
        ASSERT_EQ(parser.order_parse_test, order_test);
    }

    {
        std::string str = "a";
        parser.Parse(str);
        std::vector<std::string> parse_test = { "start", "A", "end" };
        std::vector<std::string> order_test = { "start_callback", "str", "end_callback" };
        ASSERT_EQ(parser.parse_test, parse_test);
        ASSERT_EQ(parser.order_parse_test, order_test);
    }

}

TEST(ParserTest, LimitInt64) {
    TokenParser parser;
    parser.SetStartCallback(func4callback::printStart);
    parser.SetEndCallback(func4callback::printEnd);
    parser.SetDigitTokenCallback(func4callback::transformDigit);
    parser.SetStrTokenCallback(func4callback::upper);

    {
        // 2^64 - 1
        std::string str = "a 18446744073709551615";
        parser.Parse(str);
        std::vector<std::string> parse_test = { "start", "A", "15", "end" };
        std::vector<std::string> order_test = { "start_callback", "str", "num", "end_callback" };
        ASSERT_EQ(parser.parse_test, parse_test);
        ASSERT_EQ(parser.order_parse_test, order_test);
    }
    
    {
        // 2^64
        std::string str = "a 18446744073709551616";
        parser.Parse(str);
        std::vector<std::string> parse_test = { "start", "A", "18446744073709551616", "end" };
        std::vector<std::string> order_test = { "start_callback", "str", "str", "end_callback" };
        ASSERT_EQ(parser.parse_test, parse_test);
        ASSERT_EQ(parser.order_parse_test, order_test);
    }
    
    {
        // 2^65
        std::string str = "a 36893488147419103232";
        parser.Parse(str);
        std::vector<std::string> parse_test = { "start", "A", "36893488147419103232", "end" };
        std::vector<std::string> order_test = { "start_callback", "str", "str", "end_callback" };
        ASSERT_EQ(parser.parse_test, parse_test);
        ASSERT_EQ(parser.order_parse_test, order_test);
    }
}

TEST(ParserTest, AllNull) {
    TokenParser parser;
    parser.SetStartCallback(nullptr);
    parser.SetEndCallback(nullptr);
    parser.SetDigitTokenCallback(nullptr);
    parser.SetStrTokenCallback(nullptr);

    std::string str = "aaa 123";
    parser.Parse(str);
    std::vector<std::string> parse_test = { "no_start_callback", "aaa", "123", "no_end_callback" };
    std::vector<std::string> order_test = { "start_callback", "str", "num", "end_callback" };
    ASSERT_EQ(parser.parse_test, parse_test);
    ASSERT_EQ(parser.order_parse_test, order_test);
}

TEST(ParserTest, AllDefault) {
    TokenParser parser;

    std::string str = "  bebra\n    12345  4you  \n";
    parser.Parse(str);
    std::vector<std::string> parse_test = { "no_start_callback", "bebra", "12345", "4you", "no_end_callback" };
    std::vector<std::string> order_test = { "start_callback", "str", "num", "str", "end_callback" };
    ASSERT_EQ(parser.parse_test, parse_test);
    ASSERT_EQ(parser.order_parse_test, order_test);
}

TEST(ParserTest, MixesString) {
    TokenParser parser;
    parser.SetStartCallback(func4callback::printStart);
    parser.SetEndCallback(func4callback::printEnd);
    parser.SetDigitTokenCallback(func4callback::transformDigit);
    parser.SetStrTokenCallback(func4callback::upper);

    {
        std::string str = "    bebra  4you 123  ";
        parser.Parse(str);
        std::vector<std::string> parse_test = { "start", "BEBRA", "4YOU", "23", "end" };
        std::vector<std::string> order_test = { "start_callback", "str", "str", "num", "end_callback" };
        ASSERT_EQ(parser.parse_test, parse_test);
        ASSERT_EQ(parser.order_parse_test, order_test);
    }

    {
        std::string str = "bebra    42you    123\n   word2vec\n";
        parser.Parse(str);
        std::vector<std::string> parse_test = { "start", "BEBRA", "42YOU", "23", "WORD2VEC", "end" };
        std::vector<std::string> order_test = { "start_callback", "str", "str", "num", "str", "end_callback" };
        ASSERT_EQ(parser.parse_test, parse_test);
        ASSERT_EQ(parser.order_parse_test, order_test);
    }

}

TEST(ParserTest, AnotherFuncStr) {
    TokenParser parser;
    parser.SetStartCallback(func4callback::printStart);
    parser.SetEndCallback(func4callback::printEnd);
    parser.SetDigitTokenCallback(func4callback::transformDigit);
    parser.SetStrTokenCallback(func4callback::first5); // !!!

    {
        std::string str = "beeeeebra    4you    123\n   word2vec\n";
        parser.Parse(str);
        std::vector<std::string> parse_test = { "start", "beeee", "4you", "23", "word2", "end" };
        std::vector<std::string> order_test = { "start_callback", "str", "str", "num", "str", "end_callback" };
        ASSERT_EQ(parser.parse_test, parse_test);
        ASSERT_EQ(parser.order_parse_test, order_test);
    }

    {
        std::string str = "beeeeebra    4you    99999000000000000000000000\n   word2vec\n";
        parser.Parse(str);
        std::vector<std::string> parse_test = { "start", "beeee", "4you", "99999", "word2", "end" };
        std::vector<std::string> order_test = { "start_callback", "str", "str", "str", "str", "end_callback" };
        ASSERT_EQ(parser.parse_test, parse_test);
        ASSERT_EQ(parser.order_parse_test, order_test);
    }

}


int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

}
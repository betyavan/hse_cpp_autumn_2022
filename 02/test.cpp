#include "parser.h"
#include "functions.h"
#include "gtest/gtest.h"


TEST(TestUtils, my_test_1)
{
    // дефолтный тест, на проверку функциональности
    // разделены пробелами
    TokenParser parser;
    parser.SetStartCallback(func4callback::printStart);
    parser.SetEndCallback(func4callback::printEnd);
    parser.SetDigitTokenCallback(func4callback::transformDigit);
    parser.SetStrTokenCallback(func4callback::upper);

    
    std::string str = "    bebra  4you 123  ";
    std::vector<std::string> test = {"start", "BEBRA", "4YOU", "23", "end"};
    ASSERT_EQ(parser.Parse(str), test);
    
}

TEST(TestUtils, my_test_2)
{
    // тоже самое, только есть число не помещающееся в uint64_t => считаем строкой
    TokenParser parser;
    parser.SetStartCallback(func4callback::printStart);
    parser.SetEndCallback(func4callback::printEnd);
    parser.SetDigitTokenCallback(func4callback::transformDigit);
    parser.SetStrTokenCallback(func4callback::upper);

    std::string str = "    bebra  4you 99999999999999999999999999999999  ";
    std::vector<std::string> test = {"start", "BEBRA", "4YOU", "99999999999999999999999999999999", "end"};
    ASSERT_EQ(parser.Parse(str), test);
}

TEST(TestUtils, my_test_3)
{
    // разделаны табами и \n
    TokenParser parser;
    parser.SetStartCallback(func4callback::printStart);
    parser.SetEndCallback(func4callback::printEnd);
    parser.SetDigitTokenCallback(func4callback::transformDigit);
    parser.SetStrTokenCallback(func4callback::upper);


    std::string str = "bebra    4you    123\n   word2vec\n";
    std::vector<std::string> test = {"start", "BEBRA", "4YOU", "23", "WORD2VEC", "end"};
    ASSERT_EQ(parser.Parse(str), test);


}

TEST(TestUtils, my_test_4)
{
    // пустая строка
    TokenParser parser;
    parser.SetStartCallback(func4callback::printStart);
    parser.SetEndCallback(func4callback::printEnd);
    parser.SetDigitTokenCallback(func4callback::transformDigit);
    parser.SetStrTokenCallback(func4callback::upper);


    std::string str = "";
    std::vector<std::string> test = {"start", "end"};
    ASSERT_EQ(parser.Parse(str), test);


}

TEST(TestUtils, my_test_5)
{
    // используем дефолтное
    TokenParser parser;

    std::string str = "  bebra\n    12345  4you  \n";
    std::vector<std::string> test = { "no_start_callback", "bebra", "12345", "4you", "no_end_callback" };
    ASSERT_EQ(parser.Parse(str), test);


}

TEST(TestUtils, my_test_6)
{
    // другая функция для строк
    TokenParser parser;
    parser.SetStartCallback(func4callback::printStart);
    parser.SetEndCallback(func4callback::printEnd);
    parser.SetDigitTokenCallback(func4callback::transformDigit);
    parser.SetStrTokenCallback(func4callback::first5); // !!!


    std::string str = "beeeeebra    4you    123\n   word2vec\n";
    std::vector<std::string> test = { "start", "beeee", "4you", "23", "word2", "end" };
    ASSERT_EQ(parser.Parse(str), test);

}

TEST(TestUtils, my_test_7)
{
    // другая функция для строк, где число как строка
    TokenParser parser;
    parser.SetStartCallback(func4callback::printStart);
    parser.SetEndCallback(func4callback::printEnd);
    parser.SetDigitTokenCallback(func4callback::transformDigit);
    parser.SetStrTokenCallback(func4callback::first5); // !!!


    std::string str = "beeeeebra    4you    99999000000000000000000000\n   word2vec\n";
    std::vector<std::string> test = { "start", "beeee", "4you", "99999", "word2", "end" };
    ASSERT_EQ(parser.Parse(str), test);
}

TEST(TestUtils, my_test_8)
{
    // проверка ресета
    TokenParser parser;
    parser.SetStartCallback(func4callback::printStart);
    parser.SetEndCallback(func4callback::printEnd);
    parser.SetDigitTokenCallback(func4callback::transformDigit);
    parser.SetStrTokenCallback(func4callback::upper);

    parser.ResetStrTokenCallback();

    std::string str = "    bebra  4you 123  ";
    std::vector<std::string> test = { "start", "bebra", "4you", "23", "end" };
    ASSERT_EQ(parser.Parse(str), test);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

}
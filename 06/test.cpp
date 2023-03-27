#include "format.hpp"
#include <ostream>
#include "gtest/gtest.h"

TEST(BaseTest, test_smoke) {
    auto text = format("{1}+{1}={0}", 2, "one");
    ASSERT_EQ(text, "one+one=2");
}

TEST(BaseTest, hadrer_structure_string) {
    auto text = format("{2}{1}-:/akdb laj{0}", false, "one", 2);
    ASSERT_EQ(text, "2one-:/akdb laj0");
}

TEST(ExceptionTests, wrong_format_1) {
    ASSERT_THROW(format("{1}  {0a}", 1, "one"), InvalidException);
}

TEST(ExceptionTests, wrong_format_2) {
    ASSERT_THROW(format("{}   {0}", 1), InvalidException);

}

TEST(ExceptionTests, not_enough_args) {

    ASSERT_THROW(format("{0} {1} {2}", 1, 2), IndexException);
}

TEST(ExceptionTests, wrong_format_3) {

    ASSERT_THROW(format("{0} {{1}} {2}", 1, 2, 3), InvalidException);
}

TEST(ExceptionTests, wrong_format_4) {

    ASSERT_THROW(format("{0} } {1}", 1, 2), InvalidException);
}


struct MyStruct {
    int a, b;
    std::string str;
};

std::ostream& operator<<(std::ostream& out, const MyStruct& obj) {
    out << obj.a << ' ' << obj.str << ' ' << obj.b;
    return out;
}

TEST(AnotherTypeTest, PersonalType) {
    MyStruct a = { 1, 2, "aaa" }, b = { 3, 4, "bbb" };
    auto text = format("{1} AND {0}", a, b);
    ASSERT_EQ(text, "3 bbb 4 AND 1 aaa 2");
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#include "format.hpp"
#include "gtest/gtest.h"

TEST(BaseTest, Test_1) {
    auto text = format("{1}+{1}={0}", 2, "one");
    ASSERT_EQ(text, "one+one=2");
}

TEST(BaseTest, Test_2) {
    auto text = format("{2}{1}-:/akdb laj{0}", false, "one", 2);
    ASSERT_EQ(text, "2one-:/akdb laj0");
}

TEST(ExceptionTests, Test_1) {
    try {
        auto text = format("{1}  {0a}", 1, "one");
    }
    catch (const InvalidException&) {
        SUCCEED();
    }
    catch (...) {
        FAIL();
    }
}

TEST(ExceptionTests, Test_2) {
    try {
        auto text = format("{}   {0}", 1);
    }
    catch (const InvalidException&) {
        SUCCEED();
    }
    catch (...) {
        FAIL();
    }
}

TEST(ExceptionTests, Test_3) {
    try {
        auto text = format("{0} {1} {2}", 1, 2);
    }
    catch (const IndexException&) {
        SUCCEED();
    }
    catch (...) {
        FAIL();
    }
}

TEST(ExceptionTests, Test_4) {
    try {
        auto text = format("{0} {{1}} {2}", 1, 2, 3);
    }
    catch (const InvalidException&) {
        SUCCEED();
    }
    catch (...) {
        FAIL();
    }
}

TEST(ExceptionTests, Test_5) {
    try {
        auto text = format("{0} } {1}", 1, 2);
    }
    catch (const InvalidException&) {
        SUCCEED();
    }
    catch (...) {
        FAIL();
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
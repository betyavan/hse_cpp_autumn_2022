#include "bigint.h"
#include "gtest/gtest.h"


TEST(AssignmentFromInt, Test1) {
    int value = 42;
    BigInt bigint_val;
    bigint_val = value;

    std::ostringstream oss;
    oss << bigint_val;
    ASSERT_EQ(oss.str(), std::to_string(value));
}

TEST(UnaryMinus, Test1) {
    int value = 42;
    BigInt bigint_val = value;
    bigint_val = -bigint_val;

    std::ostringstream oss;
    oss << bigint_val;
    ASSERT_EQ(oss.str(), std::to_string(-value));
}


TEST(ToAssignment, Test1) {
    int val = 42;
    BigInt bigint_val = val;
    bigint_val += bigint_val += bigint_val += bigint_val;
    val += val += val += val;

    std::ostringstream oss;
    oss << bigint_val;
    ASSERT_EQ(oss.str(), std::to_string(val));
}

TEST(Arithmetic, Test1) {
    int a = 42;
    int b = 11;
    BigInt bigint_a = a;
    BigInt bigint_b = b;

    bigint_a -= bigint_b;
    a -= b;


    bigint_b -= bigint_a *= bigint_a *= bigint_b;
    b -= a *= a *= b;

    std::ostringstream oss;
    oss << bigint_a << bigint_b;
    ASSERT_EQ(oss.str(), std::to_string(a) + std::to_string(b));
}


TEST(InStream, Test1) {
    int value = 42;
    BigInt bigint_val = value;

    std::ostringstream oss;
    oss << bigint_val;

    ASSERT_EQ(oss.str(), std::to_string(value));
}

TEST(Comparison, Test1) {
    BigInt a = std::string{"9325492345983274589758023847509283745827349587212938129"};
    BigInt b = std::string{"348762838512387487213648172639471269348172397461297"};

    std::ostringstream oss;
    oss << (a <= b) << (a >= b) << (a == b) << (a != b) << (a < b) << (a > b);
    ASSERT_EQ(oss.str(), "010101");
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
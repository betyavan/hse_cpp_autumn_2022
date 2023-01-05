#include "data.h"
#include "serializer.h"
#include "gtest/gtest.h"

TEST(TestCaseName, TestName) {
    Data x{ 1, true, 2 };

    std::stringstream stream;

    Serializer serializer(stream);
    serializer.save(x);

    ASSERT_EQ(stream.str(), "1 true 2 ");

    Data y{ 0, false, 0 };

    Deserializer deserializer(stream);
    const Error err = deserializer.load(y);

    ASSERT_EQ(err, Error::NoError);

    ASSERT_EQ(x.a, y.a);
    ASSERT_EQ(x.b, y.b);
    ASSERT_EQ(x.c, y.c);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

}
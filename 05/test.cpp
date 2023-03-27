#include "data.hpp"
#include "serializer.hpp"
#include "gtest/gtest.h"

TEST(SerializerTest, test_smoke) {
    Data x{ 1, true, 2 };

    std::stringstream stream;

    Serializer serializer(stream);
    Error err = serializer.save(x);
    ASSERT_EQ(err, Error::NoError);
    ASSERT_EQ(stream.str(), "1 true 2 ");

    Data y{ 0, false, 0 };

    Deserializer deserializer(stream);
    err = deserializer.load(y);

    ASSERT_EQ(err, Error::NoError);
    ASSERT_EQ(x.a, y.a);
    ASSERT_EQ(x.b, y.b);
    ASSERT_EQ(x.c, y.c);
}


TEST(SerializerTest, PersonTypeTest) {
    MyStruct x{ false, 1234567 };
    std::stringstream stream;
    Serializer serializer(stream);
    Error err = serializer.save(x);
    ASSERT_EQ(err, Error::NoError);
    ASSERT_EQ(stream.str(), "false 1234567 ");
    MyStruct y{ true, 10 };
    Deserializer deserializer(stream);
    err = deserializer.load(y);
    ASSERT_EQ(err, Error::NoError);
    ASSERT_EQ(x.a, y.a);
    ASSERT_EQ(x.b, y.b);
}


TEST(OnlyDeserializerTest, test) {

    {
        std::stringstream stream;
        stream << "true 1";
        MyStruct x;
        Deserializer deserializer(stream);
        Error err = deserializer.load(x);
        ASSERT_EQ(err, Error::NoError);
        ASSERT_EQ(x.a, true);
        ASSERT_EQ(x.b, 1);
    }
    
    {
        std::stringstream stream;
        stream << "false 0";
        MyStruct x;
        Deserializer deserializer(stream);
        Error err = deserializer.load(x);
        ASSERT_EQ(err, Error::NoError);
        ASSERT_EQ(x.a, false);
        ASSERT_EQ(x.b, 0);
    }
}

TEST(ExceptionTest, CorruptedTest) {
    {
        std::stringstream stream;
        stream << "0 0";
        MyStruct x;
        Deserializer deserializer(stream);
        Error err = deserializer.load(x);
        ASSERT_EQ(err, Error::CorruptedArchive);
    }
    
    {
        std::stringstream stream;
        stream << "1 1";
        MyStruct x;
        Deserializer deserializer(stream);
        Error err = deserializer.load(x);
        ASSERT_EQ(err, Error::CorruptedArchive);
    }
    
    {   // 2^64 + 1
        std::stringstream stream;
        stream << "true 18446744073709551617";
        MyStruct x;
        Deserializer deserializer(stream);
        Error err = deserializer.load(x);
        ASSERT_EQ(err, Error::CorruptedArchive);
    }
    
    {   // mixed string
        std::stringstream stream;
        stream << "true 42str";
        MyStruct x;
        Deserializer deserializer(stream);
        Error err = deserializer.load(x);
        ASSERT_EQ(err, Error::CorruptedArchive);
    }
    
    {   // wrong bool
        std::stringstream stream;
        stream << "true1 42";
        MyStruct x;
        Deserializer deserializer(stream);
        Error err = deserializer.load(x);
        ASSERT_EQ(err, Error::CorruptedArchive);
    }
}



int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

}
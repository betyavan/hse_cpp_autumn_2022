#pragma once

#include <stdint.h>
#include "serializer.hpp"

struct Data {
    uint64_t a;
    bool b;
    uint64_t c;

    template <class Serializer>
    Error serialize(Serializer& serializer) { return serializer(a, b, c); }
};

struct MyStruct {
    bool a;
    uint64_t b;

    template <class Serializer>
    Error serialize(Serializer& serializer) { return serializer(a, b); }
};
#pragma once

#include <stdint.h>
#include "serializer.h"

struct Data {
    uint64_t a;
    bool b;
    uint64_t c;

    template <class Serializer>
    Error serialize(Serializer& serializer) { return serializer(a, b, c); }
};
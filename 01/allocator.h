#pragma once

#include <cstddef>

class Allocator
{
public:
    Allocator() = default;
    void makeAllocator(size_t maxSize);
    char* alloc(size_t size);
    void reset();
    ~Allocator();
private:
    // char весит 1 байт, подходит, так как выделяем память по байтам
    char* start_ = nullptr; // указатель на начало выделенной памяти
    size_t max_size_ = 0; // ее размер
    size_t used_ = 0; // сколько на данный момент используется
    char* offset_ = nullptr; // указатель указывающий на место, готовое на выдачу
};
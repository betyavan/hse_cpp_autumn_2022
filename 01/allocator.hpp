#pragma once

#include <cstddef>

class Allocator
{
public:
    Allocator() = default;
    void makeAllocator(size_t maxSize);
    char* alloc(size_t size);
    void reset() { offset_ = 0; }
    ~Allocator() { delete[] start_; }
private:
    // char весит 1 байт, подходит, так как выделяем память по байтам
    char* start_ = nullptr; // указатель на начало выделенной памяти
    size_t max_size_ = 0; // ее размер
    size_t offset_ = 0; // смещение
};
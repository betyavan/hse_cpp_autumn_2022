#include "allocator.hpp"

void Allocator::makeAllocator(size_t maxSize)
{
	// проверка на повторный вызов, в этом случае удаляем старую
	if (start_ != nullptr)
		delete[] start_;
	// выделяем память
	if (maxSize == 0)
		start_ = nullptr;
	else
		start_ = new char[maxSize];
	offset_ = 0;
	max_size_ = maxSize;
}

char* Allocator::alloc(size_t size)
{
	// проверка на возможность выделения памяти
	if (size > max_size_ - offset_)
		return nullptr;
	char* tmp_offset = start_ + offset_;
	offset_ += size;
	return tmp_offset;
}

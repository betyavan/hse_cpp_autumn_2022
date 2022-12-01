#include "allocator.h"

void Allocator::makeAllocator(size_t maxSize)
{
	// проверка на повторный вызов, в этом случае удаляем старую
	if (start_ != nullptr)
		delete[] start_;
	// выделяем память
	if (maxSize == 0)
		start = nullptr;
	else
		start_ = new char[maxSize];
	offset_ = start_;
	used_ = 0;
	max_size_ = maxSize;
}

char* Allocator::alloc(size_t size)
{
	// проверка на возможность выделения памяти
	if (size > max_size_ - used_)
		return nullptr;
	used_ += size;
	// то что отдадим пользователю
	char* tmp_offset = offset_;
	// здесь работает арифметика указателей
	// если свободного места больше нет, то offset_ будет указывать
	// на последний байт выделенной в makeAllocator памяти, чтобы не вылезти в память, которая нам недоступна
	if (used_ == max_size_)
		offset_ += (size - 1);
	else
		// в ином случае просто сдвигаемся на size и offset_ будет указывать 
		// на память готовую к отдаче
		offset_ += size;
	return tmp_offset;
}

void Allocator::reset()
{
	// необходимо лишь изменить размер и сместить offset_ на начало
	offset_ = start_;
	used_ = 0;
}

Allocator::~Allocator()
{
	// обязательно очищаем память, чтоб не было утечки
	delete[] start_;
}

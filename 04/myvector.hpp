#pragma once

#include "myvector.h"


template <typename T>
my::MyVector<T>::MyVector(size_t size, const T& value) {
	if (size == 0)
		return;
	T* tmp_ptr = reinterpret_cast<T*>(operator new(sizeof(T) * size));
	T* current_ptr = tmp_ptr;
	try {
		while (current_ptr != tmp_ptr + size) {
			new(current_ptr) T(value);
			++current_ptr;
		}
	}
	catch (...) {
		while (current_ptr-- != tmp_ptr)
			current_ptr->~T();
		delete reinterpret_cast<void*>(tmp_ptr);
		throw;
	}
	m_ptr = tmp_ptr;
	m_size = size;
	m_capacity = size;
}

template <typename T>
my::MyVector<T>::MyVector(size_t size) {
	if (size == 0)
		return;
	T* tmp_ptr = reinterpret_cast<T*>(operator new(sizeof(T) * size));
	T* current_ptr = tmp_ptr;
	try {
		while (current_ptr != tmp_ptr + size) {
			new(current_ptr) T();
			++current_ptr;
		}
	}
	catch (...) {
		while (current_ptr-- != tmp_ptr)
			current_ptr->~T();
		delete reinterpret_cast<void*>(tmp_ptr);
		throw;
	}
	m_ptr = tmp_ptr;
	m_size = size;
	m_capacity = size;
}

template <typename T>
my::MyVector<T>::MyVector(std::initializer_list<T> lst) {
	if (lst.size() == 0)
		return;
	T* tmp_ptr = reinterpret_cast<T*>(operator new(sizeof(T) * lst.size()));
	T* current_ptr = tmp_ptr;
	try {
		for (const T& el : lst) {
			new(current_ptr) T(el);
			++current_ptr;
		}
	}
	catch (...) {
		while (current_ptr-- != tmp_ptr)
			current_ptr->~T();
		delete reinterpret_cast<void*>(tmp_ptr);
		throw;
	}
	m_ptr = tmp_ptr;
	m_size = lst.size();
	m_capacity = lst.size();
}


template<typename T>
template<typename T2>
inline void my::MyVector<T>::copy_constructor(const MyVector<T2>& obj) {
	if (obj.m_size == 0)
		return;
	T* tmp_ptr = reinterpret_cast<T*>(operator new(sizeof(T) * obj.size()));
	T* current_ptr = tmp_ptr;
	try {
		for (T2* obj_ptr = obj.m_ptr;
			current_ptr != tmp_ptr + obj.m_size;
			++current_ptr, ++obj_ptr)
			new(current_ptr) T(*obj_ptr);
	}
	catch (...) {
		while (current_ptr-- != tmp_ptr)
			current_ptr->~T();
		delete reinterpret_cast<void*>(tmp_ptr);
		throw;
	}
	m_ptr = tmp_ptr;
	m_size = obj.m_size;
	m_capacity = obj.m_size;
}

template<typename T>
inline my::MyVector<T>::~MyVector() {
	for (T* ptr = m_ptr; ptr != m_ptr + m_size; ++ptr)
		ptr->~T();
	delete reinterpret_cast<void*>(m_ptr);
}

template <typename T>
my::MyVector<T>& my::MyVector<T>::operator=(const MyVector& obj) {
	if (&obj == this)
		return *this;
	return operator=<T>(obj);
}

template <typename T>
template <typename Other>
my::MyVector<T>& my::MyVector<T>::operator=(const MyVector<Other>& obj) {
	if (m_capacity < obj.size())
		return *this = MyVector(obj);
	Other* obj_ptr = obj.m_ptr;
	T* current_ptr = m_ptr;
	while (current_ptr != m_ptr + std::min(m_size, obj.m_size))
		*current_ptr++ = *obj_ptr++;
	if (m_size < obj.size()) {
		try {
			for (; current_ptr != m_ptr + obj.m_size; ++obj_ptr, ++current_ptr)
				new(current_ptr) T(*obj_ptr);
		}
		catch (...) {
			while (current_ptr-- != m_ptr + m_size)
				current_ptr->~T();
			throw;
		}
	}
	else {
		while (current_ptr != m_ptr + m_size)
			(current_ptr++)->~T();
	}
	m_size = obj.m_size;
	return *this;
}

template <typename T>
void my::MyVector<T>::swap(MyVector& obj) noexcept {
	std::swap(m_ptr, obj.m_ptr);
	std::swap(m_size, obj.m_size);
	std::swap(m_capacity, obj.m_capacity);
}

template <typename T>
void my::MyVector<T>::push_back(const T& el) {
	if (m_size + 1 > m_capacity)
		reserve(2 * m_capacity + 1);
	new(m_ptr + m_size) T(el);
	++m_size;
}

template <typename T>
void my::MyVector<T>::pop_back() {
	if (m_size == 0)
		throw EmptyException{};
	(m_ptr + --m_size)->~T();
}

template<typename T>
inline void my::MyVector<T>::clear() noexcept {
	for (T* ptr = m_ptr; ptr != m_ptr + m_size; ++ptr)
		ptr->~T();
	m_size = 0;
}

template <typename T>
void my::MyVector<T>::resize(size_t size) {
	reserve(size);
	T* current_ptr = m_ptr + m_size;
	if (m_size < size) {
		try {
			for (; current_ptr != m_ptr + size; ++current_ptr)
				new(current_ptr) T();
		}
		catch (...) {
			while (current_ptr-- != m_ptr + m_size)
				current_ptr->~T();
			throw;
		}
	}
	else {
		while (current_ptr-- != m_ptr + size)
			current_ptr->~T();
	}
	m_size = size;
}

template <typename T>
void my::MyVector<T>::reserve(size_t capacity) {
	if (capacity <= m_capacity) {
		return;
	}
	T* tmp_ptr = reinterpret_cast<T*>(operator new(sizeof(T) * capacity));
	T* current_ptr = tmp_ptr;
	try {
		for (T* old_ptr = m_ptr;
			current_ptr != tmp_ptr + m_size;
			++current_ptr, ++old_ptr)
			new(current_ptr) T(*old_ptr);
	}
	catch (...) {
		while (current_ptr-- != tmp_ptr)
			current_ptr->~T();
		delete reinterpret_cast<void*>(tmp_ptr);
		throw;
	}
	for (T* ptr = m_ptr; ptr != m_ptr + m_size; ++ptr)
		ptr->~T();
	delete reinterpret_cast<void*>(m_ptr);
	m_ptr = tmp_ptr;
	m_capacity = capacity;
}

template <typename T>
void my::MyVector<T>::shrink_to_fit() {
	if (m_capacity == m_size)
		return;
	*this = MyVector(*this);
}

template <typename T1, typename T2>
bool my::operator==(const MyVector<T1>& obj1, const MyVector<T2>& obj2) {
	if (obj1.size() != obj2.size())
		return false;
	for (size_t i = 0; i < obj1.size(); ++i) {
		if (obj1[i] != obj2[i])
			return false;
	}
	return true;
}

template<typename T>
inline T my::MyVector<T>::front() const {
	if (m_size == 0)
		throw my::EmptyException();
	return *m_ptr;
}

template<typename T>
inline T my::MyVector<T>::back() const {
	if (m_size == 0)
		throw my::EmptyException();
	return *(m_ptr + m_size - 1);
}

template <typename T>
T& my::MyVector<T>::at(size_t i) noexcept {
	if (i >= m_size)
		throw OutOfRangeException();
	return m_ptr[i];
}

template <typename T>
const T& my::MyVector<T>::at(size_t i) const noexcept {
	if (i >= m_size)
		throw OutOfRangeException();
	return m_ptr[i];
}

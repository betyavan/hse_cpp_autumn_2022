#pragma once

#include "matrix.h"

template <typename T>
linalg::Matrix<T>::Matrix(size_t rows) {
	if (rows == 0)
		return;
	T* tmp_ptr = reinterpret_cast<T*>(operator new(sizeof(T) * rows));
	T* cur_ptr = tmp_ptr;
	try {
		while (cur_ptr != tmp_ptr + rows) {
			new(cur_ptr) T();
			++cur_ptr;
		}
	}
	catch (...) {
		while (cur_ptr-- != tmp_ptr)
			cur_ptr->~T();
		delete reinterpret_cast<void*>(tmp_ptr);
		throw;
	}
	m_ptr = tmp_ptr;
	m_size = rows;
	m_capacity = rows;
	m_rows = rows;
	m_columns = 1;
}

template<typename T>
inline linalg::Matrix<T>::Matrix(size_t rows, size_t cols) : Matrix(rows* cols) {
	m_rows = rows;
	m_columns = cols;
}

template <typename T>
template <typename T2>
inline void linalg::Matrix<T>::copy_constructor(const Matrix<T2>& obj) {
	if (obj.m_size == 0)
		return;
	T* tmp_ptr = reinterpret_cast<T*>(operator new(sizeof(T) * obj.m_size));
	T* cur_ptr = tmp_ptr;
	try {
		for (T2* obj_ptr = obj.m_ptr;
			cur_ptr != tmp_ptr + obj.m_size;
			++cur_ptr, ++obj_ptr)
			new(cur_ptr) T(*obj_ptr);
	}
	catch (...) {
		while (cur_ptr-- != tmp_ptr)
			cur_ptr->~T();
		delete reinterpret_cast<void*>(tmp_ptr);
		throw;
	}
	m_ptr = tmp_ptr;
	m_size = obj.m_size;
	m_capacity = obj.m_size;
	m_rows = obj.m_rows;
	m_columns = obj.m_columns;
}

template<typename T>
template<typename Other>
inline linalg::Matrix<T>::Matrix(std::initializer_list<Other> lst) {
	if (lst.size() == 0)
		return;
	T* tmp_ptr = reinterpret_cast<T*>(operator new(sizeof(T) * lst.size()));
	T* cur_ptr = tmp_ptr;
	try {
		for (const Other& el : lst) {
			new(cur_ptr) T(el);
			++cur_ptr;
		}
	}
	catch (...) {
		while (cur_ptr-- != tmp_ptr)
			cur_ptr->~T();
		delete reinterpret_cast<void*>(tmp_ptr);
		throw;
	}
	m_ptr = tmp_ptr;
	m_size = lst.size();
	m_capacity = lst.size();
	m_rows = lst.size();
	m_columns = 1;
}

template<typename T>
template<typename Other>
inline linalg::Matrix<T>::Matrix(std::initializer_list<std::initializer_list<Other>> lst) {
	if (lst.size() == 0)
		return;

	m_rows = lst.size();
	int columns = 0;
	bool is_first = true;
	for (std::initializer_list<Other> row : lst) {
		if (!is_first && row.size() != columns)
			throw linalg::InvalidException();
		if (is_first)
			columns = row.size();
		is_first = false;
	}
	if (columns == 0)
		return;
	m_columns = columns;

	T* tmp_ptr = reinterpret_cast<T*>(operator new(sizeof(T) * m_rows * m_columns));
	T* cur_ptr = tmp_ptr;
	try {
		for (std::initializer_list<Other> row : lst) {
			for (const Other& el : row) {
				new(cur_ptr) T(el);
				++cur_ptr;
			}
		}
	}
	catch (...) {
		while (cur_ptr-- != tmp_ptr)
			cur_ptr->~T();
		delete reinterpret_cast<void*>(tmp_ptr);
		throw;
	}
	m_ptr = tmp_ptr;
	m_size = m_rows * m_columns;
	m_capacity = m_rows * m_columns;

}

template<typename T>
inline linalg::Matrix<T>::~Matrix() {
	for (T* ptr = m_ptr; ptr != m_ptr + m_size; ++ptr)
		ptr->~T();
	delete reinterpret_cast<void*>(m_ptr);
}

template<typename T>
linalg::Matrix<T>& linalg::Matrix<T>::operator=(const Matrix& obj) {
	if (&obj == this)
		return *this;
	return operator=<T>(obj);
}

template<typename T>
inline T& linalg::Matrix<T>::operator()(size_t i, size_t j) {
	if (i >= m_rows || j >= m_columns)
		throw linalg::OutOfRangeException();
	return m_ptr[m_columns * i + j];
}

template<typename T>
inline const T& linalg::Matrix<T>::operator()(size_t i, size_t j) const {
	if (i >= m_rows || j >= m_columns)
		throw linalg::OutOfRangeException();
	return m_ptr[m_columns * i + j];
}

template<typename T>
template<typename Other>
inline linalg::Matrix<T>& linalg::Matrix<T>::operator*=(const Other& num) {
	for (size_t i = 0; i < m_rows; i++) {
		for (size_t j = 0; j < m_columns; j++)
			(*this)(i, j) *= num;
	}
	return *this;
}

template <typename T>
template <typename Other>
linalg::Matrix<T>& linalg::Matrix<T>::operator=(const Matrix<Other>& obj) {
	if (m_capacity < obj.m_size)
		return *this = Matrix(obj);
	Other* obj_ptr = obj.m_ptr;
	T* cur_ptr = m_ptr;
	while (cur_ptr != m_ptr + std::min(m_size, obj.m_size))
		*cur_ptr++ = *obj_ptr++;
	if (m_size < obj.m_size) {
		try {
			for (; cur_ptr != m_ptr + obj.m_size; ++obj_ptr, ++cur_ptr)
				new(cur_ptr) T(*obj_ptr);
		}
		catch (...) {
			while (cur_ptr-- != m_ptr + m_size)
				cur_ptr->~T();
			throw;
		}
	}
	else {
		while (cur_ptr != m_ptr + m_size)
			(cur_ptr++)->~T();
	}
	m_size = obj.m_size;
	m_rows = obj.m_rows;
	m_columns = obj.m_columns;
	return *this;
}

template<typename T>
template<typename Other>
inline linalg::Matrix<T>& linalg::Matrix<T>::operator+=(const Matrix<Other>& obj) {
	if (obj.rows() != m_rows || obj.columns() != m_columns)
		throw linalg::ValueException();
	for (size_t i = 0; i < m_size; ++i)
		m_ptr[i] += obj.m_ptr[i];
	return *this;
}

template<typename T>
template<typename Other>
inline linalg::Matrix<T>& linalg::Matrix<T>::operator-=(const Matrix<Other>& obj)
{
	if (obj.rows() != m_rows || obj.columns() != m_columns)
		throw linalg::ValueException();
	for (size_t i = 0; i < m_size; ++i)
		m_ptr[i] -= obj.m_ptr[i];
	return *this;
}

template<typename T>
template<typename Other>
inline linalg::Matrix<T>& linalg::Matrix<T>::operator*=(const Matrix<Other>& obj) {
	if (obj.rows() != m_columns)
		throw linalg::ValueException();
	Matrix<T> result(m_rows, obj.columns());
	for (size_t i = 0; i < m_rows; ++i) {
		for (size_t j = 0; j < obj.columns(); ++j) {
			T sum = 0;
			for (size_t k = 0; k < obj.rows(); ++k)
				sum += (*this)(i, k) * obj(k, j);
			result(i, j) = sum;
		}
	}
	return *this = result;
}


template <typename T>
void linalg::Matrix<T>::reshape(size_t rows, size_t cols) {
	if (rows * cols != m_size)
		throw linalg::ReshapeException();
	m_rows = rows;
	m_columns = cols;
}

template<typename T>
inline void linalg::Matrix<T>::shrink_to_fit() {
	if (m_capacity == m_size)
		return;
	*this = Matrix(*this);
}

template<typename T>
inline void linalg::Matrix<T>::clear() noexcept {
	for (T* ptr = m_ptr; ptr != m_ptr + m_size; ++ptr)
		ptr->~T();
	m_size = 0;
	m_rows = 0;
	m_columns = 0;
}

template<typename T>
inline void linalg::Matrix<T>::swap(Matrix& obj) noexcept {
	std::swap(m_ptr, obj.m_ptr);
	std::swap(m_size, obj.m_size);
	std::swap(m_capacity, obj.m_capacity);
	std::swap(m_rows, obj.m_rows);
	std::swap(m_columns, obj.m_columns);

}

template<typename T>
inline void linalg::Matrix<T>::reserve(int capacity) {
	if (capacity <= m_capacity)
		return;
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

template<typename T1, typename T2>
bool linalg::operator==(const Matrix<T1>& lhs, const Matrix<T2>& rhs) {
	if (lhs.rows() != rhs.rows() || lhs.columns() != rhs.columns())
		return false;
	for (size_t i = 0; i < lhs.rows(); ++i) {
		for (size_t j = 0; j < lhs.columns(); ++j) {
			if (lhs(i, j) != rhs(i, j))
				return false;
		}
	}
	return true;
}
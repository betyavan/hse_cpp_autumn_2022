#pragma once
#include <initializer_list>
#include <stdexcept>

namespace my {
	template <typename T = double>
	class MyVector {
	public:
		MyVector() noexcept = default;
		MyVector(size_t size, const T& value);
		MyVector(size_t size);
		MyVector(std::initializer_list<T> lst);
		MyVector(const MyVector& obj) { copy_constructor(obj); }
		template <typename Other>
		MyVector(const MyVector<Other>& obj) { copy_constructor(obj); }
		MyVector(MyVector&& obj) noexcept { swap(obj); }
		~MyVector();

		MyVector& operator=(const MyVector& obj);
		template <typename Other>
		MyVector& operator=(const MyVector<Other>& obj);
		MyVector& operator=(MyVector&& obj) noexcept { swap(obj); return *this; }

		T& operator[](size_t i) noexcept { return m_ptr[i]; }
		const T& operator[](size_t i) const noexcept { return m_ptr[i]; }
		T front() const;
		T back() const;

		T& at(size_t i) noexcept;
		const T& at(size_t i) const noexcept;
		size_t size() const noexcept { return m_size; }
		size_t capacity() const noexcept { return m_capacity; }
		void reserve(size_t capacity);
		void shrink_to_fit();
		void swap(MyVector& obj) noexcept;
		void push_back(const T& el);
		void pop_back();
		bool empty() const noexcept { return m_size == 0; }
		void clear() noexcept;
		void resize(size_t size);
	private:
		template <typename T2>
		void copy_constructor(const MyVector<T2>& obj);
	private:
		T* m_ptr = nullptr;
		size_t m_capacity = 0;
		size_t m_size = 0;
	};

	template <typename T>
	inline void swap(MyVector<T>& obj1, MyVector<T>& obj2) noexcept { obj1.swap(obj2); }

	template <typename T1, typename T2>
	bool operator==(const MyVector<T1>& obj1, const MyVector<T2>& obj2);

	template <typename T1, typename T2>
	inline bool operator!=(const MyVector<T1>& obj1, const MyVector<T2>& obj2) {
		return !(obj1 == obj2);
	}


	struct  Exception : public std::runtime_error {
		Exception(const char* str) : std::runtime_error(str) {}
	};

	struct  EmptyException : public Exception {
		EmptyException() : Exception("Invalid operation on empty vector") {}
	};

	struct  OutOfRangeException : public Exception {
		OutOfRangeException() : Exception("Out of range exception") {}
	};
}

#include "myvector.hpp"

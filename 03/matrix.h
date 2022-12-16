#pragma once

#include <initializer_list>
#include <stdexcept>

namespace linalg {
	template <typename T = double>
	class Matrix {
	public:
		Matrix() noexcept = default;
		Matrix(size_t rows);
		Matrix(size_t rows, size_t cols);
		Matrix(const Matrix& obj) { copy_constructor(obj); }
		template <typename Other>
		Matrix(const Matrix<Other>& obj) { copy_constructor(obj); }
		Matrix(Matrix<T>&& obj) noexcept { swap(obj); }
		template<typename Other>
		Matrix(std::initializer_list<Other> lst);
		template<typename Other>
		Matrix(std::initializer_list<std::initializer_list<Other>> lst);
		~Matrix();

		Matrix& operator=(const Matrix& obj);
		template <typename Other>
		Matrix& operator=(const Matrix<Other>& obj);
		Matrix& operator=(Matrix&& obj) noexcept { swap(obj); return *this; }

		T& operator()(size_t i, size_t j);
		const T& operator()(size_t i, size_t j) const;

		template <typename Other>
		Matrix& operator+=(const Matrix<Other>& obj);
		template <typename Other>
		Matrix& operator-=(const Matrix<Other>& obj);
		template <typename Other>
		Matrix& operator*=(const Matrix<Other>& obj);
		template <typename Other>
		Matrix& operator*=(const Other& num);

		template <typename Other> friend class Matrix;

		size_t size() const noexcept { return m_rows * m_columns; }
		size_t rows() const noexcept { return m_rows; }
		size_t columns() const noexcept { return m_columns; }
		bool empty() const noexcept { return m_rows * m_columns == 0; }
		void reshape(size_t rows, size_t cols);
		size_t capacity() const noexcept { return m_capacity; }
		void reserve(int capacity);
		void shrink_to_fit();
		void clear() noexcept;
		void swap(Matrix& obj) noexcept;
	private:
		template <typename T2>
		void copy_constructor(const Matrix<T2>& obj);
	private:
		T* m_ptr = nullptr;
		size_t m_rows = 0;
		size_t m_columns = 0;
		size_t m_capacity = 0;
	};

	template <typename T1, typename T2>
	bool operator==(const Matrix<T1>& lhs, const Matrix<T2>& rhs);

	template <typename T1, typename T2>
	bool operator!=(const Matrix<T1>& lhs, const Matrix<T2>& rhs) {
		return !(lhs == rhs);
	}

	template <typename T1, typename T2>
	auto operator+(const Matrix<T1>& lhs, const Matrix<T2>& rhs) {
		Matrix<decltype(T1() + T2())> result = lhs;
		return result += rhs;
	}

	template <typename T1, typename T2>
	auto operator-(const Matrix<T1>& lhs, const Matrix<T2>& rhs) {
		Matrix<decltype(T1() - T2())> result = lhs;
		return result -= rhs;
	}

	template <typename T1, typename T2>
	auto operator*(const Matrix<T1>& lhs, const Matrix<T2>& rhs);

	template <typename T1, typename T2>
	auto operator*(const Matrix<T1>& lhs, const T2& rhs) {
		Matrix<decltype(T1() * T2())> result = lhs;
		return result *= rhs;
	}

	template <typename T1, typename T2>
	auto operator*(const T1& lhs, const Matrix<T2>& rhs) {
		Matrix<decltype(T1() * T2())> result = rhs;
		return result *= lhs;
	}

	struct MatrixException : public std::exception {
		MatrixException(const char* msg) : std::exception(msg) {}
	};

	struct InvalidException : public MatrixException {
		InvalidException() : MatrixException("Invalid matrix") {}
	};

	struct ReshapeException : public MatrixException {
		ReshapeException() : MatrixException("Invalid reshape") {}
	};

	struct OutOfRangeException : public MatrixException {
		OutOfRangeException() : MatrixException("Out of range") {}
	};

	struct ValueException : public MatrixException {
		ValueException() : MatrixException("Invalid value for linalg operation") {}
	};


}

#include "matrix.hpp"

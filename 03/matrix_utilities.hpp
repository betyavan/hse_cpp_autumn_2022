#pragma once

#include <iomanip>
#include <sstream>
#include "matrix_utilities.h"

template <typename T>
static size_t obj_len(const T& obj) {
	std::stringstream str;
	str << obj;
	return str.str().size();
}


template <typename T>
static size_t max_obj_len(const linalg::Matrix<T>& mat, bool first_col = false) {
	size_t result = 0;
	if (first_col) {
		for (size_t i = 0; i < mat.rows(); ++i)
			result = std::max(result, obj_len(mat(i, 0)));
	}
	else {
		for (size_t i = 0; i < mat.rows(); ++i) {
			for (size_t j = 1; j < mat.columns(); ++j)
				result = std::max(result, obj_len(mat(i, j)));
		}
	}
	return result;
}


template <typename T>
std::ostream& operator<<(std::ostream& out, const linalg::Matrix<T>& mat) {
	if (mat.empty())
		return out << "|Empty|";
	size_t first_col_width = max_obj_len(mat, true);
	size_t rest_width = max_obj_len(mat);
	for (size_t i = 0; i < mat.rows(); ++i) {
		out << '|' << std::setw(first_col_width) << mat(i, 0);
		for (size_t j = 1; j < mat.columns(); ++j)
			out << ' ' << std::setw(rest_width) << mat(i, j);
		out << '|' << '\n';
	}
	return out;
}
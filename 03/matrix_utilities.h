#pragma once

#include "matrix.h"
#include <ostream>

template <typename T>
std::ostream& operator<<(std::ostream& out, const linalg::Matrix<T>& mat);

#include "matrix_utilities.hpp"
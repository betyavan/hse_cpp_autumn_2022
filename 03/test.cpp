#include <iostream>
#include "matrix.hpp"
#include "gtest/gtest.h"

TEST(MatrixTest, copy_constr_test) {
	linalg::Matrix<int> m1 = { 1, 2, 3, 4, 5, 6 };
	linalg::Matrix<int> m2 = m1;
	ASSERT_TRUE(m2 == m1);
}

TEST(MatrixTest, move_constr_test) {
	linalg::Matrix<int> m1 = { 1, 2, 3, 4, 5, 6 };
	linalg::Matrix<int> m2 = std::move(m1);
	linalg::Matrix<int> test = { 1, 2, 3, 4, 5, 6 };
	ASSERT_TRUE(m2 == test);
	ASSERT_TRUE(m1.empty());
}


TEST(MatrixTest, uniform_init_test2) {
	linalg::Matrix<int> m1 = { {1, 2, 3 } };
	ASSERT_TRUE(m1.rows() == 1 && m1.columns() == 3);
	linalg::Matrix<int> m2 = { {{1}}, {2}, {3} };
	ASSERT_TRUE(m2.rows() == 3 && m2.columns() == 1);
}

TEST(MatrixTest, copy_assigment_test) {
	linalg::Matrix<int> m1;
	linalg::Matrix<int> m2 = { 1, 2, 3 };
	m1 = m2;
	ASSERT_EQ(m1, m2);

	// another type
	linalg::Matrix<double> m3 = { {1.2, 2.3}, {3.1, 3.2} };
	linalg::Matrix<int> m4 = { {1, 2}, {3, 3} };
	m1 = m3;
	ASSERT_EQ(m1, m4);
}

TEST(MatrixTest, move_assigment_test) {
	linalg::Matrix<int> m1;
	linalg::Matrix<int> m2 = { 1, 2, 3 };
	m1 = std::move(m2);
	ASSERT_TRUE(m1.rows() == 3 && m1.columns() == 1);

	// another type
	linalg::Matrix<double> m3 = { {1.2, 1.5}, {3.1, 3.2} };
	linalg::Matrix<int> test = { {1, 1}, {3, 3} };
	m1 = std::move(m3);
	ASSERT_TRUE(m1 == test);
}

TEST(MatrixTest, index_test) {
	linalg::Matrix<int> m = { {1, 2, 3}, {4, 5, 6} };
	int val1 = m(0, 2);
	ASSERT_EQ(val1, 3);
	m(0, 2) = 7;
	ASSERT_EQ(m(0, 2), 7);

	// first dim fail
	try {
		m(3, 2);
	}
	catch (const linalg::OutOfRangeException&) {}
	catch (...) { FAIL(); }

	// second dim fail
	try {
		m(0, 5);
	}
	catch (const linalg::OutOfRangeException&) {}
	catch (...) { FAIL(); }

	const linalg::Matrix<int> c_m = { {1, 2, 3}, {4, 5, 6} };
	int val2 = c_m(0, 2);
	ASSERT_EQ(val2, 3);

	//c_m(0,2) = 7; compilation error, cause c_m is const
}

TEST(MatrixTest, print_test) {
	linalg::Matrix<int> m = { {123, 34, 56, 7}, {1, 2, 4, 3}, {5, 4, 321, 1} };
	std::string test = "|123  34  56   7|\n|  1   2   4   3|\n|  5   4 321   1|\n";
	
	std::stringstream ss;
	ss << m;
	ASSERT_EQ(ss.str(), test);
}


TEST(MatrixTest, plus_assigment_test) {
	linalg::Matrix<int> m1 = { 1, 2, 3 };
	linalg::Matrix<int> m2 = { 4, 5, 6 };
	m1 += m2;
	linalg::Matrix<int> check = { 5, 7, 9 };
	ASSERT_EQ(m1, check);

	m1 = { 1, 2, 3 };
	linalg::Matrix<double> m3 = { 4., 5., 6. };
	m1 += m3;
	ASSERT_EQ(m1, check);

	linalg::Matrix<double> m_d = { {1, 2, 3}, {4, 5, 6 } };
	linalg::Matrix<int> m_i = { {1.1, 2.2, 3.3}, {4.4, 5.5, 6.6} };
	linalg::Matrix<short> m_s = m_d;
	m_s = m_i;
	m_s += m_i;

	try {
		linalg::Matrix<int> m1 = { 1, 2, 3 };
		linalg::Matrix<int> m2 = { 1, 2, 3 , 4 };
		m1 += m2;
	}
	catch (const linalg::ValueException&) {}
	catch (...) { ASSERT_EQ(1, 2); }
}

TEST(MatrixTest, plus_test) {
	linalg::Matrix<int> m1 = { 1, 2, 3 };
	linalg::Matrix<double> m2 = { 4., 5., 6. };
	linalg::Matrix<int> check = { 5, 7, 9 };
	ASSERT_EQ(m1 + m2, check);
}

TEST(MatrixTest, mult_test) {
	linalg::Matrix<int> m1 = { {1, 2, 3}, {4, 5, 6} };
	linalg::Matrix<double> m2 = { {10., 11.}, {12., 13.}, {14., 15.} };
	linalg::Matrix<int> check = { {76, 82}, { 184, 199 } };
	ASSERT_EQ(m1 * m2, check);
}

TEST(MatrixTest, mult_num_test) {
	linalg::Matrix<double> m = { 1., 2., 3. };
	linalg::Matrix<int> check = { 2, 4, 6 };
	ASSERT_EQ(2 * m, check);
}

TEST(MatrixTest, mult_ass_test) {
	linalg::Matrix<double> m = { 1., 2., 3. };
	m *= 2;
	linalg::Matrix<int> check = { 2, 4, 6 };
	ASSERT_EQ(m, check);
}

TEST(MatrixTest, rows_test) {
	linalg::Matrix<int> m1(4);
	ASSERT_EQ(m1.rows(), 4);
}

TEST(MatrixTest, columns_test) {
	linalg::Matrix<int> m1(4);
	ASSERT_EQ(m1.columns(), 1);
	linalg::Matrix<int> m2(4, 6);
	ASSERT_EQ(m2.columns(), 6);
}

TEST(MatrixTest, empty_test) {
	linalg::Matrix<int> m;
	ASSERT_TRUE(m.empty());
}

TEST(MatrixTest, reshape_test) {
	linalg::Matrix<int> m = { {1, 2, 3}, {4, 5, 6} };
	ASSERT_TRUE(m.rows() == 2 && m.columns() == 3);
	m.reshape(3, 2);
	ASSERT_TRUE(m.rows() == 3 && m.columns() == 2);
	try {
		m.reshape(3, 3);
	}
	catch (const linalg::ReshapeException&) {}
	catch (...) { FAIL(); }

}

TEST(MatrixTest, capacity_test) {
	linalg::Matrix<int> m(3, 5);
	ASSERT_EQ(m.capacity(), 15);
}


TEST(MatrixTest, reserve_test) {
	linalg::Matrix<int> m(3, 5);
	m.reserve(10);
	ASSERT_EQ(m.capacity(), 15);
	m.reserve(20);
	ASSERT_EQ(m.capacity(), 20);
}


TEST(MatrixTest, shrink_to_fit_test) {
	linalg::Matrix<int> m = { {1, 2, 3}, {4, 5, 6} };
	ASSERT_EQ(m.capacity(), 6);
	m.reserve(15);
	ASSERT_EQ(m.capacity(), 15);
	m.shrink_to_fit();
	ASSERT_EQ(m.capacity(), 6);
}

TEST(MatrixTest, clear_test) {
	linalg::Matrix<int> m = { {1, 2, 3}, {4, 5, 6} };
	m.clear();
	ASSERT_TRUE(m.empty());
}

TEST(MatrixTest, swap_test) {
	linalg::Matrix<int> m1 = { {1, 2, 3 } };
	linalg::Matrix<int> m2 = { {1, 2, 3, 5}, {5, 6, 7, 8} };
	m1.swap(m2);
	ASSERT_TRUE(m1.rows() == 2 && m1.columns() == 4 && m2.rows() == 1 && m2.columns() == 3);
}

TEST(MatrixTest, equal_matrixes) {
	linalg::Matrix<int> m1 = { {1, 2, 3, 5}, {5, 6, 7, 8} };
	linalg::Matrix<int> m2 = m1;
	ASSERT_TRUE(m1 == m2);

	m2(1, 2) = 20;
	ASSERT_TRUE(m1 != m2);

	m2(1, 2) = 7;
	ASSERT_TRUE(m1 == m2);
}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

}
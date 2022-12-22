#pragma once

#include <string>
#include "myvector.h"
#include <ostream>

class BigInt {
public:
	// храним в одном элементе по 4 цифры
	static const int BASE = 10000;
	static const int SIZE = 4;

	BigInt();
	BigInt(int num);
	BigInt(std::string num);
	BigInt(const BigInt& obj);
	BigInt(BigInt&& obj) noexcept;

	BigInt& operator=(const BigInt& obj);
	BigInt& operator=(BigInt&& obj) noexcept;

	BigInt operator-() const;

	BigInt& operator+=(const BigInt& obj);
	BigInt& operator-=(const BigInt& obj);
	BigInt& operator*=(const BigInt& obj);

	friend std::ostream& operator<<(std::ostream& out, const BigInt& num);

	friend BigInt operator+(BigInt lhs, const BigInt& rhs);
	friend BigInt operator+(BigInt lhs, int32_t rhs);
	friend BigInt operator-(BigInt lhs, const BigInt& rhs);
	friend BigInt operator-(BigInt lhs, int32_t rhs);
	friend BigInt operator*(const BigInt& lhs, const BigInt& rhs);
	friend BigInt operator*(const BigInt& lhs, int32_t rhs);

	friend bool operator==(const BigInt& lhs, const BigInt& rhs);
	friend bool operator!=(const BigInt& lhs, const BigInt& rhs) { return !(lhs == rhs); }
	friend bool operator<(const BigInt& lhs, const BigInt& rhs);
	friend bool operator<=(const BigInt& lhs, const BigInt& rhs) { return lhs < rhs || lhs == rhs; }
	friend bool operator>(const BigInt& lhs, const BigInt& rhs) { return !(lhs < rhs) && lhs != rhs; }
	friend bool operator>=(const BigInt& lhs, const BigInt& rhs) { return !(lhs < rhs); }

private:
	void remove_zeros(); // удаляет ведущие нули
private:
	my::MyVector<int> data; // u16 так как будем в одном элементе хранить несклько цифр, экономим память
	bool negative_flag;

};


#include "bigint.h"

BigInt::BigInt() {
	data = my::MyVector<int>(1, 0);
	negative_flag = 0;
}

BigInt::BigInt(int num) {
	negative_flag = num < 0;
	data.clear();
	num = std::abs(num);
	if (num == 0)
		data = my::MyVector<int>(1, 0);
	else {
		while (num != 0) {
			int x = num % BigInt::BASE;
			data.push_back(x);
			num /= BigInt::BASE;
		}
	}
	remove_zeros();
}

BigInt::BigInt(std::string num) {
	if (num.empty()) {
		negative_flag = 0;
		data = my::MyVector<int>(1, 0);
		return;
	}
	
	if (num[0] == '-') {
		negative_flag = 1;
		num = num.substr(1);
	}
	else
		negative_flag = 0;
	
	for (long long i = num.size(); i > 0; i -= BigInt::SIZE) {
		if (i < BigInt::SIZE)
			data.push_back(atoi(num.substr(0, i).c_str()));
		else
			data.push_back(atoi(num.substr(i - BigInt::SIZE, BigInt::SIZE).c_str()));
	}
	remove_zeros();
}

BigInt::BigInt(const BigInt& obj) {
	data = obj.data;
	negative_flag = obj.negative_flag;
}

BigInt::BigInt(BigInt&& obj) noexcept {
	data = std::move(obj.data);
	negative_flag = obj.negative_flag;
}

BigInt& BigInt::operator=(const BigInt& obj) {
	data = obj.data;
	negative_flag = obj.negative_flag;
	return *this;
}

BigInt& BigInt::operator=(BigInt&& obj) noexcept {
	data = std::move(obj.data);
	negative_flag = obj.negative_flag;
	return *this;
}

BigInt BigInt::operator-() const {
	BigInt tmp = *this;
	tmp.negative_flag = (this->negative_flag + 1) % 2;
	if (tmp.data.size() == 1 && tmp.data[0] == 0)
		tmp.negative_flag = 0;
	return tmp;
}

BigInt& BigInt::operator+=(const BigInt& obj) {
	*this = *this + obj;
	return *this;
}

BigInt& BigInt::operator-=(const BigInt& obj) {
	*this = *this - obj;
	return *this;
}

BigInt& BigInt::operator*=(const BigInt& obj) {
	*this = *this * obj;
	return *this;
}

void BigInt::remove_zeros() {
	while (data.size() > 1 && data.back() == 0)
		data.pop_back();
	if (data.size() == 1 && data[0] == 0)
		negative_flag = 0;
}

static std::string collate(size_t len, std::string str) {
	std::string out(len - str.size(), '0');
	return out + str;
}

std::ostream& operator<<(std::ostream& out, const BigInt& num) {
	if (num.negative_flag)
		out << '-';
	out << num.data.back();
	for (long long i = num.data.size() - 2; i >= 0; --i)
		out << collate(BigInt::SIZE, std::to_string(num.data[i]));
	return out;
}

BigInt operator+(BigInt lhs, const BigInt& rhs) {
	if (lhs.negative_flag) {
		if (rhs.negative_flag)
			return -((-lhs) + (-rhs));
		return rhs - (-lhs);
	}
	if (rhs.negative_flag)
		return rhs + lhs;
	int carry = 0;
	for (size_t i = 0;
		i < std::max(lhs.data.size(), rhs.data.size()) || carry != 0; ++i) {
		if (i == lhs.data.size())
			lhs.data.push_back(0);
		lhs.data[i] += (i >= rhs.data.size() ? 0 : rhs.data[i]) + carry;
		carry = lhs.data[i] / BigInt::BASE;
		lhs.data[i] %= BigInt::BASE;
	}
	lhs.remove_zeros();
	return lhs;
}

BigInt operator+(BigInt lhs, int32_t rhs) {
	BigInt tmp_rhs = rhs;
	return lhs + rhs;
}

BigInt operator-(BigInt lhs, const BigInt& rhs) {
	if (lhs.negative_flag) {
		if (rhs.negative_flag)
			return lhs + (-rhs);
		return -((-lhs) + rhs);
	}
	if (rhs.negative_flag)
		return lhs + (-rhs);
	if (lhs < rhs) return -(rhs - lhs);
	int carry = 0;
	for (size_t i = 0;
		i < std::max(lhs.data.size(), rhs.data.size()) || carry != 0; ++i) {
		if (i == lhs.data.size())
			lhs.data.push_back(0);
		lhs.data[i] -= (i >= rhs.data.size() ? 0 : rhs.data[i]) + carry;
		carry = 0;
		if (lhs.data[i] < 0) {
			lhs.data[i] += BigInt::BASE;
			carry = 1;
		}
	}
	lhs.remove_zeros();
	return lhs;
}

BigInt operator-(BigInt lhs, int32_t rhs) {
	BigInt tmp_rhs = rhs;
	return lhs - tmp_rhs;
}

BigInt operator*(const BigInt& lhs, const BigInt& rhs) {
	BigInt result;
	result.data.resize(lhs.data.size() + rhs.data.size());
	for (size_t i = 0; i < lhs.data.size(); ++i) {
		int carry = 0;
		for (size_t j = 0; j < rhs.data.size() || carry != 0; ++j) {
			long long cur = result.data[i + j] +
				lhs.data[i] * 1LL * (j < rhs.data.size() ? rhs.data[j] : 0) + carry;
			result.data[i + j] = static_cast<int>(cur % BigInt::BASE);
			carry = static_cast<int>(cur / BigInt::BASE);
		}
	}
	result.negative_flag = lhs.negative_flag != rhs.negative_flag;
	result.remove_zeros();
	return result;
}

BigInt operator*(const BigInt& lhs, int32_t rhs) {
	BigInt tmp_rhs = rhs;
	return lhs * tmp_rhs;
}

bool operator==(const BigInt& lhs, const BigInt& rhs) {
	return (lhs.data == rhs.data && lhs.negative_flag == rhs.negative_flag);
}

bool operator<(const BigInt& lhs, const BigInt& rhs) {
	if (lhs == rhs)
		return false;
	if (lhs.negative_flag && !rhs.negative_flag)
		return true;
	if (rhs.negative_flag && !lhs.negative_flag)
		return false;
	if (lhs.negative_flag && rhs.negative_flag) {
		if (lhs.data.size() > rhs.data.size())
			return true;
		if (lhs.data.size() < rhs.data.size())
			return false;
		for (long long i = lhs.data.size() - 1; i >= 0; --i) {
			if (lhs.data[i] != rhs.data[i])
				return lhs.data[i] > rhs.data[i];
		}
	}
	if (!lhs.negative_flag && !rhs.negative_flag) {
		if (lhs.data.size() > rhs.data.size()) 
			return false;
		if (lhs.data.size() < rhs.data.size()) 
			return true;
		for (long long i = lhs.data.size() - 1; i >= 0; i--) {
			if (lhs.data[i] != rhs.data[i]) 
				return lhs.data[i] < rhs.data[i];
		}
	}
	return 0;
}

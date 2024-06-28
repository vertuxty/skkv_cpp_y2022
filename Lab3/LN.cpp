

//#include "LN.h"
//
//#include "CustomErrors.h"
//#include "constants.h"
//#include "return_codes.h"
//
//#include <cmath>
//#include <cstring>
//#include <iostream>
//
//LN::LN(long long int const number)
//{
//	this->NaN_ = 0;
//	this->sign_ = number >= 0 ? 1 : -1;
//	this->zero_ = number == 0 ? 1 : 0;
//	long long module_number = number * this->sign_;
//	size_t len = LN::get_length(module_number);
//	int pos = this->sign_ == -1 ? -1 : 0;
//	this->capacity_ = (len + pos) / this->block_size_ + 1;
//	this->value_ = new uint8_t[this->capacity_ + 1];
//	if (!this->value_)
//	{
//		throw MemoryError();
//	}
//	fill_zero();
//	LN::hex_to_base(number, len);
//}
//
//LN::LN(std::string_view number)
//{
//	if ("NaN" == number)
//	{
//		this->NaN_ = 1;
//	}
//	else if (number == "-0" || number == "0")
//	{
//		this->zero_ = 1;
//		this->NaN_ = 0;
//		this->sign_ = 1;
//		this->value_ = new uint8_t[this->capacity_ + 1];
//		if (!this->value_)
//		{
//			throw MemoryError();
//		}
//		this->value_[0] = 0;
//		this->value_[1] = 0;
//		this->value_[2] = 0;
//	}
//	else
//	{
//		this->NaN_ = 0;
//		this->zero_ = 0;
//		this->sign_ = (number[0] == '-') ? -1 : 1;
//		size_t len = number.length();
//		int pos = this->sign_ == -1 ? -1 : 0;
//		this->capacity_ = (len + pos) / this->block_size_ + 1;
//		this->value_ = new uint8_t[this->capacity_ + 1];
//		if (!this->value_)
//		{
//			throw MemoryError();
//		}
//		fill_zero();
//		LN::hex_to_base(number, len);
//	}
//}
//
//LN::LN(const char *number)
//{
//	if (strcmp(number, "NaN") == 0)
//	{
//		this->NaN_ = 1;
//	}
//	else if (strcmp(number, "-0") == 0 || strcmp(number, "0") == 0)
//	{
//		this->zero_ = 1;
//		this->NaN_ = 0;
//		this->sign_ = 1;
//		this->value_ = new uint8_t[this->capacity_ + 1];
//		if (!this->value_)
//		{
//			throw MemoryError();
//		}
//		this->value_[0] = 0;
//		this->value_[1] = 0;
//		this->value_[2] = 0;
//	}
//	else
//	{
//		this->NaN_ = 0;
//		this->zero_ = 0;
//		this->sign_ = number[0] == '-' ? -1 : 1;
//		int pos = this->sign_ == -1 ? -1 : 0;
//		size_t len = strlen(number);
//		this->capacity_ = (len + pos) / this->block_size_ + 1;
//		this->value_ = new uint8_t[this->capacity_ + 1];
//		if (!this->value_)
//		{
//			throw MemoryError();
//		}
//		LN::fill_zero();
//		LN::hex_to_base(number, len);
//	}
//}
//
//LN::LN(size_t capacity_)
//{
//	this->value_ = new uint8_t[capacity_ + 1];
//	if (!this->value_)
//	{
//		throw MemoryError();
//	}
//	this->NaN_ = 0;
//	this->zero_ = 1;
//	this->sign_ = 1;
//	this->size_ = capacity_ - 1;
//	this->capacity_ = capacity_;
//	for (int i = capacity_; i != -1; i--)
//	{
//		this->value_[i] = 0;
//	}
//}
//
//LN::LN(LN &&number) noexcept
//{
//	if (this != &number)
//	{
//		this->value_ = number.value_;
//		this->sign_ = number.sign_;
//		this->size_ = number.size_;
//		this->NaN_ = number.NaN_;
//		this->capacity_ = number.capacity_;
//		this->zero_ = number.zero_;
//		number.value_ = nullptr;
//		number.size_ = 0;
//		number.sign_ = 0;
//		number.capacity_ = 0;
//		number.NaN_ = 1;
//		number.zero_ = 0;
//	}
//}
//
//LN::LN(const LN &number)
//{
//	if (this != &number)
//	{
//		this->value_ = new uint8_t[number.capacity_ + 1];
//		if (!this->value_)
//		{
//			throw MemoryError();
//		}
//		std::memcpy(this->value_, number.value_, (number.capacity_ + 1) * sizeof(uint8_t));
//		this->sign_ = number.sign_;
//		this->zero_ = number.zero_;
//		this->NaN_ = number.NaN_;
//		this->capacity_ = number.capacity_;
//		this->size_ = number.size_;
//	}
//}
//
//LN &LN::operator=(const LN &number)
//{
//	if (this == &number)
//	{
//		return *this;
//	}
//	delete[] this->value_;
//	this->value_ = new uint8_t[number.capacity_ + 1];
//	if (!this->value_)
//	{
//		throw MemoryError();
//	}
//	this->capacity_ = number.capacity_;
//	std::memcpy(this->value_, number.value_, number.capacity_ * sizeof(uint8_t));
//	this->sign_ = number.sign_;
//	this->zero_ = number.zero_;
//	this->NaN_ = number.NaN_;
//	this->size_ = number.size_;
//	return *this;
//}
//
//LN &LN::operator=(LN &&number) noexcept
//{
//	if (*this == number)
//	{
//		return *this;
//	}
//	delete[] this->value_;
//	this->value_ = number.value_;
//	this->sign_ = number.sign_;
//	this->size_ = number.size_;
//	this->NaN_ = number.NaN_;
//	this->capacity_ = number.capacity_;
//	this->zero_ = number.zero_;
//	number.value_ = nullptr;
//	number.size_ = 0;
//	number.sign_ = 0;
//	number.capacity_ = 0;
//	number.NaN_ = 1;
//	number.zero_ = 0;
//	return *this;
//}
//
//bool LN::operator<(const LN &number) const
//{
//	if (this->value_ == nullptr || number.value_ == nullptr)
//	{
//		return false;
//	}
//	if (this->NaN_ || number.NaN_)
//	{
//		return false;
//	}
//	if (this->zero_ && number.zero_)
//	{
//		return false;
//	}
//	if ((this->sign_ < number.sign_) || (this->zero_ == 1 && number.sign_ > 0))
//	{
//		return true;
//	}
//	if ((this->sign_ > number.sign_) || (this->zero_ == 1 && number.sign_ > 0))
//	{
//		return false;
//	}
//	if ((this->sign_ < 0 && number.zero_))
//	{
//		return true;
//	}
//	if ((this->sign_ > 0 && number.zero_))
//	{
//		return false;
//	}
//	if (this->capacity_ == number.capacity_)
//	{
//		for (int i = this->capacity_; i != -1; i--)
//		{
//			if (this->value_[i] < number.value_[i] && this->sign_ > 0 && number.sign_ > 0)
//			{
//				return true;
//			}
//			else if (number.value_[i] < this->value_[i] && this->sign_ > 0 && number.sign_ > 0)
//			{
//				return false;
//			}
//			else if (this->value_[i] > number.value_[i] && this->sign_ < 0 && number.sign_ < 0)
//			{
//				return true;
//			}
//			else if (this->value_[i] < number.value_[i] && this->sign_ < 0 && number.sign_ < 0)
//			{
//				return false;
//			}
//		}
//	}
//	else
//	{
//		int count_this = 0;
//		int count_number = 0;
//		size_t j = this->capacity_;
//		while (true)
//		{
//			if (this->value_[j] != 0)
//			{
//				break;
//			}
//			count_this++;
//			j--;
//		}
//		j = number.capacity_;
//		while (true)
//		{
//			if (number.value_[j] != 0)
//			{
//				break;
//			}
//			count_number++;
//			j--;
//		}
//		if (this->capacity_ - count_this != number.capacity_ - count_number)
//		{
//			if ((this->sign_ > 0) && (this->capacity_ - count_this) < (number.capacity_ - count_number))
//			{
//				return true;
//			}
//			else if (this->sign_ < 0 && (this->capacity_ - count_this) > (number.capacity_ - count_number))
//			{
//				return true;
//			}
//			return false;
//		}
//		for (int i = this->capacity_ - count_this; i != -1; i--)
//		{
//			if (this->value_[i] < number.value_[i] && this->sign_ > 0 && number.sign_ > 0)
//			{
//				return true;
//			}
//			else if (number.value_[i] < this->value_[i] && this->sign_ > 0 && number.sign_ > 0)
//			{
//				return false;
//			}
//			else if (this->value_[i] > number.value_[i] && this->sign_ < 0 && number.sign_ < 0)
//			{
//				return true;
//			}
//			else if (this->value_[i] < number.value_[i] && this->sign_ < 0 && number.sign_ < 0)
//			{
//				return false;
//			}
//		}
//	}
//	return false;
//}
//
//bool LN::operator==(const LN &number) const
//{
//	if (this->value_ == nullptr || number.value_ == nullptr)
//	{
//		return false;
//	}
//	if (this->NaN_ || number.NaN_)
//	{
//		return false;
//	}
//	if (this->zero_ && number.zero_)
//	{
//		return true;
//	}
//	if (this->zero_ && !number.zero_ || !this->zero_ && number.zero_)
//	{
//		return false;
//	}
//	if (this->sign_ != number.sign_)
//	{
//		return false;
//	}
//	if (this->capacity_ == number.capacity_)
//	{
//		for (int i = this->capacity_; i != -1; i--)
//		{
//			if (this->value_[i] != number.value_[i])
//			{
//				return false;
//			}
//		}
//	}
//	else
//	{
//		int count_this = 0;
//		int count_number = 0;
//		size_t i = this->capacity_;
//		while (true)
//		{
//			if (this->value_[i] != 0)
//			{
//				break;
//			}
//			count_this++;
//			i--;
//		}
//		i = number.capacity_;
//		while (true)
//		{
//			if (number.value_[i] != 0)
//			{
//				break;
//			}
//			count_number++;
//			i--;
//		}
//		if (this->capacity_ - count_this != number.capacity_ - count_number)
//		{
//			return false;
//		}
//		for (int j = this->capacity_ - count_this; j != -1; j--)
//		{
//			if (this->value_[j] != number.value_[j])
//			{
//				return false;
//			}
//		}
//	}
//	return true;
//}
//
//bool LN::operator<=(const LN &number) const
//{
//	return ((*this) < number) || ((*this) == number);
//}
//
//bool LN::operator>(const LN &number) const
//{
//	return !((*this) < number) && !((*this) == number);
//}
//
//bool LN::operator>=(const LN &number) const
//{
//	return ((*this) > number) || ((*this) == number);
//}
//
//bool LN::operator!=(const LN &number) const
//{
//	return !((*this) == number);
//}
//
//LN LN::operator-() const
//{
//	return negate();
//}
//
//LN LN::operator-(const LN &number) const
//{
//	return subtract(number);
//}
//
//LN LN::operator+(const LN &number) const
//{
//	return add(number);
//}
//
//LN LN::operator/(const LN &number) const
//{
//	return divide(number);
//}
//
//LN LN::operator*(const LN &number) const
//{
//	return multiply(number);
//}
//
//LN LN::operator%(const LN &number) const
//{
//	return mod(number);
//}
//
//LN &LN::operator+=(const LN &number)
//{
//	(*this) = std::move((*this) + number);
//	return (*this);
//}
//
//LN &LN::operator-=(const LN &number)
//{
//	(*this) = std::move((*this) - number);
//	return (*this);
//}
//
//LN &LN::operator*=(const LN &number)
//{
//	(*this) = std::move((*this) * number);
//	return (*this);
//}
//
//LN &LN::operator/=(const LN &number)
//{
//	(*this) = std::move((*this) / number);
//	return (*this);
//}
//
//LN &LN::operator%=(const LN &number)
//{
//	(*this) = std::move((*this) % number);
//	return (*this);
//}
//
//// Сложение.
//LN LN::add(const LN &number) const
//{
//	if ((*this).NaN_ || number.NaN_)
//	{
//		return LN("NaN");
//	}
//	if ((*this).sign_ < 0 && number.sign_ > 0)
//	{
//		return number - (-(*this));
//	}
//	if ((*this).sign_ > 0 && number.sign_ < 0)
//	{
//		return (*this) - (-number);
//	}
//	if ((*this).sign_ < 0 && number.sign_ < 0)
//	{
//		return -(-(*this) + (-number));
//	}
//	size_t max_sz = std::max(this->capacity_, number.capacity_);
//	LN res = LN(max_sz + 1);
//	int carry = 0;
//	int left_dig = 0;
//	int right_dig = 0;
//	int res_dig = 0;
//	for (size_t i = 0; i <= max_sz; i++)
//	{
//		left_dig = this->capacity_ <= i ? 0 : this->value_[i];
//		right_dig = number.capacity_ <= i ? 0 : number.value_[i];
//		res_dig = left_dig + right_dig + carry;
//		carry = res_dig / this->base_;
//		res.value_[i] = res_dig % this->base_;
//	}
//	res.zero_ = 0;
//	return res;
//}
//
//LN LN::subtract(const LN &number) const
//{
//	if ((*this).NaN_ || number.NaN_)
//	{
//		return LN("NaN");
//	}
//	if ((*this).sign_ > 0 && number.sign_ < 0)
//	{
//		return (*this) + -number;
//	}
//	if (this->sign_ < 0 && number.sign_ > 0)
//	{
//		return -(-(*this) + number);
//	}
//	if (this->sign_ < 0 && number.sign_ < 0)
//	{
//		return (-number) - (-(*this));
//	}
//	if (*this < number)
//	{
//		return -(number - (*this));
//	}
//	if (number == *this)
//	{
//		return LN("0");
//	}
//	if ((*this).zero_ == 1)
//	{
//		LN res = number;
//		res.sign_ = res.sign_ > 0 ? -1 : 1;
//		return res;
//	}
//	if ((number).zero_ == 1)
//	{
//		return *this;
//	}
//	size_t max_sz = std::max(this->capacity_, number.capacity_);
//	LN res = LN(max_sz);
//	int carry = 0;
//	int left_dig = 0;
//	int right_dig = 0;
//	int res_dig = 0;
//	for (size_t i = 0; i <= max_sz; i++)
//	{
//		left_dig = this->capacity_ <= i ? 0 : this->value_[i];
//		right_dig = number.capacity_ <= i ? 0 : number.value_[i];
//		res_dig = left_dig - right_dig - carry;
//		if (res_dig < 0)
//		{
//			carry = 1;
//			res_dig += 256;
//			res.value_[i] = res_dig;
//		}
//		else
//		{
//			res.value_[i] = res_dig;
//			carry = 0;
//		}
//	}
//	res.zero_ = 0;
//	res.sign_ = 1;
//	return res;
//}
//
//LN LN::divide(const LN &number) const
//{
//	if ((*this).sign_ < 0 && number.sign_ > 0)
//	{
//		return -((-(*this)) / number);
//	}
//	if ((*this).sign_ > 0 && number.sign_ < 0)
//	{
//		return -((*this) / (-number));
//	}
//	if ((*this).sign_ < 0 && number.sign_ < 0)
//	{
//		return (-(*this) / (-number));
//	}
//	if ((*this).NaN_ || number.NaN_ || number.zero_)
//	{
//		return LN("NaN");
//	}
//	if ((*this).sign_ > 0 && number.sign_ > 0 && (*this) < number)
//	{
//		return LN("0");
//	}
//	if ((*this).sign_ < 0 && number.sign_ < 0 && (*this) > number)
//	{
//		return LN("0");
//	}
//	if (number == LN("1"))
//	{
//		return (*this);
//	}
//	size_t len_this = get_length((*this));
//	size_t len_right = get_length(number);
//	size_t sz = len_this - len_right + 1;
//	LN res = LN(sz + 1);
//	res.zero_ = 0;
//	for (int i = sz - 1; i != -1; i--)
//	{
//		binarySearch(number, res, i);
//	}
//	if (res == LN("0"))
//	{
//		res.zero_ = 1;
//	}
//	return res;
//}
//
//LN LN::multiply(const LN &number) const
//{
//	if ((*this).NaN_ || number.NaN_)
//	{
//		return LN("NaN");
//	}
//	if (this->zero_ || number.zero_)
//	{
//		return LN("0");
//	}
//	if (number == LN("1"))
//	{
//		return (*this);
//	}
//	if ((*this) == LN("1"))
//	{
//		return number;
//	}
//	int left_dig = 0;
//	int right_dig = 0;
//	int res_dig = 0;
//	int carry = 0;
//	size_t tmp_sz = std::max(this->capacity_, number.capacity_);
//	size_t mx_sz = tmp_sz * 2 + 1;
//	LN res = LN(mx_sz);
//	for (int i = 0; i <= tmp_sz; i++)
//	{
//		for (int j = 0; j <= tmp_sz; j++)
//		{
//			right_dig = number.capacity_ <= i ? 0 : number.value_[i];
//			left_dig = this->capacity_ <= j ? 0 : this->value_[j];
//			res_dig = (left_dig * right_dig + carry + res.value_[i + j]);
//			carry = res_dig / this->base_;
//			res.value_[i + j] = res_dig % this->base_;
//		}
//		res.value_[i + tmp_sz + 1] = carry != 0 ? carry : res.value_[i + tmp_sz + 1];
//	}
//	res.zero_ = 0;
//	res.sign_ = (*this).sign_ * number.sign_;
//	return res;
//}
//
//LN LN::sqrt() const
//{
//	if ((*this) == LN("0"))
//	{
//		return LN("0");
//	}
//	if ((*this) < LN("0"))
//	{
//		return LN("NaN");
//	}
//	if ((*this) == LN("1"))
//	{
//		return LN("1");
//	}
//	size_t sz = get_length(*this) / 2 + 1;
//	LN res = LN(sz + 1);
//	res.zero_ = 0;
//	for (int i = sz - 1; i != -1; i--)
//	{
//		binarySearch(res, i);
//	}
//	if (res == LN("0"))
//	{
//		res.zero_ = 1;
//	}
//	return res;
//}
//
//LN LN::mod(const LN &number) const
//{
//	if (number.zero_ || this->NaN_ || number.NaN_ || this->value_ == nullptr || number.value_ == nullptr)
//	{
//		return LN("NaN");
//	}
//	LN prom = (*this) - ((*this) / number) * number;
//	if (this->sign_ > 0 && number.sign_ < 0)
//	{
//		prom.sign_ = 1;
//	}
//	else if (this->sign_ < 0 && number.sign_ > 0)
//	{
//		prom.sign_ = -1;
//	}
//	else if (this->sign_ < 0 && number.sign_ < 0)
//	{
//		if (!prom.zero_)
//		{
//			prom.sign_ = -1;
//		}
//		else
//		{
//			prom.sign_ = 1;
//		}
//	}
//	return prom;	// Проверять ноль ли это не надо, т.к. в вычитании это будет сразу сделано.
//}
//
//LN LN::negate() const
//{
//	if (this->NaN_ || this->value_ == nullptr)
//	{
//		return LN("NaN");
//	}
//	LN newNumber = (*this);
//	newNumber.sign_ = this->sign_ > 0 ? -1 : 1;
//	return newNumber;
//}
//
//void LN::help_out(uint8_t value_)
//{
//	if (value_ >= 10)
//	{
//		std::cout << (char)(value_ + '0' + 7);
//	}
//	else if (value_ > 0)
//	{
//		std::cout << (char)(value_ + '0');
//	}
//	else
//	{
//		std::cout << (char)'0';
//	}
//}
//
//void LN::print(const LN &number)
//{
//	if (number.zero_)
//	{
//		std::cout << "0";
//		return;
//	}
//	if (number.sign_ < 0)
//	{
//		std::cout << "-";
//	}
//	int flag = 0;
//	for (int i = number.size_ - 1; i != -1; i--)
//	{
//		if (flag == 0 && number.value_[i] == 0)
//		{
//			continue;
//		}
//		if (number.value_[i] < 16)
//		{
//			if (flag != 0)
//			{
//				std::cout << (char)'0';
//			}
//			help_out(number.value_[i]);
//			flag = 1;
//		}
//		else
//		{
//			int first = 0;
//			int second = 0;
//			first = number.value_[i] / 16;
//			second = number.value_[i] % 16;
//			flag = 1;
//			help_out(first);
//			help_out(second);
//		}
//	}
//}
//
//LN::~LN()
//{
//	delete[] this->value_;
//	this->capacity_ = 0;
//	this->size_ = 0;
//	this->zero_ = 0;
//	this->NaN_ = 1;
//	this->sign_ = 0;
//}
//
//LN::operator long long() const
//{
//	unsigned long long check_sum = 0;
//	long long number = 0;
//	unsigned long long p = 1;
//	for (size_t i = 0; i < this->capacity_ + 1; i++)
//	{
//		if (this->value_[i] != 0 && p <= 0)
//		{
//			throw OverflowException();
//		}
//		if ((this->value_[i] * p > MAX_LONG && this->sign_ > 0) || (-this->value_[i] * p < MIN_LONG && this->sign_ < 0))
//		{
//			throw OverflowException();
//		}
//		check_sum += this->value_[i] * p;
//		number += this->value_[i] * p;
//		if (check_sum != number)
//		{
//			throw OverflowException();
//		}
//		p *= this->base_;
//		if (this->sign_ < 0 && -number < MIN_LONG)
//		{
//			throw OverflowException();
//		}
//	}
//	return number;
//}
//
//LN::operator bool() const
//{
//	return this->zero_;
//}
//
//LN operator"" _ln(const char *_value_)
//{
//	if (_value_[0] == '0' && _value_[1] == 'x')
//	{
//		_value_ = _value_ + 2;
//	}
//	return LN(_value_);
//}
//
//void LN::hex_to_base(std::string_view number, size_t len)
//{
//	int p = 1;
//	int el = 0;
//	int pos = this->sign_ < 0 ? -1 : 0;
//	uint8_t num = 0;
//	for (int i = len - 1; i != -1 - pos; i--)
//	{
//		if (el == 2)
//		{
//			this->value_[this->size_] = num;
//			p = 1;
//			el = 0;
//			num = 0;
//			this->size_++;
//		}
//		if (number[i] >= 'A')
//		{
//			num += (number[i] - 55) * p;
//		}
//		else
//		{
//			num += (number[i] - '0') * p;
//		}
//		el++;
//		p = 16;
//	}
//	if (el != 0)
//	{
//		this->value_[this->size_] = num;
//		this->size_++;
//	}
//}
//
//void LN::hex_to_base(long long int number, size_t len)
//{
//	int p = 1;
//	int el = 0;
//	uint8_t num = 0;
//	if (number == 0)
//	{
//		this->size_ = 1;
//		this->value_[this->size_] = 0;
//		return;
//	}
//	while (number > 0)
//	{
//		if (el == 2)
//		{
//			this->value_[this->size_] = num;
//			p = 1;
//			el = 0;
//			num = 0;
//			this->size_++;
//		}
//		num += (number % 10) * p;
//		el++;
//		p = 16;
//		number /= 10;
//	}
//	if (el != 0)
//	{
//		this->value_[this->size_] = num;
//		this->size_++;
//	}
//}
//
//size_t LN::get_length(long long int number)
//{
//	if (number == 0)
//	{
//		return 1;
//	}
//	size_t len = 0;
//	while (number > 0)
//	{
//		len++;
//		number /= 10;
//	}
//	return len;
//}
//size_t LN::get_length(const LN &number) const
//{
//	size_t len = 0;
//	int flag = 0;
//	for (int i = number.capacity_; i != -1; i--)
//	{
//		if (number.value_[i] == 0 && flag != 1)
//		{
//			continue;
//		}
//		else if (number.value_[i] != 0)
//		{
//			flag = 1;
//		}
//		len++;
//	}
//	return len;
//}
//
//void LN::fill_zero()
//{
//	for (int i = this->capacity_; i != -1; i--)
//	{
//		this->value_[i] = 0;
//	}
//}
//
//LN LN::operator~() const
//{
//	return sqrt();
//}
//
//std::ostream &operator<<(std::ostream &out, const LN &number)
//{
//	if (number.NaN_ == 1)
//	{
//		out << "NaN\n";
//		return out;
//	}
//	if (number.zero_)
//	{
//		out << "0";
//		return out;
//	}
//	if (number.sign_ < 0)
//	{
//		out << "-";
//	}
//	int flag = 0;
//	for (int i = number.size_ - 1; i != -1; i--)
//	{
//		if (flag == 0 && number.value_[i] == 0)
//		{
//			continue;
//		}
//		if (number.value_[i] < 16)
//		{
//			if (flag != 0)
//			{
//				out << (char)'0';
//			}
//			if (number.value_[i] >= 10)
//			{
//				out << (char)(number.value_[i] + '0' + 7);
//			}
//			else if (number.value_[i] > 0)
//			{
//				out << (char)(number.value_[i] + '0');
//			}
//			else
//			{
//				out << (char)'0';
//			}
//			flag = 1;
//		}
//		else
//		{
//			int first = 0;
//			int second = 0;
//			first = number.value_[i] / 16;
//			second = number.value_[i] % 16;
//			flag = 1;
//			if (first >= 10)
//			{
//				out << (char)(first + '0' + 7);
//			}
//			else if (first > 0)
//			{
//				out << (char)(first + '0');
//			}
//			else
//			{
//				out << (char)'0';
//			}
//			if (second >= 10)
//			{
//				out << (char)(second + '0' + 7);
//			}
//			else if (second > 0)
//			{
//				out << (char)(second + '0');
//			}
//			else
//			{
//				out << (char)'0';
//			}
//		}
//	}
//	return out;
//}
//
//void LN::binarySearch(const LN &number2, LN &res, const int i) const
//{
//	int left = 0;
//	int right = 256;
//	while (right - left > 1)
//	{
//		long long m = (right + left) / 2;
//		res.value_[i] = m;
//		if (res * number2 > (*this))
//		{
//			right = m;
//		}
//		else
//		{
//			left = m;
//		}
//	}
//	res.value_[i] = left;
//}
//
//void LN::binarySearch(LN &res, const int i) const
//{
//	int left = 0;
//	int right = 256;
//	while (right - left > 1)
//	{
//		long long m = (right + left) / 2;
//		res.value_[i] = m;
//		if (res * res > (*this))
//		{
//			right = m;
//		}
//		else
//		{
//			left = m;
//		}
//	}
//	res.value_[i] = left;
//}

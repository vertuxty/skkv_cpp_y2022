#pragma once

#include <string_view>

#include <cmath>
#include <cstdint>

class LN
{
  public:
	LN() = default;

	LN(const LN &);

	LN(LN &&) noexcept;

	explicit LN(long long int expression = 0);

	explicit LN(size_t capacity);

	explicit LN(const char *);

	explicit LN(std::string_view);

	~LN();

	LN operator+(const LN &) const;

	LN operator-(const LN &) const;

	LN operator*(const LN &) const;

	LN operator/(const LN &) const;

	LN operator-() const;

	LN operator~() const;

	LN &operator=(const LN &);

	LN &operator=(LN &&) noexcept;

	bool operator==(const LN &) const;

	bool operator<(const LN &b) const;

	bool operator<=(const LN &) const;

	bool operator>(const LN &) const;

	bool operator>=(const LN &) const;

	bool operator!=(const LN &) const;

	LN &operator+=(const LN &);

	LN &operator-=(const LN &);

	LN &operator*=(const LN &);

	LN &operator/=(const LN &);

	LN &operator%=(const LN &);

	friend std::ostream &operator<<(std::ostream &, const LN &);

	explicit operator long long() const;

	explicit operator bool() const;

	LN operator%(const LN &) const;

	static void print(const LN &);

	void binarySearch(const LN &, LN &, int) const;

	void binarySearch(LN &, int) const;

  private:
	int sign_ = 1;
	int NaN_ = 1;
	int zero_ = 0;
	int base_ = 256;	// Основание С.С. в которой храним наше число. Т.к. я выбрал uint_8, то это 256.
	size_t block_size_ = log2(this->base_) / (4);
	size_t size_ = 0;
	size_t capacity_ = 2;
	uint8_t *value_ = nullptr;

	LN add(const LN &number) const;

	LN divide(const LN &number) const;

	LN multiply(const LN &number) const;

	LN subtract(const LN &number) const;

	LN mod(const LN &number) const;

	LN negate() const;

	LN sqrt() const;

	void hex_to_base(std::string_view number, size_t len);

	void hex_to_base(long long number, size_t len);

	size_t get_length(long long number);

	size_t get_length(const LN &number) const;

	static void help_out(uint8_t value);

	void fill_zero();
	//    void new_size(LN&res);
};

LN operator"" _ln(const char *_value);

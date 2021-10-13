//
// Created by Vislovich on 23.09.2021.
//

#ifndef HW1_BIGINT_H
#define HW1_BIGINT_H

#include <vector>
#include <string>
#include <ostream>


class BigInt {
private:
    std::vector<int8_t> digits;
    bool negative;

    std::vector<int8_t> get_digits(std::string_view);

    BigInt multiplied_by_digit(int8_t) const;

public:

    template<typename Integral,
            typename std::enable_if_t<std::is_integral_v<Integral> && !std::is_same_v<Integral, bool>, Integral> * = nullptr>
    BigInt(Integral a) : BigInt(std::conditional_t<std::is_signed_v<Integral>, int64_t, uint64_t>(a)) {}

    BigInt(uint64_t);

    BigInt(int64_t);

    BigInt(std::string_view);

    BigInt(std::vector<int8_t>, bool);

    friend std::ostream &operator<<(std::ostream &, const BigInt &);

    bool operator<(const BigInt &) const;

    bool operator>(const BigInt &) const;

    bool operator<=(const BigInt &) const;

    bool operator>=(const BigInt &) const;

    bool operator==(const BigInt &) const;

    bool operator!=(const BigInt &) const;

    BigInt operator<<(long long) const;

    BigInt operator++();

    BigInt operator++(int);

    BigInt operator--();

    BigInt operator--(int);

    BigInt operator-();

    BigInt abs() const;

    BigInt &operator+=(const BigInt &);

    BigInt &operator-=(const BigInt &);

    BigInt &operator*=(const BigInt &);

    BigInt &operator=(const BigInt &);

    BigInt operator+(const BigInt &) const;

    BigInt operator-(const BigInt &) const;

    BigInt operator*(const BigInt &) const;

    BigInt operator/(const BigInt &) const;
};


#endif //HW1_BIGINT_H

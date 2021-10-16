//
// Created by Vislovich on 23.09.2021.
//

#ifndef HW1_BIGINT_H
#define HW1_BIGINT_H

#include <vector>
#include <string>
#include <algorithm>
#include <ostream>


class BigInt {
private:
    std::vector<int8_t> digits;
    bool negative;

    BigInt multiplied_by_digit(int8_t) const;

    BigInt as_positive() const;

    BigInt as_negative() const;

    BigInt nth_power_of_10(int64_t) const;

public:

    template<typename Integral,
            typename std::enable_if_t<
                    std::is_integral_v<Integral> && !std::is_same_v<Integral, bool>, Integral> * = nullptr>
    BigInt(Integral a): BigInt(std::conditional_t<std::is_signed_v<Integral>, int64_t, uint64_t>(a)) {};

    BigInt(uint64_t);

    BigInt(int64_t);

    BigInt(std::string_view);

    BigInt(std::vector<int8_t>, bool);

    BigInt(const BigInt &) = default;

    BigInt(BigInt &&) = default;

    BigInt &operator=(const BigInt &) = default;

    BigInt &operator=(BigInt &&) = default;

    friend std::ostream &operator<<(std::ostream &, const BigInt &);

    bool operator<(const BigInt &) const;

    bool operator>(const BigInt &) const;

    bool operator<=(const BigInt &) const;

    bool operator>=(const BigInt &) const;

    bool operator==(const BigInt &) const;

    bool operator!=(const BigInt &) const;

    BigInt operator++();

    BigInt operator++(int);

    BigInt operator--();

    BigInt operator--(int);

    BigInt operator-() const;

    BigInt abs() const;

    BigInt &operator+=(const BigInt &);

    BigInt &operator-=(const BigInt &);

    BigInt &operator*=(const BigInt &);

    BigInt &operator/=(const BigInt &);


    BigInt operator+(const BigInt &) const;

    BigInt operator-(const BigInt &) const;

    BigInt operator*(const BigInt &) const;

    BigInt operator/(const BigInt &) const;
};


#endif //HW1_BIGINT_H

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
    std::vector<short> digits;
    bool negative;

    std::vector<short> get_digits(unsigned long long);

    std::vector<short> get_digits(std::string);

    BigInt(unsigned long long, bool);

    BigInt multiplied_by_digit(short) const;

public:
    template<typename Integral, typename std::enable_if<
            std::is_integral<Integral>::value &&
            !std::is_same<Integral, bool>::value &&
            std::is_signed<Integral>::value, Integral>::type * = nullptr>
    BigInt(Integral a) : BigInt(abs(a), a < 0) {}

    template<typename Integral, typename std::enable_if<
            std::is_integral<Integral>::value &&
            !std::is_same<Integral, bool>::value &&
            !std::is_signed<Integral>::value, Integral>::type * = nullptr>
    BigInt(Integral a) : BigInt(a, false) {}

    BigInt(std::string);

    BigInt(std::vector<short>, bool);

    bool is_negative() const;

    std::vector<short> get_digits() const;

    friend std::ostream &operator<<(std::ostream &, const BigInt &);

    bool operator<(const BigInt &) const;

    bool operator<(const long long &) const;

    bool operator>(const BigInt &) const;

    bool operator>(const long long &) const;

    bool operator<=(const BigInt &) const;

    bool operator<=(const long long &) const;

    bool operator>=(const BigInt &) const;

    bool operator>=(const long long &) const;

    bool operator==(const BigInt &) const;

    bool operator==(const long long &) const;

    bool operator!=(const BigInt &) const;

    bool operator!=(const long long &) const;

    BigInt operator<<(long long) const;

    BigInt operator++();

    BigInt operator++(int);

    BigInt operator--();

    BigInt operator--(int);

    BigInt operator-();

    BigInt &operator+=(const BigInt &);

    BigInt &operator-=(const BigInt &);

    BigInt &operator=(const BigInt &);

    BigInt operator+(const BigInt &) const;

    BigInt operator+(const long long &) const;

    BigInt operator-(const BigInt &) const;

    BigInt operator-(const long long &) const;

    BigInt operator*(const BigInt &) const;

    BigInt operator*(const long long &) const;


};


#endif //HW1_BIGINT_H

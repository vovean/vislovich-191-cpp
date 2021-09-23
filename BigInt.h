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

    std::vector<short> get_digits(long long);

    std::vector<short> get_digits(std::string);

public:
    BigInt(long long);

    BigInt(std::string);

    BigInt(std::vector<short> digits, bool negative);

    bool is_negative() const;

    std::vector<short> get_digits() const;

    bool operator<(const BigInt &rhs) const;

    bool operator>(const BigInt &rhs) const;

    bool operator<=(const BigInt &rhs) const;

    bool operator>=(const BigInt &rhs) const;

    bool operator==(const BigInt &rhs) const;

    bool operator==(const long long &rhs) const;

    friend std::ostream &operator<<(std::ostream &os, const BigInt &anInt);

    friend BigInt operator+(const BigInt &lhs, const BigInt &rhs);

    friend BigInt operator-(const BigInt &lhs, const BigInt &rhs);
};


#endif //HW1_BIGINT_H

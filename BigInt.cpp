//
// Created by Vislovich on 23.09.2021.
//

#include "BigInt.h"
#include <algorithm>
#include <iostream>
#include <utility>

bool BigInt::operator<(const BigInt &rhs) const {
    if (*this == rhs)
        return false;
    if (!negative && rhs.negative)
        return false;
    if (negative && !rhs.negative)
        return true;
    if (digits.size() != rhs.digits.size())
        return (digits.size() > rhs.digits.size()) == negative;
    //compare digit by digit
    int i = 0;
    while (digits[i] == rhs.digits[i])
        i++;
    return (digits[i] > rhs.digits[i]) == negative;
}

bool BigInt::operator>(const BigInt &rhs) const {
    return rhs < *this;
}

bool BigInt::operator<=(const BigInt &rhs) const {
    return *this < rhs || *this == rhs;
}

bool BigInt::operator>=(const BigInt &rhs) const {
    return *this > rhs || *this == rhs;
}

bool BigInt::operator==(const long long int &rhs) const {
    return *this == BigInt(rhs);
}

bool BigInt::operator==(const BigInt &rhs) const {
    return negative == rhs.negative && digits == rhs.digits;
}

std::vector<short> BigInt::get_digits(long long int a) {
    std::vector<short> _digits;
    while (a) {
        _digits.push_back(a % 10);
        a /= 10;
    }
    std::reverse(_digits.begin(), _digits.end());
    return _digits;
}

std::vector<short> BigInt::get_digits(std::string a) {
    int i = 0;
    while (a[i] == '0')
        i++;
    std::vector<short> chars(a.begin() + i, a.end());
    std::for_each(chars.begin(), chars.end(), [&a, this](short &s) {
        if (s < '0' || s > '9') {
            std::cerr << "Invalid value for integer: " << (negative ? "-" : "") << a << std::endl;
            exit(1);
        }
        s -= '0';
    });
    return chars;
}

BigInt::BigInt(long long int a) {
    negative = a < 0;
    digits = get_digits(a >= 0 ? a : -a);
}

BigInt::BigInt(std::string a) {
    negative = a[0] == '-';
    digits = get_digits(a[0] == '-' ? a.substr(1) : a);
}

BigInt::BigInt(std::vector<short> _digits, bool negative) : digits(std::move(_digits)), negative(negative) {
    int i = 0;
    while (digits[i] == 0)
        i++;
    digits = std::vector<short>(digits.begin() + i, digits.end());
}

std::ostream &operator<<(std::ostream &os, const BigInt &bigInt) {
    if (bigInt.negative) {
        os << "-";
    }
    std::vector<short> digits = bigInt.digits;
    std::for_each(digits.begin(), digits.end(), [&os](short &i) {
        os << i;
    });
    return os;
}

BigInt operator+(const BigInt &lhs, const BigInt &rhs) {
    if (lhs.negative)
        return rhs - BigInt(lhs.digits, false);
    if (rhs.negative)
        return lhs - BigInt(rhs.digits, false);
    auto lhs_digits = lhs.digits;
    auto rhs_digits = rhs.digits;
    int size = std::max(lhs_digits.size(), rhs_digits.size());
    auto leading_zeros = std::vector<int>(size - std::min(lhs_digits.size(), rhs_digits.size()), 0);
    if (lhs_digits.size() < size) {
        lhs_digits.insert(lhs_digits.begin(), leading_zeros.begin(), leading_zeros.end());
    } else {
        rhs_digits.insert(rhs_digits.begin(), leading_zeros.begin(), leading_zeros.end());
    }
    std::vector<short> sum(size + 1, 0);
    int t = 0;
    for (int i = size - 1; i >= 0; --i) {
        sum[i + 1] = (lhs_digits[i] + rhs_digits[i] + t) % 10;
        t = (lhs_digits[i] + rhs_digits[i] + t) / 10;
    }
    sum[0] = t;
    return BigInt(sum, false);
}

BigInt operator-(const BigInt &lhs, const BigInt &rhs) {
    // -a - (-b) = -a + b = b - a
    if (lhs.negative && rhs.negative)
        // can use + instead of - right part, but it will be harder to understand
        return BigInt(rhs.digits, false) - BigInt(lhs.digits, false);
    // -a - b = -(a + b)
    if (lhs.negative && !rhs.negative)
        return BigInt(
                (BigInt(lhs.digits, false) + rhs).digits,
                true
        );
    // a - (-b) = a + b
    if (!lhs.negative && rhs.negative)
        return BigInt(
                (lhs + BigInt(rhs.digits, false)).digits,
                false
        );
    // both >= 0
    if (rhs == lhs)
        return BigInt(0);
    if (rhs > lhs)
        return BigInt((rhs - lhs).digits, true);
    // greater minus lower
    auto lhs_digits = lhs.digits;
    auto rhs_digits = rhs.digits;
    auto leading_zeros = std::vector<short>(lhs_digits.size() - rhs_digits.size(), 0);
    rhs_digits.insert(rhs_digits.begin(), leading_zeros.begin(), leading_zeros.end());
    std::vector<short> diff(lhs_digits.size(), 0);
    for (int i = lhs_digits.size() - 1; i >= 0; --i) {
        if (lhs_digits[i] >= rhs_digits[i]) {
            diff[i] = lhs_digits[i] - rhs_digits[i];
            continue;
        }
        lhs_digits[i - 1] -= 1;
        diff[i] = lhs_digits[i] - rhs_digits[i] + 10;
    }
    return BigInt(diff, false);
}

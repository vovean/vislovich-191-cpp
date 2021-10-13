//
// Created by Vislovich on 23.09.2021.
//

#include "BigInt.h"
#include <algorithm>
#include <iostream>

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

bool BigInt::operator==(const BigInt &rhs) const {
    return negative == rhs.negative && digits == rhs.digits;
}

std::vector<int8_t> BigInt::get_digits(std::string_view a) {
    int i = 0;
    while (a[i] == '0')
        i++;
    std::vector<int8_t> chars(a.begin() + i, a.end());
    std::for_each(chars.begin(), chars.end(), [&a, this](int8_t &s) {
        if (s < '0' || s > '9') {
            std::cerr << "Invalid value for integer: " << (negative ? "-" : "") << a << std::endl;
            exit(1);
        }
        s -= '0';
    });
    return chars;
}

BigInt::BigInt(int64_t a) {
    negative = a < 0;
    while (a) {
        digits.emplace_back(std::abs(a % 10));
        a /= 10;
    }
    std::reverse(digits.begin(), digits.end());
    if (digits.empty()) {
        digits = {0};
    }
}

BigInt::BigInt(uint64_t a) {
    negative = false;
    while (a) {
        digits.emplace_back(a % 10);
        a /= 10;
    }
    std::reverse(digits.begin(), digits.end());
    if (digits.empty()) {
        digits = {0};
    }
}

BigInt::BigInt(std::string_view a) {
    if (a[0] == '-') {
        negative = true;
        a = a.substr(1);
    } else {
        negative = false;
    }
    if (a.empty()){
        digits = {0};
        return;
    }
    int i = 0;
    while (a[i] == '0')
        i++;
    std::for_each(a.begin() + i, a.end(), [&a, this](const char &s) {
        if (s < '0' || s > '9') {
            std::cerr << "Invalid value for integer: " << (negative ? "-" : "") << a << std::endl;
            exit(1);
        }
        digits.emplace_back(s - '0');
    });

}

BigInt::BigInt(std::vector<int8_t> _digits, bool negative) : digits(std::move(_digits)), negative(negative) {
    int i = 0;
    while (digits[i] == 0 && i < digits.size())
        i++;
    if (i != digits.size())
        digits = std::vector<int8_t>(digits.begin() + i, digits.end());
    else
        digits = std::vector<int8_t>(1, 0);
}

std::ostream &operator<<(std::ostream &os, const BigInt &bigInt) {
    if (bigInt.negative) {
        os << "-";
    }
    std::vector<int8_t> digits = bigInt.digits;
    std::for_each(digits.begin(), digits.end(), [&os](int8_t &i) {
        os << +i;
    });
    return os;
}

BigInt BigInt::operator+(const BigInt &rhs) const {
    if (negative)
        return rhs - BigInt(digits, false);
    if (rhs.negative)
        return *this - BigInt(rhs.digits, false);
    auto lhs_digits = digits;
    auto rhs_digits = rhs.digits;
    int size = std::max(lhs_digits.size(), rhs_digits.size());
    auto leading_zeros = std::vector<int>(size - std::min(lhs_digits.size(), rhs_digits.size()), 0);
    if (lhs_digits.size() < size) {
        lhs_digits.insert(lhs_digits.begin(), leading_zeros.begin(), leading_zeros.end());
    } else {
        rhs_digits.insert(rhs_digits.begin(), leading_zeros.begin(), leading_zeros.end());
    }
    std::vector<int8_t> sum(size + 1, 0);
    int t = 0;
    for (int i = size - 1; i >= 0; --i) {
        sum[i + 1] = (lhs_digits[i] + rhs_digits[i] + t) % 10;
        t = (lhs_digits[i] + rhs_digits[i] + t) / 10;
    }
    sum[0] = t;
    return BigInt(sum, false);
}

BigInt BigInt::operator-(const BigInt &rhs) const {
    // -a - (-b) = -a + b = b - a
    if (negative && rhs.negative)
        // can use + instead of - right part, but it will be harder to understand
        return BigInt(rhs.digits, false) - BigInt(digits, false);
    // -a - b = -(a + b)
    if (negative && !rhs.negative)
        return BigInt(
                (BigInt(digits, false) + rhs).digits,
                true
        );
    // a - (-b) = a + b
    if (!negative && rhs.negative)
        return BigInt(
                (*this + BigInt(rhs.digits, false)).digits,
                false
        );
    // both >= 0
    if (rhs == *this)
        return BigInt(0);
    if (rhs > *this)
        return BigInt((rhs - *this).digits, true);
    // greater minus lower
    auto lhs_digits = digits;
    auto rhs_digits = rhs.digits;
    auto leading_zeros = std::vector<int8_t>(lhs_digits.size() - rhs_digits.size(), 0);
    rhs_digits.insert(rhs_digits.begin(), leading_zeros.begin(), leading_zeros.end());
    std::vector<int8_t> diff(lhs_digits.size(), 0);
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

bool BigInt::operator!=(const BigInt &rhs) const {
    return !(*this == rhs);
}

BigInt &BigInt::operator=(const BigInt &other) = default;

BigInt BigInt::operator++() {
    *this = *this + 1;
    return *this;
}

BigInt BigInt::operator++(int a) {
    BigInt tmp = *this;
    *this = *this + 1;
    return tmp;
}

BigInt BigInt::operator--() {
    *this = *this - 1;
    return *this;
}

BigInt BigInt::operator--(int a) {
    BigInt tmp = *this;
    *this = *this - 1;
    return tmp;
}

BigInt BigInt::operator-() {
    return BigInt(digits, !negative);
}

BigInt BigInt::operator*(const BigInt &rhs) const {
    if (*this < rhs)
        return rhs * *this;
    bool negativity = negative ^ rhs.negative;
    auto rhs_digits = rhs.digits;

    BigInt sum(0);
    for (int i = 0; i < rhs_digits.size(); ++i) {
        sum += multiplied_by_digit(rhs_digits[rhs_digits.size() - i - 1]) << i;
    }
    return BigInt(sum.digits, negativity);
}

BigInt BigInt::multiplied_by_digit(int8_t digit) const {
    std::vector<int8_t> lhs_digits(digits.begin(), digits.end());
    auto rhs_digits = std::vector<int8_t>(lhs_digits.size(), digit);

    std::vector<int8_t> new_digits;
    std::transform(lhs_digits.begin(), lhs_digits.end(),
                   rhs_digits.begin(),
                   std::back_inserter(new_digits),
                   std::multiplies<>());
    for (int i = new_digits.size() - 1; i > 0; --i) {
        if (new_digits[i] > 9) {
            new_digits[i - 1] += new_digits[i] / 10;
            new_digits[i] %= 10;
        }
    }
    if (new_digits[0] > 9) {
        new_digits.insert(new_digits.begin(), new_digits[0] / 10);
        new_digits[1] %= 10;
    }
    return BigInt(new_digits, negative);
}

BigInt BigInt::operator<<(const long long n) const {
    if (n > 0) {
        auto digits_copy = digits;
        std::vector<int8_t> zeros(n, 0);
        digits_copy.insert(digits_copy.end(), zeros.begin(), zeros.end());
        return BigInt(digits_copy, negative);
    }
    return *this;
}

BigInt &BigInt::operator+=(const BigInt &rhs) {
    *this = *this + rhs;
    return *this;
}

BigInt &BigInt::operator-=(const BigInt &rhs) {
    *this = *this - rhs;
    return *this;
}

BigInt &BigInt::operator*=(const BigInt &rhs) {
    *this = *this * rhs;
    return *this;
}

BigInt BigInt::abs() const {
    return BigInt(digits, false);
}

/*
 * Important to notice, that the integral division here is performed in absolute values, in other words, towards zero
 * Examples:
 * >  3 /   2  ==  1
 * > -3 /   2  == -1
 * >  3 / (-2) == -1
 * > -3 / (-2) ==  1
 */
BigInt BigInt::operator/(const BigInt &rhs) const {
    if (abs() < rhs.abs())
        return BigInt(0);
    bool negativity = negative ^ rhs.negative;
    BigInt rhs_abs = rhs.abs();
    BigInt ans(0), tmp(digits[0]);
    for (int i = 1; i < digits.size(); ++i) {
        tmp *= 10;
        tmp += digits[i];
        if (tmp < rhs_abs) {
            ans *= 10;
            continue;
        }
        int c = 0;
        // happens <= 10 times
        while (tmp >= rhs_abs) {
            tmp -= rhs_abs;
            c++;
        }
        ans *= 10;
        ans += c;
    }
    // lhs is 1-digit BigInt
    if (tmp >= rhs_abs) {
        int c = 0;
        while (tmp >= rhs_abs) {
            tmp -= rhs_abs;
            c++;
        }
        ans += c;
    }
    return BigInt(ans.digits, negativity);
}

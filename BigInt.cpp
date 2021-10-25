//
// Created by Vislovich on 23.09.2021.
//

#include "BigInt.h"
#include <algorithm>
#include <iostream>

namespace {
    template<typename T>
    void set_digits(T number, std::vector<int8_t> &digits) {
        while (number) {
            digits.emplace_back(::abs(number % 10));
            number /= 10;
        }
        std::reverse(digits.begin(), digits.end());
        if (digits.empty()) {
            digits = {0};
        }
    }

    void add_digits(std::vector<int8_t> &this_digits, const std::vector<int8_t> &other_digits) {
        if (this_digits.size() < other_digits.size()) {
            auto leading_zeros = std::vector<int8_t>(other_digits.size() - this_digits.size(), 0);
            this_digits.insert(this_digits.begin(), leading_zeros.begin(), leading_zeros.end());
        }
        int shift = this_digits.size() - other_digits.size();
        // add digits corresponding digits from end to beginning
        int8_t t = 0;
        for (int i = other_digits.size() - 1; i >= 0; --i) {
            this_digits[shift + i] += other_digits[i] + t;
            t = this_digits[shift + i] / 10;
            this_digits[shift + i] %= 10;
        }
        // in case 2nd num is shorter than second and t=1, i.e. 999 + 2
        while (t != 0 && shift > 0) {
            this_digits[shift - 1] += t;
            t = this_digits[shift - 1] / 10;
            this_digits[shift - 1] %= 10;
            shift--;
        }
        if (t != 0) {
            this_digits.insert(this_digits.begin(), t);
        }
    }

    void subtract_digits(std::vector<int8_t> &this_digits, const std::vector<int8_t> &other_digits) {
        int shift = this_digits.size() - other_digits.size();
        std::vector<int8_t> diff(this_digits.size(), 0);
        for (int i = other_digits.size() - 1; i >= 0; --i) {
            if (this_digits[shift + i] >= other_digits[i]) {
                this_digits[shift + i] -= other_digits[i];
                continue;
            }
            this_digits[shift + i - 1] -= 1;
            this_digits[shift + i] = this_digits[shift + i] - other_digits[i] + 10;
        }
        while (this_digits[shift - 1] < 0 && shift > 0) {
            this_digits[shift - 1] += 10;
            this_digits[shift - 2] -= 1;
            shift--;
        }
        while (this_digits[0] == 0)
            this_digits.erase(this_digits.begin());
    }
}

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

BigInt::BigInt(std::string_view a) {
    if (a.empty()) {
        digits = {0};
        negative = false;
    }
    if (a[0] == '-') {
        negative = true;
        a = a.substr(1);
    } else {
        negative = false;
    }
    if (a.empty()) {
        digits = {0};
        return;
    }
    int i = 0;
    while (a[i] == '0')
        i++;
    std::for_each(a.begin() + i, a.end(), [&a, this](const char &s) {
        if (s < '0' || s > '9') {
            std::cerr << "Invalid value for integer: " << (negative ? "-" : "") << a << std::endl;
            throw std::invalid_argument(std::string(a.begin(), a.end()));
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
    bigInt.print_to_os(os);
    return os;
}

BigInt BigInt::operator+(const BigInt &rhs) const {
    BigInt tmp = *this;
    tmp += rhs;
    return tmp;
}

BigInt &BigInt::operator+=(const BigInt &rhs) {
    if (negative)
        *this = std::move(rhs - as_positive());
    else if (rhs.negative)
        *this = std::move(*this - BigInt(rhs.digits, false));
    else
        add_digits(digits, rhs.digits);
    return *this;
}

BigInt BigInt::operator-(const BigInt &rhs) const {
    BigInt left = *this;
    left -= rhs;
    return left;
}

BigInt &BigInt::operator-=(const BigInt &rhs) {
    if (negative && rhs.negative) // -a - (-b) = -a + b = b - a
        *this = rhs.as_positive() - as_positive();
    else if (negative && !rhs.negative) // -a - b = -(a + b)
        add_digits(digits, rhs.digits);
    else if (!negative && rhs.negative) // a - (-b) = a + b
        add_digits(digits, rhs.digits);
    else { // both >= 0
        if (rhs == *this)
            *this = 0;
        else if (rhs > *this)
            *this = (rhs - *this).as_negative();
        else
            subtract_digits(digits, rhs.digits);
    }
    return *this;
}

bool BigInt::operator!=(const BigInt &rhs) const {
    return !(*this == rhs);
}

BigInt &BigInt::operator++() {
    *this += 1;
    return *this;
}

BigInt BigInt::operator++(int) {
    BigInt tmp = *this;
    *this += 1;
    return tmp;
}

BigInt &BigInt::operator--() {
    *this -= 1;
    return *this;
}

BigInt BigInt::operator--(int a) {
    BigInt tmp = *this;
    *this -= 1;
    return tmp;
}

BigInt BigInt::operator-() const {
    return {digits, !negative};
}

BigInt BigInt::operator*(const BigInt &rhs) const {
    BigInt left = *this;
    left *= rhs;
    return left;
}

BigInt &BigInt::operator*=(const BigInt &rhs) {
    if (*this < rhs)
        *this = rhs * *this;
    else {
        negative ^= rhs.negative;

        BigInt sum(0);
        for (int i = 0; i < rhs.digits.size(); ++i) {
            sum += multiplied_by_digit(rhs.digits[rhs.digits.size() - i - 1]).nth_power_of_10(i);
        }
        *this = std::move(sum);
    }
    return *this;
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
    return {new_digits, negative};
}

BigInt BigInt::abs() const {
    return {digits, false};
}


BigInt BigInt::operator/(const BigInt &rhs) const {
    BigInt left = *this;
    left /= rhs;
    return left;
}

/*
 * Important to notice, that the integral division here is performed in absolute values, in other words, towards zero
 * Examples:
 * >  3 /   2  ==  1
 * > -3 /   2  == -1
 * >  3 / (-2) == -1
 * > -3 / (-2) ==  1
 */
BigInt &BigInt::operator/=(const BigInt &rhs) {
    if (rhs == 0)
        throw std::invalid_argument("Division by 0 error");
    if (abs() < rhs.abs())
        *this = 0;
    else {
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
        // lhs is 1-digit BigInt, that means rhs is also 1-digit BigInt
        if (tmp >= rhs_abs) {
            ans = digits[0] / rhs.digits[0];
        }
        ans.negative = negative ^ rhs.negative;
        *this = std::move(ans);
    }
    return *this;
}

BigInt::BigInt(int64_t a) : negative(a < 0) {
    set_digits(a, digits);
}

BigInt::BigInt(uint64_t a) : negative(a < 0) {
    set_digits(a, digits);
}

BigInt BigInt::as_positive() const {
    return {digits, false};
}

BigInt BigInt::as_negative() const {
    return {digits, true};
}

// keeps old value for n = 0
BigInt BigInt::nth_power_of_10(int64_t n) const {
    if (n > 0) {
        auto digits_copy = digits;
        std::vector<int8_t> zeros(n, 0);
        digits_copy.insert(digits_copy.end(), zeros.begin(), zeros.end());
        return {digits_copy, negative};
    }
    return *this;
}

void BigInt::print_to_os(std::ostream &os) const {
    if (negative) {
        os << "-";
    }
    std::for_each(digits.begin(), digits.end(), [&os](const int8_t &i) {
        os << +i;
    });
}

BigInt &BigInt::operator%=(const BigInt &rhs) {
    *this -= (*this) / rhs * rhs;
    return *this;
}

BigInt BigInt::operator%(const BigInt &rhs) const {
    BigInt tmp(*this);
    tmp %= rhs;
    return tmp;
}

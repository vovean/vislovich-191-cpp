//
// Created by Vislovich on 08.10.2021.
//

#ifndef HW1_MATHPARSER_H
#define HW1_MATHPARSER_H

#include <algorithm>
#include <iostream>
#include <string>
#include <cctype>
#include <iterator>
#include "BigInt.h"

class Exp {
public:
    virtual std::ostream &print(std::ostream &os) = 0;

    virtual void release() = 0;

    friend std::ostream &operator<<(std::ostream &, Exp &);

    virtual BigInt get_value() = 0;
};

class Term : public Exp {
    BigInt val;
public:
    Term(const std::string &v) : val(v) {}

    std::ostream &print(std::ostream &os) override;

    void release() override {}

    BigInt get_value() override;
};

class Node : public Exp {
    Exp *l_exp;
    Exp *r_exp;
    char op; // +, -, *, /
public:
    Node(char op, Exp *left, Exp *right) : op(op), l_exp(left), r_exp(right) {}

    ~Node() = default;

    std::ostream &print(std::ostream &os) override;

    void release() override;

    BigInt get_value() override;
};

Exp *parse_expression(std::string &str);

#endif //HW1_MATHPARSER_H

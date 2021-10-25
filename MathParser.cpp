//
// Created by Vislovich on 22.10.2021.
//

#include "MathParser.h"

std::ostream &Term::print(std::ostream &os) {
    if (val < 0)
        os << '(' << val << ')';
    else
        os << val;
    return os;
}

BigInt Term::get_value() {
    return val;
}

std::ostream &Node::print(std::ostream &os) {
    os << "( " << op << ' ';
    l_exp->print(os);
    os << ' ';
    r_exp->print(os);
    os << " )";
    return os;
}

void Node::release() {
    l_exp->release();
    r_exp->release();
    delete l_exp;
    delete r_exp;
}

BigInt Node::get_value() {
    switch (op) {
        case '+':
            return l_exp->get_value() + r_exp->get_value();
        case '-':
            return l_exp->get_value() - r_exp->get_value();
        case '*':
            return l_exp->get_value() * r_exp->get_value();
        case '/':
            return l_exp->get_value() / r_exp->get_value();
        default: // %
            return l_exp->get_value() % r_exp->get_value();
    }
}

std::ostream &operator<<(std::ostream &os, Exp &exp) {
    exp.print(os);
    return os;
}

Exp *parse_expression(std::string &str) {
    int level = 0; // to track ()
    // process +, - from right to left
    for (int i = str.size() - 1; i >= 0; --i) {
        char c = str[i];
        if (c == ')') {
            ++level;
            continue;
        }
        if (c == '(') {
            --level;
            continue;
        }
        if (level > 0) continue;
        if ((c == '+' || c == '-') && i != 0) { // if i==0 then s[0] is a sign
            std::string left(str.substr(0, i));
            std::string right(str.substr(i + 1));
            return new Node(c, parse_expression(left), parse_expression(right));
        }
    }
    // process *, /, % from right to left
    for (int i = str.size() - 1; i >= 0; --i) {
        char c = str[i];
        if (c == ')') {
            ++level;
            continue;
        }
        if (c == '(') {
            --level;
            continue;
        }
        if (level > 0) continue;
        if (c == '*' || c == '/' || c == '%') {
            std::string left(str.substr(0, i));
            std::string right(str.substr(i + 1));
            return new Node(c, parse_expression(left), parse_expression(right));
        }
    }
    // process expression in ()
    if (str[0] == '(') {
        for (int i = 0; i < str.size(); ++i) {
            if (str[i] == '(') {
                ++level;
                continue;
            }
            if (str[i] == ')') {
                --level;
                if (level == 0) {
                    std::string exp(str.substr(1, i - 1));
                    return parse_expression(exp);
                }
                continue;
            }
        }
    } else
        // it is a number
        return new Term(str);
    std::cerr << "If u see it then something has totally broken" << std::endl;
    return nullptr;
}

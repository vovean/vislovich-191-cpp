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
//  Exp(){}
    virtual void print() {}

    virtual void release() {}
};

class Term : public Exp {
    BigInt val;
public:
    Term(std::string v) : val(v) {}

    void print() override {
        std::cout << ' ' << val << ' ';
    }

    void release() override {}
};

class Node : public Exp {
    Exp *l_exp;
    Exp *r_exp;
    char op; // +, -, *, /
public:
    Node(char op, Exp *left, Exp *right) : op(op), l_exp(left), r_exp(right) {}

    ~Node() = default;

    void print() override {
        std::cout << '(' << op << ' ';
        l_exp->print();
        r_exp->print();
        std::cout << ')';
    }

    void release() override {
        l_exp->release();
        r_exp->release();
        delete l_exp;
        delete r_exp;
    }
};

Exp *strToExp(std::string &str) {
    int level = 0;//inside parentheses check
    //case + or -
    //most right '+' or '-' (but not inside '()') search and split
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
        if ((c == '+' || c == '-') && i != 0) {//if i==0 then s[0] is sign
            std::string left(str.substr(0, i));
            std::string right(str.substr(i + 1));
            return new Node(c, strToExp(left), strToExp(right));
        }
    }
    //case * or /
    //most right '*' or '/' (but not inside '()') search and split
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
        if (c == '*' || c == '/') {
            std::string left(str.substr(0, i));
            std::string right(str.substr(i + 1));
            return new Node(c, strToExp(left), strToExp(right));
        }
    }
    if (str[0] == '(') {
        //case ()
        //pull out inside and to strToExp
        for (int i = 0; i < str.size(); ++i) {
            if (str[i] == '(') {
                ++level;
                continue;
            }
            if (str[i] == ')') {
                --level;
                if (level == 0) {
                    std::string exp(str.substr(1, i - 1));
                    return strToExp(exp);
                }
                continue;
            }
        }
    } else
        //case value
        return new Term(str);
    std::cerr << "Error:never execute point" << std::endl;
    return NULL;//never
}

#endif //HW1_MATHPARSER_H

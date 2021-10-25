#include <iostream>
#include "BigInt.h"
#include "MathParser.h"

Exp *read_exp() {
    std::string input;
    getline(std::cin, input);
    input.erase(remove(input.begin(), input.end(), ' '), input.end());
    return parse_expression(input);
}

int main() {
    std::cout << "Enter an expression to calculate\n"
                 "Available operators: +, -, *, /, %\n"
                 "All negative numbers should be wrapped in parentheses\n";
    Exp *exp = read_exp();
    std::cout << "The answer is " << exp->get_value();


    return 0;
}

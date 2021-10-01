#include <iostream>
#include "BigInt.h"

int main() {
    std::string a, b;
    std::cout << "Enter 2 valid very long numbers, may start with \"-\": " << std::endl;
    std::cin >> a >> b;
    BigInt bi_a(a), bi_b(b);
    std::cout << "a + b = " << bi_a + bi_b << std::endl;
    std::cout << "a - b = " << bi_a - bi_b << std::endl;
    std::cout << "a * b = " << bi_a * bi_b << std::endl;
    std::cout << "a / b = " << bi_a / bi_b << std::endl;
    std::cout << "b / a = " << bi_b / bi_a << std::endl;
    std::cout << "a > b = " << (bi_a > bi_b) << std::endl;
    std::cout << "a < b = " << (bi_a < bi_b) << std::endl;
    std::cout << "a >= b = " << (bi_a >= bi_b) << std::endl;
    std::cout << "a <= b = " << (bi_a <= bi_b) << std::endl;
    std::cout << "a == b = " << (bi_a == bi_b) << std::endl;


    return 0;
}

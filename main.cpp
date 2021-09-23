#include <iostream>
#include "BigInt.h"

int main() {
    BigInt a("123"), b(-124);

    std::cout << a << " " << b << " " << (a >= b);

    return 0;
}

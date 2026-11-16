#include <iostream>
#include "Number.h"

int main() {
    Number num(0);
    num = -67;

    Number sixseven(0);
    sixseven = 67;

    Number num1(0);
    num1 = 2;

    Number zero(0);
    zero = 0;

    Number result = num + num1;
    std::cout << result.toStr() << std::endl;
    result = num - num1;
    std::cout << result.toStr() << std::endl;
    result = num1 - num;
    std::cout << result.toStr() << std::endl;
    result = num * num1;
    std::cout << result.toStr() << std::endl;
    result = num / num1;
    std::cout << result.toStr() << std::endl;
    result = num % num1;
    std::cout << num(0) << std::endl;
    std::cout << sixseven(0) << std::endl;
}

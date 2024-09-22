#include <iostream>
#include "PrimeChecker.h"

int main() {
    int val;
    std::cout << "Enter a number: ";
    std::cin >> val;

    bool result = PrimeChecker::IsPrime(val);
    std::cout << (result ? "Prime" : "Not Prime") << std::endl;

    return 0;
}
#include <iostream>
#include "is_prime.h"

int main()
{
    std::cout << "Please input a number";
    std::cin >> i;
    std::cout << "The number " << i << " is prime: " << is_prime(i);
}
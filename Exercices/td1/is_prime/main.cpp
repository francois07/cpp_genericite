#include <iostream>
#include "is_prime.h"

int main()
{
    int i;

    std::cout << "Please input a number: ";
    while (std::cin >> i)
    {
        std::cout << "The number " << i << " is prime: " << std::boolalpha << is_prime(i) << std::endl;
    }

    return 0;
}
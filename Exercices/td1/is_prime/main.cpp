#include <iostream>
#include "is_prime.hh"

int main()
{
    int i;

    while (std::cin >> i)
    {
        std::cout << i << " is a prime: " << std::boolalpha << is_prime(i) << std::endl;
    }

    return 0;
}
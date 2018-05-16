#ifndef NOISY_H
#define NOISY_H

#include <iostream>

class Noisy {
public:
    Noisy (): value(0)
    {
        std::cout << "noisy default constructor" << std::endl;
    }

    Noisy(int value): value(value)
    {
        std::cout << "noisy value constructor" << std::endl;
    }

    Noisy(const Noisy& other): value(other.value)
    {
        noisy++;
        std::cout << "noisy copy constructor" << std::endl;
    }

    Noisy(Noisy&& other): value(other.value)
    {
            other.value = 0;
            std::cout << "noisy move constructor" << std::endl;
    }

    Noisy& operator= (const Noisy& other) {
        value = other.value;
        noisy++;
        std::cout << "noisy copy assign" << std::endl;
        return *this;
    }

    bool operator==(const Noisy& other) const {
        return value == other.value;
    }

    int value;
    static int noisy;

};

#endif // NOISY_H

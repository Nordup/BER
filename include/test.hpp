#ifndef TEST_HPP
#define TEST_HPP

#include "header.hpp"

void test_print_Vector(std::vector<unsigned char> data)
{
    std::cout << "vector data:" << std::endl;
    for (auto& ch: data)
    {
        std::cout << ch;
    }
    std::cout << std::endl;
}

#endif
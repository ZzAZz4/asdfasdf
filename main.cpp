#include <iostream>
#include "include/scanner.hpp"

int main ()
{
    std::string_view sv = "neun";
    int c = match(sv);
    std::cout << TOKEN_VAL[c];
}
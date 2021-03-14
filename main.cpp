#include <iostream>
#include "include/scanner.hpp"

int main ()
{
    std::string_view sv = "neun";
    auto c = match(sv);

    if (!(c & Token::ERR_BIT))
        std::cout << getVal(c);
    else
        std::cerr << "Error: expected " << getName(c & ~(Token::ERR_BIT));
}
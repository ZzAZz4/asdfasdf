#include <iostream>
#include "include/scanner.hpp"

int main ()
{
    using namespace Grammar;
    auto i = Lexer::lex("zwoelfasdfzehnokokzehnknvdnvdkn");
    auto it = "z1278911";
    auto a = toEnum(it);
    std::cout << std::boolalpha << (a == Z1278911);

    return 0;
}
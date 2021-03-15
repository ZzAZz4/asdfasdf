#include <iostream>
#include <bitset>
#include "include/scanner.hpp"

int main ()
{
    // this grammar is not faulty, so it passes. yay!
//    std::string s = "zweihundertzweiundzwanzigtausendvierhundertsiebzehn";
//    Lexer lexer(s);
//    auto lexemes = lexer.lex();
//    for (auto lexeme : lexemes)
//        std::cout << (int) lexeme.token << ' ' << lexeme.str() << ' ' << lexeme.value() << '\n';
//    std::cout << static_cast<Grammar::Token> (30) << ' ';

    // this grammar is faulty as hell
    std::string s = "puddingpuddingnepupudding";
    Lexer lexer(s);
    auto lexemes = lexer.lex();

    std::cerr.flush();

    // this one is the first one but has a typo
//    std::string s = "zweihundertzwpiundzwanzigtausendvierhundertsiebzehn";
//    Lexer lexer(s);
//    auto lexemes = lexer.lex();
//    for (auto lexeme : lexemes)
//        std::cout << (int) lexeme.token << ' ' << lexeme.str << ' ' << lexeme.value << '\n';


}
#include <iostream>
#include <bitset>
#include "include/scanner.hpp"
#include "include/parser.hpp"
#include "json/json.h"


void addRule (Rules& rules, ID lhs, Rule rhs)
{
    rules.emplace(lhs, std::move(rhs));
}



int main ()
{
    SLRParser s;
    auto l = Lexer::lex("fuenftausendzweihundertneinundfuenfdrei");
    auto r = s.parse(l);
    std::cout << std::boolalpha << r;

//     this grammar is not faulty, so it passes. yay!
//    std::string s = "zweihundertzweiundzwanzigtausendvierhundertsiebzehn";
//    Lexer lexer(s);
//    auto lexemes = lexer.lex();
//    for (auto lexeme : lexemes)
//        std::cout << (int) lexeme.token << ' ' << lexeme.str() << ' ' << lexeme.value() << '\n';

//    // this grammar is faulty as hell
//    std::string s = "puddingpuddingnepupudding";
//    Lexer lexer(s);
//    auto lexemes = lexer.lex();
//
//    std::cerr.flush();

    // this one is the first one but has a typo
//    std::string s = "zweihundertzwpiundzwanzigtausendvierhundertsiebzehn";
//    Lexer lexer(s);
//    auto lexemes = lexer.lex();
//    for (auto lexeme : lexemes)
//        std::cout << (int) lexeme.token << ' ' << lexeme.str << ' ' << lexeme.value << '\n';

    return 0;
}
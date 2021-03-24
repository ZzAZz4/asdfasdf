#include <iostream>
#include <bitset>
#include "include/scanner.hpp"
#include "include/parser.hpp"

int main()
{
    SLRParser s;
    string entry;
    cin >> entry;
    auto l = Lexer::lex(entry);

    auto r = s.parse(l);
    if (!r)
    {
        int pos = 0;
        auto errIt = s.errors.begin();

        for (int i = 0; i < l.size() && errIt != s.errors.end(); ++i)
        {
            if (i == *errIt)
            {
                cerr << "Syntax error: at position: " << pos << '\n';
                cerr << '\t' << l[i].strVal << '\n';
            }
            pos += l[i].strVal.size();
        }
    }
    std::cout << std::boolalpha << "Accepted: " << r << '\n';

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
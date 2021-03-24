#include <iostream>
#include <bitset>
#include "include/scanner.hpp"
#include "include/parser.hpp"
#include "json/json.h"

int levDistance(std::string source,std::string target)
{

    // Step 1

    const int n = source.length();
    const int m = target.length();
    if (n == 0) {
        return m;
    }
    if (m == 0) {
        return n;
    }

    // Good form to declare a TYPEDEF

    typedef std::vector< std::vector<int>> Tmatrix;

    Tmatrix matrix(n+1);

    // Size the vectors in the 2.nd dimension. Unfortunately C++ doesn't
    // allow for allocation on declaration of 2.nd dimension of vec of vec

    for (int i = 0; i <= n; i++) {
        matrix[i].resize(m+1);
    }

    // Step 2

    for (int i = 0; i <= n; i++) {
        matrix[i][0]=i;
    }

    for (int j = 0; j <= m; j++) {
        matrix[0][j]=j;
    }

    // Step 3

    for (int i = 1; i <= n; i++) {

        const char s_i = source[i-1];

        // Step 4

        for (int j = 1; j <= m; j++) {

            const char t_j = target[j-1];

            // Step 5

            int cost;
            if (s_i == t_j) {
                cost = 0;
            }
            else {
                cost = 1;
            }

            // Step 6

            const int above = matrix[i-1][j];
            const int left = matrix[i][j-1];
            const int diag = matrix[i-1][j-1];
            int cell = std::min( above + 1, std::min(left + 1, diag + cost));

            // Step 6A: Cover transposition, in addition to deletion,
            // insertion and substitution. This step is taken from:
            // Berghel, Hal ; Roach, David : "An Extension of Ukkonen's
            // Enhanced Dynamic Programming ASM Algorithm"
            // (http://www.acm.org/~hlb/publications/asm/asm.html)

            if (i>2 && j>2) {
                int trans=matrix[i-2][j-2]+1;
                if (source[i-2]!=t_j) trans++;
                if (s_i!=target[j-2]) trans++;
                if (cell>trans) cell=trans;
            }

            matrix[i][j]=cell;
        }
    }

    // Step 7

    return matrix[n][m];
}

string test(std::string str){
    int tempRes = 0;
    int temp = INT_MAX;
    for(int i = 0; i < Grammar::NUM_TOKENS;i++){
        auto res = levDistance(str, static_cast<const string>(Grammar::TOKEN_STR[i]));
        if(temp > res) {temp = res; tempRes = i;}
    }

    return string(Grammar::TOKEN_STR[tempRes]);
}

void errorInToken(const std::vector<Lexer::Item> & v){
    for(auto it: v){
        if(it.token == Grammar::$ERROR){
            cerr<<"did you meant  to say:"<<it.strVal<<" -> "<<test(string(it.strVal))<<'\n';
        }
    }
}



int main ()
{
    SLRParser s;
    auto l = Lexer::lex("fuenftausendzweihundertneunundfuenfzig");
    errorInToken(l);
    auto r = s.parse(l);
    if(!r){
        cerr<<"semantic errors at index :";
        for(auto it: s.errors){
            cerr<<it<<" ";
        }
        cerr<<"\n";
    }
    std::cout << std::boolalpha << r<<'\n';

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
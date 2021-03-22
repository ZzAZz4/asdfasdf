#ifndef ASDFASDF_SCANNER_HPP
#define ASDFASDF_SCANNER_HPP


#include <string_view>
#include <array>
#include <vector>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <sstream>

namespace Grammar
{
    using namespace std::string_view_literals;

    using View = std::string_view;
    using Type = unsigned;

    constexpr static unsigned NUM_TOKENS = 20;
    constexpr static unsigned NUM_ALL_TOKENS = 21;


    enum Token : Type
    {
        ACHT = 0,
        DREI,
        EIN,
        ELF,
        FUENF,
        HUNDERT,
        NEUN,
        SECH,
        SECHS,
        SIEB,
        SIEBEN,
        SSIG,
        TAUSEND,
        UND,
        VIER,
        ZEHN,
        ZIG,
        ZWAN,
        ZWEI,
        ZWOELF,
        $end = ~((Type) 0) - 2,
        $error,
    };


    constexpr static unsigned NUM_NON_TERM = 19;
    constexpr static Type PROD_MASK = 1 << (8 * sizeof(Type) - 1);

    enum Production : Type
    {
        START = PROD_MASK,
        S,
        Z1,
        Z2,
        Z3,
        Z4,
        Z5,
        Z6,
        Z7,
        Z8,
        Z9,
        Z10,
        Z11,
        Z12,
        Z13,
        Z14,
        Z613,
        Z1278911,
        U,
    };

    constexpr static std::array TOKEN_VAL = {
            8, 3, 1, 11, 5, 100, 9, 6, 6, 7, 7,
            10, 1000, 0, 4, 10, 10, 2, 2, 12
    };

    using Value = decltype(TOKEN_VAL)::value_type;

    constexpr static std::array TOKEN_STR = {
            "acht"sv, "drei"sv, "ein"sv, "elf"sv,
            "fuenf"sv, "hundert"sv, "neun"sv, "sech"sv,
            "sechs"sv, "sieb"sv, "sieben"sv, "ssig"sv,
            "tausend"sv, "und"sv, "vier"sv, "zehn"sv,
            "zig"sv, "zwan"sv, "zwei"sv, "zwoelf"sv,
    };

    constexpr static std::array PRODUCTION_STR = {
            "s'"sv, "s"sv,"z1"sv, "z2"sv,
            "z3"sv, "z4"sv, "z5"sv, "z6"sv,
            "z7"sv, "z8"sv,"z9"sv, "z10"sv,
            "z11"sv, "z12"sv, "z13"sv, "z14"sv,
            "z613"sv, "z1278911"sv, "u"sv,
    };



    constexpr static View strOf (Type token) noexcept
    {
        if (token == $end)
            return "$"sv;
        return TOKEN_STR[token];
    }

    constexpr static View NON_TERM_STR (Type token) noexcept
    {
        return PRODUCTION_STR[token];
    }

    constexpr static Value valueOf (Type token) noexcept
    {
        return TOKEN_VAL[token];
    }

    constexpr static Production indexOfProduction (unsigned index)
    {
        return (Production)(index | PROD_MASK);
    }

    static Type toEnum (View str)
    {
        auto tIt = find(TOKEN_STR.begin(), TOKEN_STR.end(), str);
        if (tIt != TOKEN_STR.end())
            return distance(TOKEN_STR.begin(), tIt);

        auto pIt = find(PRODUCTION_STR.begin(), PRODUCTION_STR.end(), str);
        return PROD_MASK | distance(PRODUCTION_STR.begin(), pIt);
    }

}

constexpr auto beginsWith (Grammar::View s, Grammar::View e)
{
    return s.substr(0, e.size()) == e;
};

namespace Lexer
{
    using namespace Grammar;
    using namespace std;

    struct Item
    {
        Token token;
        View strVal;
        Value value;
    };


    constexpr static auto matcher (View s)
    {
        return [s = View(s)] (View a, View b)
        {
            auto diff_pos = mismatch(begin(s), end(s), begin(b)).first;
            auto new_size = (size_t) distance(begin(s), diff_pos);
            return s.substr(0, max(new_size, a.size()));
        };
    }

    static auto lexToken (View s)
    {
        using namespace std;

        if (s.empty()) return Item{ $end, strOf($end), 0 };

        auto greater_than_s = [s = View(s)] (const auto& a) { return a > s; };
        auto gt_eq = greater_equal<>();
        auto noMatch = s.substr(0, 0);

        auto rtBegin = rbegin(TOKEN_STR), rtEnd = rend(TOKEN_STR) - 1;
        auto tBegin = begin(TOKEN_STR) + 1, tEnd = end(TOKEN_STR);

        auto rev = lower_bound(rtBegin, rtEnd, s, gt_eq);
        auto matchFst = decltype(TOKEN_STR)::const_iterator((rev + 1).base());

        auto matchLst = find_if(matchFst, tEnd, greater_than_s);

        if (rev == rtEnd || (*rev)[0] != s[0])
            matchFst++, matchLst++;

        auto matchFunc = matcher(s);
        auto match = accumulate(matchFst, matchLst, noMatch, matchFunc);

        auto fs = lower_bound(tBegin, tEnd, match);
        if (fs != tEnd && *fs == match)
        {
            auto token = static_cast<Token>(distance(begin(TOKEN_STR), fs));
            auto strVal = s.substr(0, match.size());
            auto item = Item{ token, strVal, valueOf(token) };
            return item;
        }
        return Item{ $error, s.substr(0, 1) };
    }

    static std::vector<Item> lex (View stream)
    {
        std::vector<Item> lexemes;
        unsigned position = 0;
        while (true)
        {
            auto item = lexToken(stream.substr(position));
            if (item.token == Grammar::$end)
            {
                lexemes.push_back(item);
                return lexemes;
            }
            if (item.token == Grammar::$error)
            {
                position++;
                if (lexemes.empty() || lexemes.back().token != Grammar::$error)
                {
                    lexemes.push_back(item);
                }
                else
                {
                    auto& back = lexemes.back().strVal;
                    back = Grammar::View{ back.data(), back.size() + 1 };
                }
            }
            else
            {
                lexemes.push_back(item);
                position += item.strVal.size();
            }
        }
    }
};



#endif //ASDFASDF_SCANNER_HPP

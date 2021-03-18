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

    constexpr static unsigned NUM_TOKENS = 20;
    enum Token : unsigned
    {
        ACHT = 1,
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
        ERR_BIT = (1u << 31u),
        NONE = 0u,
    };

    constexpr static std::array TOKEN_VAL = {
        0xfffffff, 8, 3, 1, 11, 5, 100, 9, 6, 6, 7, 7,
        10, 1000, 0, 4, 10, 10, 2, 2, 12
    };

    constexpr static std::array TOKEN_STR = {
        ""sv, "acht"sv, "drei"sv, "ein"sv, "elf"sv,
        "fuenf"sv, "hundert"sv, "neun"sv, "sech"sv,
        "sechs"sv, "sieb"sv, "sieben"sv, "ssig"sv,
        "tausend"sv, "und"sv, "vier"sv, "zehn"sv,
        "zig"sv, "zwan"sv, "zwei"sv, "zwoelf"sv,
    };

    constexpr static auto strOf (unsigned token) noexcept
    {
        return TOKEN_STR[token];
    }

    constexpr static auto valueOf (unsigned token) noexcept
    {
        return TOKEN_VAL[token];
    }

    using Type = std::underlying_type_t<Token>;
    using Value = decltype(TOKEN_VAL)::value_type;
    using View = std::string_view;
}

constexpr auto beginsWith (Grammar::View s, Grammar::View e)
{
    return s.substr(0, e.size()) == e;
};

struct Lexer
{
private:
    template<class Compare>
    constexpr auto matcher (std::string_view s, Compare func)
    {
        using namespace Grammar;
        return [s = View(s), f = std::move(func)] (View a, View b)
        {
            auto diff_pos = std::mismatch(begin(s), end(s), begin(b)).first;
            auto new_size = (std::size_t) std::distance(begin(s), diff_pos);
            return s.substr(0, f(new_size, a.size()));
        };
    }

public:
    struct Item
    {
        Grammar::Type error = 0;
        Grammar::Token token = Grammar::NONE;

        constexpr bool hasError ()
        { return error & (unsigned) Grammar::ERR_BIT; }

        constexpr Grammar::Value value () const
        {
            return Grammar::valueOf(token);
        }

        constexpr Grammar::View str () const
        {
            return Grammar::strOf(token);
        }

        template<class ... Args>
        constexpr static auto Error (Args ... args)
        {
            using namespace Grammar;
            Type err = (ERR_BIT | ... | (1u << ((Type) args - 1)));

            return Item{
                .error = err,
                .token = NONE
            };
        }

        constexpr static auto Valid (Grammar::Token token)
        {
            using namespace Grammar;
            return Item{
                .error = (Type) NONE,
                .token = token
            };
        }
    };

    Grammar::View stream;
    int position = 0;

public:
    explicit Lexer (std::string_view s)
        : stream(s), position(0)
    {}

    /*constexpr*/
    auto lexToken (std::string_view s)
    {
        using namespace Grammar;
        using namespace std;

        if (s.empty()) return Item::Valid(NONE);

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

        auto bestFunc = matcher(s, [] (auto a, auto b) { return max(a, b); });
        auto match = accumulate(matchFst, matchLst, noMatch, bestFunc);

        auto fs = lower_bound(tBegin, tEnd, match);
        if (fs != tEnd && *fs == match)
        {
            auto token = static_cast<Token>(distance(begin(TOKEN_STR), fs));
            auto item = Item::Valid(static_cast<Token>(token));
            position += item.str().size();
            return item;
        }

        auto commonFunc = matcher(s, [] (auto a, auto b) { return min(a, b); });
        auto common = accumulate(matchFst, matchLst, s, commonFunc);

        auto item = Item::Error(NONE);
        if (common.empty()) return item;

        auto fix = lower_bound(tBegin, tEnd, common);
        for (; fix != end(TOKEN_STR) && beginsWith(*fix, common); ++fix)
        {
            auto bitToSet = (Type) distance(tBegin, fix);
            item.error |= ((Type) 1u) << bitToSet;
        }
        position += common.size();
        return item;
    }

    struct LexerError : std::exception
    {
        std::string msg;
        LexerError (std::string_view s) : msg(s) {}
        const char* what () const throw()
        {
            return msg.c_str();
        }
    };

    [[noreturn]]
    void exitWithError (Item fault, Grammar::View str)
    {
        constexpr int prefixSize = 10, trailSize = 12;
        std::stringstream errStream;

        errStream << "On position " << position << " of input: \n";
        errStream << "Unexpected character '" << stream[position]
                  << "' during lex attempt of: " << str << "\n\n";

        auto hasThreeDots = position >= prefixSize;
        auto prefixBegin = std::max(0, position - prefixSize);
        auto prefix = stream.substr(prefixBegin, position - prefixBegin);
        auto trail = stream.substr(position, trailSize);
        errStream << (position >= prefixSize ? "..." : "")
                  << prefix << trail
                  << (trail.size() < trailSize ? "" : "...")
                  << "\n";

        for (int i = 0; i < prefix.size() + 3 * hasThreeDots; ++i)
            errStream << '-';
        errStream << "^\n\n";

        bool found = false;
        for (unsigned i = 0; i < Grammar::NUM_TOKENS; ++i)
        {
            if (fault.error & (1u << i))
            {
                found = true;
                errStream << "Possible fix: " << Grammar::strOf(i + 1) << "\n";
            }
        }
        if (!found) errStream << "No obvious fixes available\n";
        throw LexerError(errStream.str());
    }

    std::vector<Item> lex ()
    {
        Item item;
        std::vector<Item> lexemes;
        while (true)
        {
            auto pPos = position;
            item = lexToken(stream.substr(position));
            auto diff = position - pPos;
            if (!item.token)
            {
                if (!item.hasError()) return lexemes;
                exitWithError(item, stream.substr(pPos, diff));
            }
            lexemes.emplace_back(item);
        }
    }
};


#endif //ASDFASDF_SCANNER_HPP

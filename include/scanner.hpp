#ifndef ASDFASDF_SCANNER_HPP
#define ASDFASDF_SCANNER_HPP

#include <string_view>
#include <array>
#include <vector>
#include <cassert>
#include <iostream>

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

struct Lexer
{
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
        static auto Error (Args ... args)
        {
            using namespace Grammar;
            Type err = (ERR_BIT | ... | (1u << ((Type) args - 1)));

            return Item{
                .error = err,
                .token = NONE
            };
        }

        static auto Valid (Grammar::Token token)
        {
            using namespace Grammar;
            return Item{
                .error = (Type) NONE,
                .token = token
            };
        }
    };

    struct State
    {
        Grammar::View stream;
        Grammar::View parsed;
        int& orig_pos;

        constexpr void advance (int dist)
        {
            parsed = stream.substr(0, parsed.size() + dist);
            orig_pos += dist;
        }

        constexpr void revert (int dist)
        {
            parsed = stream.substr(0, parsed.size() - dist);
            orig_pos -= dist;
        }
    };

    Grammar::View stream;
    int position = 0;

public:
    explicit Lexer (std::string_view s)
        : stream(s), position(0)
    {}

    static constexpr auto consume (State& state)
    {
        auto pos = state.parsed.size();
        auto c = state.stream.at(pos);
        state.advance(1);
        return c;
    };

    static constexpr auto expect (State& state, std::string_view exp)
    {
        auto pos = state.parsed.size();
        auto prefix = state.stream.substr(pos, exp.size());
        if (prefix != exp)
        {
            auto unmatched = std::mismatch(
                exp.begin(), exp.end(), prefix.begin());
            state.advance(std::distance(exp.begin(), unmatched.first));
            return false;
        }
        state.advance(exp.size());
        return true;
    };

    constexpr auto lexToken (std::string_view str)
    {
        using namespace Grammar;
        constexpr auto Valid = Item::Valid;
        const auto Err = [&] (auto... args) { return Item::Error(args...); };

        if (str.empty()) return Valid(NONE);

        int init = position;
        State s{ str, "", position };

        auto itemIf = [&] (Token token, View p)
        {
            if (expect(s, p)) return Valid(token);
            else return Err(token);
        };

        auto c = consume(s);
        if (c == 'a') return itemIf(ACHT, "cht");
        else if (c == 'd') return itemIf(DREI, "rei");
        else if (c == 'e')
        {
            c = consume(s);
            if (c == 'i') return itemIf(EIN, "n");
            if (c == 'l') return itemIf(ELF, "f");
            s.revert(1);
            return Err(EIN, ELF);
        }
        else if (c == 'f') return itemIf(FUENF, "uenf");
        else if (c == 'h') return itemIf(HUNDERT, "undert");
        else if (c == 'n') return itemIf(NEUN, "eun");
        else if (c == 's')
        {
            c = consume(s);
            if (c == 'e')
            {
                if (!expect(s, "ch")) return Err(SECH, SECHS);
                auto follow_s = consume(s);
                if (follow_s == 's') return Valid(SECHS);
                s.revert(1);
                return Valid(SECH);
            }
            else if (c == 'i')
            {
                if (!expect(s, "eb")) return Err(SIEB, SIEBEN);
                auto follow_e = consume(s);
                auto follow_n = consume(s);
                if (follow_e == 'e' && follow_n == 'n') return Valid(SIEBEN);
                s.revert(2);
                return Valid(SIEB);
            }
            else if (c == 's') return itemIf(SSIG, "ig");
            else return Err(SECH, SECHS, SIEB, SIEBEN, SSIG);
        }
        if (c == 't') return itemIf(TAUSEND, "ausend");
        if (c == 'u') return itemIf(UND, "nd");
        if (c == 'v') return itemIf(VIER, "ier");
        if (c == 'z')
        {
            c = consume(s);
            if (c == 'e') return itemIf(ZEHN, "hn");
            if (c == 'i') return itemIf(ZIG, "g");
            if (c == 'w')
            {
                c = consume(s);
                if (c == 'a') return itemIf(ZWAN, "n");
                if (c == 'e') return itemIf(ZWEI, "i");
                if (c == 'o') return itemIf(ZWOELF, "elf");
                s.revert(1);
                return Err(ZWAN, ZWEI, ZWOELF);
            }
            s.revert(1);
            return Err(ZEHN, ZIG, ZWAN, ZWEI, ZWOELF);
        }
        s.revert(1);
        return Err(NONE);
    }


    [[noreturn]]
    void exitWithError (Item fault, Grammar::View str)
    {
        constexpr int pSize = 10, tSize = 12;
        std::cerr << "On position " << position << " of input: \n";
        std::cerr << "Unexpected character '" << stream[position]
                  << "' during lex attempt of: " << str << "\n\n";

        auto prefInit = std::max(position - pSize, 0);
        auto prefSz = std::min(position, pSize) + 3 * (position >= pSize);
        auto prefix = stream.substr(prefInit, prefSz);
        auto trail = stream.substr(position, tSize);
        auto trailSz = trail.size();
        std::cerr << (position >= pSize ? "..." : "")
                  << prefix << trail
                  << (trailSz >= tSize ? "..." : "")
                  << "\n";

        while (prefSz--)
            std::cerr << '-';
        std::cerr << "^\n\n";

        bool found = false;
        for (unsigned i = 0; i < Grammar::NUM_TOKENS; ++i)
        {
            if (fault.error & (1u << i))
            {
                found = true;
                std::cerr << "Possible fix: " << Grammar::strOf(i + 1) << "\n";
            }
        }
        if (!found) std::cerr << "No obvious fixes available\n";
        std::exit(-1);
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

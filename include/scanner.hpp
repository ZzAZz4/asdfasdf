#ifndef ASDFASDF_SCANNER_HPP
#define ASDFASDF_SCANNER_HPP

#include <string_view>
#include <array>
using namespace std::string_view_literals;

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
    TOKEN_AUX,
    NONE = 0,
    ERR_BIT = (1u << 31u)
};

constexpr static std::array<int, TOKEN_AUX - 1> TOKEN_VAL = {
    8, 3, 1, 11, 5, 100, 9, 6, 6, 7, 7,
    10, 1000, -1, 4, 10, 10, 2, 2, 12
};

constexpr static std::array<std::string_view, TOKEN_AUX> TOKEN_STR = {
    "acht"sv, "drei"sv, "ein"sv, "elf"sv,
    "fuenf"sv, "hundert"sv, "neun"sv, "sech"sv,
    "sechs"sv, "sieb"sv, "sieben"sv, "ssig"sv,
    "tausend"sv, "und"sv, "vier"sv, "zehn"sv,
    "zig"sv, "zwan"sv, "zwei"sv, "zwoelf"sv,
};

constexpr auto getVal (unsigned token)
{
    return TOKEN_VAL[(unsigned) token - 1];
}

constexpr auto getName (unsigned token)
{
    return TOKEN_STR[(unsigned) token - 1];
}

constexpr auto consume (std::string_view& s)
{
    auto c = s.front();
    s.remove_prefix(1);
    return c;
};
constexpr unsigned expect (std::string_view& s, char c)
{
    if (c != s.front()) return ERR_BIT;
    s.remove_prefix(1);
    return 0u;
};
constexpr unsigned str_expect (std::string_view& s, std::string_view c)
{
    if (s.substr(0, c.size()) != c) return ERR_BIT;
    s.remove_prefix(c.size());
    return 0u;
};

static constexpr unsigned match (std::string_view& s)
{
    if (s.empty()) return NONE;

    auto c = consume(s);
    if (c == 'a') return ACHT | str_expect(s, "cht");
    else if (c == 'd') return DREI | str_expect(s, "rei");
    else if (c == 'e')
    {
        auto nc = consume(s);
        if (nc == 'i') return EIN | expect(s, 'n');
        if (nc == 'l') return ELF | expect(s, 'f');
        else return NONE | ERR_BIT;
    }
    else if (c == 'f') return FUENF | str_expect(s, "uenf");
    else if (c == 'h') return HUNDERT | str_expect(s, "undert");
    else if (c == 'n') return NEUN | str_expect(s, "eun");
    else if (c == 's')
    {
        auto nc = consume(s);
        if (nc == 'e')
        {
            if (str_expect(s, "ch")) return SECH | ERR_BIT;
            if (s.front() != 's') return SECH;
            s.remove_prefix(1);
            return SECHS;
        }
        else if (nc == 'i')
        {
            if (str_expect(s, "eb")) return SIEB | ERR_BIT;
            if (s.substr(0, 2) != "en") return SIEB;
            s.remove_prefix(2);
            return SIEBEN;
        }
        else if (nc == 's') return SSIG | str_expect(s, "ig");
        else return NONE | ERR_BIT;
    }
    if (c == 't') return TAUSEND | str_expect(s, "ausend");
    if (c == 'u') return UND | str_expect(s, "nd");
    if (c == 'v') return VIER | str_expect(s, "ier");
    if (c == 'z')
    {
        c = consume(s);
        if (c == 'e') return ZEHN | str_expect(s, "hn");
        if (c == 'i') return ZIG | expect(s, 'g');
        if (c != 'w') return NONE | ERR_BIT;
        c = consume(s);
        if (c == 'a') return ZWAN | expect(s, 'n');
        if (c == 'e') return ZWEI | expect(s, 'i');
        if (c == 'o') return ZWOELF | str_expect(s, "elf");
        else return NONE | ERR_BIT;
    }
    return NONE | ERR_BIT;
}


#endif //ASDFASDF_SCANNER_HPP

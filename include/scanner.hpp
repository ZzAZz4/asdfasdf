#ifndef ASDFASDF_SCANNER_HPP
#define ASDFASDF_SCANNER_HPP

#include <string_view>


enum Token : unsigned char
{
    ACHT,
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
    NONE = 0xFF,
    ERROR = 0x80
};

constexpr static int TOKEN_VAL[] =
    { 8, 3, 1, 11, 5, 100, 9, 6, 6, 7, 7,
      10, 1000, -1, 4, 10, 10, 2, 2, 12 };

static constexpr auto match (std::string_view& s)
{
    constexpr auto consume = [] (std::string_view& s)
    {
        auto c = s.front();
        s.remove_prefix(1);
        return c;
    };
    constexpr auto expect = [] (std::string_view& s, char c)
    {
        if (c != s.front())
            return false;
        s.remove_prefix(1);
        return true;
    };
    constexpr auto str_expect = [] (std::string_view& s, std::string_view c)
    {
        if (s.substr(0, c.size()) != c)
            return false;
        s.remove_prefix(c.size());
        return true;
    };
    if (s.empty()) return NONE;

    auto c = consume(s);
    if (c == 'a') return str_expect(s, "cht") ? ACHT : ERROR;
    else if (c == 'd') return str_expect(s, "rei") ? DREI : ERROR;
    else if (c == 'e')
    {
        auto nc = consume(s);
        if (nc == 'i') return expect(s, 'n') ? EIN : ERROR;
        else return nc == 'l' && expect(s, 'f') ? ELF : ERROR;
    }
    else if (c == 'f') return str_expect(s, "uenf") ? FUENF : ERROR;
    else if (c == 'h') return str_expect(s, "undert") ? HUNDERT : ERROR;
    else if (c == 'n') return str_expect(s, "eun") ? NEUN : ERROR;
    else if (c == 's')
    {
        auto nc = consume(s);
        if (nc == 'e')
        {
            if (!str_expect(s, "ch")) return ERROR;
            if (s.front() != 's') return SECH;
            s.remove_prefix(1);
            return SECHS;
        }
        else if (nc == 'i')
        {
            if (!str_expect(s, "eb")) return ERROR;
            if (s.substr(0, 2) != "en") return SIEB;
            s.remove_prefix(2);
            return SIEBEN;
        }
        else return nc == 's' && str_expect(s, "ig") ? SSIG : ERROR;
    }
    if (c == 't') return str_expect(s, "ausend") ? TAUSEND : ERROR;
    if (c == 'u') return str_expect(s, "nd") ? UND : ERROR;
    if (c == 'v') return str_expect(s, "ier") ? VIER : ERROR;
    if (c == 'z')
    {
        c = consume(s);
        if (c == 'e') return str_expect(s, "hn") ? ZEHN : ERROR;
        if (c == 'i') return expect(s, 'g') ? ZIG : ERROR;
        if (c != 'w') return ERROR;
        c = consume(s);
        if (c == 'a') return expect(s, 'n') ? ZWAN : ERROR;
        if (c == 'e') return expect(s, 'i') ? ZWEI : ERROR;
        return c == 'o' && str_expect(s, "elf") ? ZWOELF : ERROR;
    }
    return ERROR;
}


template<typename StVal_>
struct Scanner
{
    constexpr static int NUM_OF_TOKENS = (int) Token::TOKEN_AUX;
    StVal_ value;

    Token readToken ()
    {

    }
};


#endif //ASDFASDF_SCANNER_HPP

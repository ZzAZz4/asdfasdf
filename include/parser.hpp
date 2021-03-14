//
// Created by esteb on 13-Mar-21.
//

#ifndef ASDFASDF_PARSER_HPP
#define ASDFASDF_PARSER_HPP

#include <vector>

using Rules = std::vector<int>;
using Rules = std::vector<int>;




class SLRparser{
private:


public:

    explicit SLRparser (const Rules& rules, ID start);

    bool
    parse (std::string_view s);
};


#endif //ASDFASDF_PARSER_HPP

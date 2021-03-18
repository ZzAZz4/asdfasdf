//
// Created by esteb on 13-Mar-21.
//

#ifndef ASDFASDF_PARSER_HPP
#define ASDFASDF_PARSER_HPP

#include "scanner.hpp"
#include <unordered_map>
#include <unordered_set>


template<class Iterable>
struct Range
{
    typedef typename Iterable::const_iterator Iterator;
    Iterator begin_;
    Iterator end_;

    constexpr Range (const Iterable& e)
        : begin_(e.cbegin()), end_(e.cend())
    {}

    constexpr Range (Iterator begin, Iterator end)
        : begin_(begin), end_(end)
    {}

    constexpr auto begin () const
    { return begin_; }

    constexpr auto end () const
    { return end_; }
};

namespace Grammar
{
    constexpr unsigned NON_TERM_BIT = 1u << 30u;
    constexpr unsigned SPEC_BIT = 1u << 29u;

    enum Production : Type
    {
        START = NON_TERM_BIT,
        E,
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
        REPL_Z5_9,
        REPL_Z6_13,
        REPL_Z1_2_7_8,
        REPL_Z1_2_7_8_9_11,
        REPL_Z1_2_7_8_9_11_DUP,
        EPSILON = NON_TERM_BIT | SPEC_BIT + 0u,
        $
    };

    constexpr auto LAST_PROD = REPL_Z1_2_7_8_9_11_DUP;
    constexpr auto NUM_PRODUCTIONS = LAST_PROD & ~(NON_TERM_BIT) + 1u;

    constexpr auto isTerminal (Type x)
    {
        return !(x & NON_TERM_BIT);
    }

    using ID = Grammar::Type;
    using Expansion = std::vector<ID>;
    using ExpView = Range<Expansion>;
    using Rules = std::unordered_map<ID, std::vector<Expansion>>;

    static Rules rules = {
        { START, {
                     { E }
                 }},
        { E,     {
                     { Z1 },
                     { Z2 },
                     { Z7 },
                     { Z8 },
                     { Z9 },
                     { Z10 },
                     { Z11 },
                     { Z12 },
                     { Z13 },
                     { Z14 }
                 }},
        { Z1, {

        }}

    };

    static Type indexOf (Type production)
    {
        return production & ~(NON_TERM_BIT);
    };
}

namespace Table
{
    using namespace Grammar;

    using IDSet = std::unordered_set<ID>;
    using TableEntry = struct
    {
        IDSet first;
        IDSet follow;
    };
    using Table = struct
    {
        std::vector<TableEntry> entries;

        auto begin () const
        { return entries.begin(); }

        auto end () const
        { return entries.end(); }

        auto& operator [] (Type val)
        { return entries[indexOf(val)]; }

        auto& operator [] (Type val) const
        { return entries[indexOf(val)]; }

    };


    IDSet rhsFirst (ExpView expansion, const Table& table)
    {
        IDSet retVal = {};
        for (auto& Y : expansion)
        {
            auto fy = isTerminal(Y) ? IDSet({ Y }) : table[Y].first;
            auto epsFound = fy.erase(EPSILON);
            retVal.merge(fy);
            if (!epsFound)
                return retVal;
        }
        retVal.emplace(EPSILON);
        return retVal;
    }

    void fillFirst (Table& table, Rules& rules)
    {
        while (true)
        {
            int changes = 0;
            for (Type x = START; x <= LAST_PROD; ++x)
            {
                for (const auto& expansion : rules[x])
                {
                    auto elementsToAdd = rhsFirst(expansion, table);
                    for (const auto& element : elementsToAdd)
                    {
                        auto wasAdded = table[x].first.emplace(element).second;
                        changes += wasAdded;
                    }
                }
            }
            if (!changes) return;
        }
    }

    Table generate (Rules& rules)
    {
        Table table;
        table.entries.resize(NUM_PRODUCTIONS);
        fillFirst(table, rules);
        return table;
    }
}


#endif //ASDFASDF_PARSER_HPP

#include "../include/firstSet.hpp"



IDSet
rhsFirst (IDIter begin, IDIter end, SetMap& first, const IDSet& nonTerms)
{
    IDSet retVal;
    for (auto iter = begin; iter != end; ++iter)
    {
        auto& Y = *iter;
        auto fy = contains(nonTerms, Y) ? first.at(Y) : IDSet{ Y };
        auto epsFound = fy.erase(epsilon);
        retVal.merge(fy);
        if (!epsFound)
            return retVal;
    }
    retVal.emplace(epsilon);
    return retVal;
}

SetMap
makeFirst (const Rules& rules, const IDSet& nonTerms)
{
    SetMap firstSets;

    for (auto[lhs, _] : rules)
        firstSets.emplace(lhs, IDSet{});

    while (true)
    {
        int changes = 0;
        for (auto&[lhs, rhs] : rules)
        {
            auto& xSet = firstSets[lhs];
            auto toAdd = rhsFirst(rhs.begin(), rhs.end(), firstSets, nonTerms);
            for (const auto& id : toAdd)
            {
                auto addedElement = xSet.emplace(id).second;
                changes += addedElement;
            }
        }
        if (!changes)
            return firstSets;
    }
}


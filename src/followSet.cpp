


#include "../include/followSet.hpp"

#include <algorithm>



IDIter findProduction (IDIter begin, IDIter end, IDSet& nonTerms){
    auto isProduction = [&] (auto& item) { return contains(nonTerms, item); };
    return std::find_if(begin, end, isProduction);
}

IDSet rhsFollow (ID lhs, IDIter begin, IDIter end, FSets sets){

    auto qBegin = begin + 1;
    auto followB = rhsFirst(qBegin, end, sets.first, sets.nonTerms);
    bool hasEpsilon = followB.erase(epsilon);
    if (hasEpsilon)
    {
        auto lhsFollow = sets.follow.at(lhs);
        followB.merge(lhsFollow);
    }
    return followB;

}

SetMap makeFollow (const Rules& rules, ID start, SetMap& first, IDSet& nonTerms){
    SetMap followSets;
    FSets sets = { first, followSets, nonTerms };

    for (auto[lhs, _] : rules)
        followSets.emplace(lhs, IDSet{});

    followSets[start].emplace(endMark);

    while (true)
    {
        int changes = 0;
        for (auto&[lhs, rule] : rules)
        {
            auto it = findProduction(rule.begin(), rule.end(), nonTerms);
            for (; it != rule.end();)
            {
                auto toAdd = rhsFollow(lhs, it, rule.end(), sets);
                for (auto element : toAdd)
                    changes += followSets.at(*it).emplace(element).second;

                it = findProduction(it + 1, rule.end(), nonTerms);
            }
        }
        if (!changes)
            return followSets;
    }
}


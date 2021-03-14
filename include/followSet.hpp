//
// Created by Jean Paul on 13/03/2021.
//

#ifndef ASDFASDF_FOLLOWSET_HPP
#define ASDFASDF_FOLLOWSET_HPP


#include "common.hpp"
#include "firstSet.hpp"


IDIter
findProduction (IDIter begin, IDIter end, IDSet& nonTerms);

IDSet
rhsFollow (ID lhs, IDIter begin, IDIter end, FSets sets);

SetMap
makeFollow (const Rules& rules, ID start, SetMap& first, IDSet& nonTerms);


#endif //ASDFASDF_FOLLOWSET_HPP

//
// Created by Jean Paul on 13/03/2021.
//

#ifndef ASDFASDF_FIRSTSET_HPP
#define ASDFASDF_FIRSTSET_HPP

#include "common.hpp"

IDSet
rhsFirst (IDIter begin, IDIter end, SetMap& first, const IDSet& nonTerms);

SetMap
makeFirst (const Rules& rules, const IDSet& nonTerms);


#endif //ASDFASDF_FIRSTSET_HPP

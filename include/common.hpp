//
// Created by Jean Paul on 13/03/2021.
//

#ifndef ASDFASDF_COMMON_HPP
#define ASDFASDF_COMMON_HPP


#include <string_view>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <stack>

#include "scanner.hpp"


using ID = std::string_view ;
using Rule = std::vector<ID>;
using Rules = std::unordered_multimap<ID, Rule>;
using IDSet = std::unordered_set<ID>;
using SetMap = std::unordered_map<ID, IDSet>;
using IDIter = Rule::const_iterator;
using PTable = std::unordered_map<ID, std::unordered_multimap<ID, const Rule&>>;

struct FSets
{
    SetMap& first ;
    SetMap& follow ;
    IDSet& nonTerms ;
};

constexpr ID epsilon = "";
constexpr ID endMark = "$";

template<template<class, class...> class Container, class T, class ... U>
bool contains (const Container<T, U...>& c, const T& elem);

#include "../src/common.cpp"



#endif //ASDFASDF_COMMON_HPP

//
// Created by esteb on 13-Mar-21.
//

#ifndef ASDFASDF_PARSER_HPP
#define ASDFASDF_PARSER_HPP


#include "common.hpp"

#include <iostream>


template <typename T = Rules>
struct node{
    // pos of dot and rule
    std::vector<std::pair<unsigned ,T>> rules;

    node(T  val){
        rules.push_back({0,val});
    }

    node(T  val, unsigned  pos){
        rules.push_back({pos,val});
    }

};


// type and key
template <typename T = ID, typename  K = int>
class parserDfa{

    const std::string separator = ".";
    const K finalState = 1;
    const K initialState = 0;


    std::unordered_map< K,  node <T>> states;
    std::unordered_map<K , std::unordered_map<T, node<Rules>* >> states_{};


    parserDfa(const Rules& rules, const ID start,const SetMap& first, const SetMap &follow){
        node curr{rules[start]};
    }
};

class SLRparser{
private:

    std::stack<ID> stack;
    SetMap first;
    SetMap follow;
    IDSet nonTerms;


public:

    explicit SLRparser (const Rules& rules, ID start);

    bool parse (std::vector<ID> s);

};


#endif //ASDFASDF_PARSER_HPP

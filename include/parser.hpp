//
// Created by esteb on 13-Mar-21.
//

#ifndef ASDFASDF_PARSER_HPP
#define ASDFASDF_PARSER_HPP


#include "common.hpp"

#include <iostream>
#include <tuple>
#include <queue>

// type and key
template <typename T = Rule ,  typename  K = int>
struct Node{
    // ID -> pos of dot and rule
    std::unordered_multimap<ID, std::pair<K, T>> rules;

    Node() = default;

    void insert(T  val, unsigned  pos = 0){
        rules.insert({pos,val});
    }


    bool contains(const ID& lhs,const T &rule, unsigned pos = 0){
        auto its = rules.equal_range(lhs);
        for (auto it = its.first; it != its.second; ++it) {
            auto [pos1,  lhs] = it->second;
            if(pos != pos1) continue;

            auto elem1 = rule.begin();
            auto  elem2 = lhs.begin();

            while(elem1 != rule.end() || elem2 != lhs.end() ){
                if(elem1++ != elem2++) continue;
            }
            if(elem1 == rule.end() && elem2 == lhs.end()) return true;
        }
        return false;
    }

    bool operator == (Node<T,K> &other){
        return false;
    }



};


// type and key
template <typename T = ID, typename  K = int>
class parserDfa{

    const K finalState = 1;
    const K initialState = 0;
    IDSet  nonTerms;


    std::unordered_map< K,  Node <Rule>*> states;
    std::unordered_map<K , std::unordered_map<T, Node<Rule>* >> states_{};

public:

    auto getRules(const Rules& rules, const ID& nonTerminal){
        return rules.equal_range(nonTerminal);
    }

    void completaState(K state){
        std::queue<std::pair<K,Rule>> toUse;
        auto node = states_[state];

        for(auto it: node->rules){
            auto [pos,rule] = it.second;
            if(nonTerms.find(rule[pos]) != nonTerms.end()) toUse.push({0,rule});

        }

        while (!toUse.empty()){
            auto curr = toUse.front();
            if(!node->contains(curr.second) && nonTerms.find(curr.second[0]) != nonTerms.end() )
                node->insert(curr.first, curr.second);
            toUse.pop();
        }


    }

    parserDfa(const Rules& rules, const ID start,const SetMap& first, const SetMap &follow, const IDSet nonTerms): nonTerms(nonTerms){
        std::unordered_set<ID> visNonTerminal;
        states_.emplace(0,new Node<Rule>);


        auto range = rules.equal_range(start);
        for (auto it = range.first; it != range.second; ++it) {
            states_[initialState];
        }


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

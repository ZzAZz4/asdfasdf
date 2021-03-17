//
// Created by esteb on 13-Mar-21.
//

#ifndef ASDFASDF_PARSER_HPP
#define ASDFASDF_PARSER_HPP


#include "common.hpp"
#include "../include/followSet.hpp"
#include "../include/firstSet.hpp"


#include <iostream>
#include <tuple>
#include <queue>
#include <utility>

// type and key
struct Node{
    using T = Rule;
    using K = int;

    // ID -> pos of dot and rule
    std::unordered_multimap<ID, std::pair<K, T>> rules;

    Node() = default;

    void insert(ID lhs,const T&  rule,unsigned  pos = 0){
        rules.insert({lhs, {pos, rule}});
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





};


// type and key
class parserDfa{

    using K = int;
    using T = ID;

    const K finalState = 1;
    const K initialState = 0;
    const Rules & rules;

    IDSet  nonTerms;


    // saving nodes with
    std::unordered_map< K,  Node*> states;
    // saving transition of nodes
    std::unordered_map<K , std::unordered_map<T, K>> transition;

public:


    void completeState(K state){
        std::queue<ID>toUse;
        auto node = states[state];

        //        std::unordered_multimap<ID, std::pair<K, T>> rules;
        for(const auto& it: node->rules){
            auto [pos,rule] = it.second;
            if(nonTerms.find(rule[pos]) != nonTerms.end()) toUse.push({rule[pos]});

        }

        while (!toUse.empty()){
            int pos = 0;
            auto range = rules.equal_range(toUse.front());
            for (auto it = range.first; it != range.second;it++ ) {
                if(!node->contains(it->first,it->second) && nonTerms.find(it->second[pos]) != nonTerms.end()){
                    node->insert(it->first, it->second,pos);
                    if(node->rules.find(it->second[0]) == node->rules.end()){
                        toUse.push(it->second[0]);
                    }
                }
            }
            toUse.pop();
        }

    }

    K makeTransition(){
        
    }

    parserDfa(const Rules& rules, const ID&  start, IDSet  nonTerms): rules(rules),
    nonTerms(std::move(nonTerms)){
        std::unordered_set<ID> visNonTerminal;

        auto node = states[initialState] = new Node;


//        std::cout<<"range:"<<'\n';
        auto range = rules.equal_range(start);
        for (auto it = range.first; it != range.second;it++ ) {
//            std::cout<<it->first<<'\n';
            for(auto rule: it->second){
                node->insert(it->first, it->second);
//                std::cout<<rule<<" ";
            }
//            std::cout<<std::endl;
        }
        completeState(initialState);








    }
};

void print(const SetMap& m){
    for(const auto & it: m){
        std::cout<<it.first<<" :";
        for(auto const & it2: it.second){
            std::cout<<" "<<it2;
        }
        std::cout<<std::endl;
    }
}


class SLRparser{
private:

    std::stack<ID> stack;
    SetMap first;
    SetMap follow;
    IDSet nonTerms;


public:

    explicit SLRparser (const Rules& rules, ID start){
        // cretate first and follow
        for (auto[lhs, _] : rules)
            nonTerms.emplace(lhs);

        first = makeFirst(rules, nonTerms);
        follow = makeFollow(rules, start, first, nonTerms);
        print(first);
        std::cout<<'\n';
        print(follow);


            std::cout<<start<<'\n';

        // create dfa
        parserDfa dfa (rules,start,nonTerms);


        // create the table
    }

    bool parse (std::vector<ID> s){
        return false;
    }

};


#endif //ASDFASDF_PARSER_HPP

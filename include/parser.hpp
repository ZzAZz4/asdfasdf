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
            if(rule.size() != it->second.second.size()) continue;

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
    std::vector<Node*> states;
    // saving transition of nodes
    std::vector<std::unordered_map<T, K>> transition;

public:


    void completeState(Node* node){
        std::queue<ID>toUse;

        //        std::unordered_multimap<ID, std::pair<K, T>> rules;
        for(const auto& it: node->rules){
            auto [pos,rule] = it.second;
            if(nonTerms.find(rule[pos]) != nonTerms.end()) toUse.push({rule[pos]});

        }

        while (!toUse.empty()){
            int pos = 0;
            auto range = rules.equal_range(toUse.front());
            for (auto it = range.first; it != range.second;it++ ) {
                auto temp1 = it->first;
                auto temp2 = it->second;
                if(!node->contains(it->first,it->second)){
                    node->insert(it->first, it->second,pos);
                    if(nonTerms.find(it->second[0]) != nonTerms.end() && node->rules.find(it->second[0]) == node->rules.end()){
                        toUse.push(it->second[0]);
                    }
                }
            }
            toUse.pop();
        }

    }

    void makeTransition(std::queue<K> & toUse, K state){
        std::unordered_map<ID, Node*> toComplete;

        // makin all posible transitions
        for(const auto& r: states[state]->rules){
            const auto& [lhs, rhs] = r;
            auto [pos,rule] = rhs;
            // the if below does'nt work
            if(transition[state].find(rule[pos]) == transition[state].end() &&
            toComplete.find(rule[pos]) == toComplete.end()){
                auto node = new Node;
                node->insert(lhs , rule, pos + 1);
                toComplete.insert({rule[pos],node});
            }
            else{
                states[state]->insert(lhs , rule, pos + 1);
            }
        }

        // completing anf checking for repetitions
        for(auto it: toComplete){
            bool flag = true;
            auto node = it.second;
            completeState(node);
            // oh god, oh fuck. why i must do this.
            // god are you there?
            int counter = 0;
            for(auto ip: states){
                if(it.second->rules == ip->rules){
                    transition[state][it.first] = counter;
                    flag = false;
                    break;
                }
                counter++;
            }
            if(flag){
                states.push_back(node);
                transition[state][it.first] = states.size() - 1;
                toUse.push(states.size() - 1);
            }
        }

    }

    parserDfa(const Rules& rules, const ID&  start, IDSet  nonTerms): rules(rules),
    nonTerms(std::move(nonTerms)), states(1, nullptr){

        std::queue<K> toUse;
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
        completeState(node);
        makeTransition(toUse,initialState);
//
//        while (!toUse.empty()){
//            makeTransition(toUse,toUse.front());
//            toUse.pop();
//        }

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

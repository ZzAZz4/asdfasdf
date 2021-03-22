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
#include <iomanip>

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
    const ID  start;


    // saving nodes with
    std::vector<Node*> states;
    // saving transition of nodes
    std::vector<std::unordered_map<T, K>> transition;


    void insertState(Node* node){
        states.emplace_back(node);
    }

    void insertTransition(K state, T tran, K val){
        while(transition.size() <= state)
            transition.emplace_back();
        transition[state][tran] = val;
    }

    static bool isCompleteElement(const std::pair<int, Rule>& rhs){
        return rhs.second.size() == rhs.first;
    }

public:


    void completeState(Node* node){
        std::queue<ID>toUse;
        //        std::unordered_multimap<ID, std::pair<K, T>> rules;
        for(const auto& it: node->rules){
            auto [pos,rule] = it.second;
            if(pos < rule.size() && nonTerms.find(rule[pos]) != nonTerms.end()) toUse.push({rule[pos]});

        }

        while (!toUse.empty()){
            int pos = 0;
            auto range = rules.equal_range(toUse.front());
            for (auto it = range.first; it != range.second;it++ ) {
                auto temp1 = it->first;
                auto temp2 = it->second;
                if(!node->contains(it->first,it->second)){
                    node->insert(it->first, it->second,pos);
                    if(nonTerms.find(it->second[0]) != nonTerms.end()){
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
            if(rule[0] == epsilon || pos >= rule.size()) continue;

            if(toComplete.find(rule[pos]) == toComplete.end()){
                auto node = new Node;
                node->insert(lhs , rule, pos + 1);
                toComplete.insert({rule[pos],node});
            }
            else{
                toComplete[rule[pos]]->insert(lhs , rule, pos + 1);
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
                    insertTransition(state,it.first,counter);
                    flag = false;
                    break;
                }
                counter++;
            }
            if(flag){
                insertState(node);
                insertTransition(state,it.first,states.size() - 1);
                toUse.push(states.size() - 1);
            }
        }

    }

    parserDfa(const Rules& rules, const ID  start, IDSet  nonTerms): rules(rules),
    nonTerms(std::move(nonTerms)), start(start){
        std::queue<K> toUse;
        auto node =new Node;
        insertState(node);

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

        while (!toUse.empty()){
            makeTransition(toUse,toUse.front());
            toUse.pop();
        }

    }

    void printDfa(){
        std::cout<<'\n';
//        std::cout<<states.size();
//        return ;
        for(int i = 0; i < states.size(); i++){
            std::cout<<"State: "<<i<<'\n';
            auto node = states[i];
            for(const auto& it: node->rules){
                std::cout<<it.first<<" -> ";
                auto [pos,rule] = it.second;
                for(int j = 0; j < rule.size(); j++){
                    if(pos == j) std::cout<<" . ";
                    std::cout<<rule[j]<<" ";
                }
                if(pos == rule.size()) std::cout<<" . ";
                std::cout<<"\n";
            }
        }
    }

    void printTransitions(){
        std::cout<<'\n' <<"States Transition: "<<'\n';

        for(int i = 0; i < transition.size(); i++){
            std::cout<<"State "<<i<<":\n";
            if(transition[i].empty()) std::cout<<"No Transitions \n";
            for(auto it: transition[i]){
                std::cout<<"     "<<it.first<<" -> "<<it.second<<'\n';
            }
        }
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

#include "../json/json.h"

class SLRparser{
private:



    using state = unsigned;
    using token = int;

    stack<pair<token, state>> sintaxStack;
    queue<token> inputQueue;
    slrTable table;

    using input = std::vector<token>;

public:

    explicit SLRparser (): table(){

    }





};


#endif //ASDFASDF_PARSER_HPP

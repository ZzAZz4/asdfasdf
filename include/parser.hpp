//
// Created by esteb on 13-Mar-21.
//

#ifndef ASDFASDF_PARSER_HPP
#define ASDFASDF_PARSER_HPP

//
//#include "common.hpp"
//#include "../include/followSet.hpp"
//#include "../include/firstSet.hpp"

// type and key
//struct Node{
//    using T = Rule;
//    using K = int;
//
//    // ID -> pos of dot and rule
//    std::unordered_multimap<ID, std::pair<K, T>> rules;
//
//    Node() = default;
//
//    void insert(ID lhs,const T&  rule,unsigned  pos = 0){
//        rules.insert({lhs, {pos, rule}});
//    }
//
//
//    bool contains(const ID& lhs,const T &rule, unsigned pos = 0){
//        auto its = rules.equal_range(lhs);
//        for (auto it = its.first; it != its.second; ++it) {
//            auto [pos1,  lhs] = it->second;
//            if(pos != pos1) continue;
//            if(rule.size() != it->second.second.size()) continue;
//
//            auto elem1 = rule.begin();
//            auto  elem2 = lhs.begin();
//
//            while(elem1 != rule.end() || elem2 != lhs.end() ){
//                if(elem1++ != elem2++) continue;
//            }
//            if(elem1 == rule.end() && elem2 == lhs.end()) return true;
//        }
//        return false;
//    }
//
//};


// type and key
//class parserDfa{
//
//    using K = int;
//    using T = ID;
//
//    const K finalState = 1;
//    const K initialState = 0;
//    const Rules & rules;
//
//    IDSet  nonTerms;
//    const ID  start;
//
//
//    // saving nodes with
//    std::vector<Node*> states;
//    // saving transition of nodes
//    std::vector<std::unordered_map<T, K>> transition;
//
//
//    void insertState(Node* node){
//        states.emplace_back(node);
//    }
//
//    void insertTransition(K state, T tran, K val){
//        while(transition.size() <= state)
//            transition.emplace_back();
//        transition[state][tran] = val;
//    }
//
//    static bool isCompleteElement(const std::pair<int, Rule>& rhs){
//        return rhs.second.size() == rhs.first;
//    }
//
//public:
//
//
//    void completeState(Node* node){
//        std::queue<ID>toUse;
//        //        std::unordered_multimap<ID, std::pair<K, T>> rules;
//        for(const auto& it: node->rules){
//            auto [pos,rule] = it.second;
//            if(pos < rule.size() && nonTerms.find(rule[pos]) != nonTerms.end()) toUse.push({rule[pos]});
//
//        }
//
//        while (!toUse.empty()){
//            int pos = 0;
//            auto range = rules.equal_range(toUse.front());
//            for (auto it = range.first; it != range.second;it++ ) {
//                auto temp1 = it->first;
//                auto temp2 = it->second;
//                if(!node->contains(it->first,it->second)){
//                    node->insert(it->first, it->second,pos);
//                    if(nonTerms.find(it->second[0]) != nonTerms.end()){
//                        toUse.push(it->second[0]);
//                    }
//                }
//            }
//            toUse.pop();
//        }
//
//    }
//
//    void makeTransition(std::queue<K> & toUse, K state){
//        std::unordered_map<ID, Node*> toComplete;
//
//
//        // makin all posible transitions
//        for(const auto& r: states[state]->rules){
//            const auto& [lhs, rhs] = r;
//            auto [pos,rule] = rhs;
//            if(rule[0] == epsilon || pos >= rule.size()) continue;
//
//            if(toComplete.find(rule[pos]) == toComplete.end()){
//                auto node = new Node;
//                node->insert(lhs , rule, pos + 1);
//                toComplete.insert({rule[pos],node});
//            }
//            else{
//                toComplete[rule[pos]]->insert(lhs , rule, pos + 1);
//            }
//        }
//
//
//        // completing anf checking for repetitions
//        for(auto it: toComplete){
//            bool flag = true;
//            auto node = it.second;
//            completeState(node);
//            // oh god, oh fuck. why i must do this.
//            // god are you there?
//            int counter = 0;
//            for(auto ip: states){
//                if(it.second->rules == ip->rules){
//                    insertTransition(state,it.first,counter);
//                    flag = false;
//                    break;
//                }
//                counter++;
//            }
//            if(flag){
//                insertState(node);
//                insertTransition(state,it.first,states.size() - 1);
//                toUse.push(states.size() - 1);
//            }
//        }
//
//    }
//
//    parserDfa(const Rules& rules, const ID  start, IDSet  nonTerms): rules(rules),
//    nonTerms(std::move(nonTerms)), start(start){
//        std::queue<K> toUse;
//        auto node =new Node;
//        insertState(node);
//
////        std::cout<<"range:"<<'\n';
//        auto range = rules.equal_range(start);
//        for (auto it = range.first; it != range.second;it++ ) {
////            std::cout<<it->first<<'\n';
//            for(auto rule: it->second){
//                node->insert(it->first, it->second);
////                std::cout<<rule<<" ";
//            }
////            std::cout<<std::endl;
//        }
//        completeState(node);
//        makeTransition(toUse,initialState);
//
//        while (!toUse.empty()){
//            makeTransition(toUse,toUse.front());
//            toUse.pop();
//        }
//
//    }
//
//    void printDfa(){
//        std::cout<<'\n';
////        std::cout<<states.size();
////        return ;
//        for(int i = 0; i < states.size(); i++){
//            std::cout<<"State: "<<i<<'\n';
//            auto node = states[i];
//            for(const auto& it: node->rules){
//                std::cout<<it.first<<" -> ";
//                auto [pos,rule] = it.second;
//                for(int j = 0; j < rule.size(); j++){
//                    if(pos == j) std::cout<<" . ";
//                    std::cout<<rule[j]<<" ";
//                }
//                if(pos == rule.size()) std::cout<<" . ";
//                std::cout<<"\n";
//            }
//        }
//    }
//
//    void printTransitions(){
//        std::cout<<'\n' <<"States Transition: "<<'\n';
//
//        for(int i = 0; i < transition.size(); i++){
//            std::cout<<"State "<<i<<":\n";
//            if(transition[i].empty()) std::cout<<"No Transitions \n";
//            for(auto it: transition[i]){
//                std::cout<<"     "<<it.first<<" -> "<<it.second<<'\n';
//            }
//        }
//    }
//
//};

//void print(const SetMap& m){
//    for(const auto & it: m){
//        std::cout<<it.first<<" :";
//        for(auto const & it2: it.second){
//            std::cout<<" "<<it2;
//        }
//        std::cout<<std::endl;
//    }
//}

#include "../include/scanner.hpp"
#include "../json/json.h"
#include <iostream>
#include <tuple>


struct SLRParser
{
private:
    using state = unsigned;
    using token = int;

    slrTable table;
    using input = std::vector<token>;

public:

    std::vector<Grammar::Type> errors;


    SLRParser () : table()
    {
    }

    auto parse (const vector<Lexer::Item>& input)
    {
        using Entry = struct
        {
            bool isState;
            Grammar::Type stored;
            Grammar::View strVal = "";
            int val = 0;
        };

        stack<Entry> symbolStack;
        symbolStack.emplace(Entry{ true, 0 });
        int index = 0;

        while (index < input.size())
        {
            if (symbolStack.top()
                           .isState) // read token from input and do an action
            {
                auto state = symbolStack.top().stored;
                std::pair<string, Grammar::Type> action;
                Grammar::Type token = input.at(index).token;


                action = table.action[state][token];

                if (action.first.empty()){
                    errors.push_back(index);

                    std::vector<unsigned > temp = table.goTo[state];

                    int i = 0;
                    Grammar::Type tempState = Grammar::$ERROR;

                    for(; i < temp.size(); i++){
                        if(temp[i] != Grammar::$ERROR){
                            auto expectedState = table.action[temp[i]][token];
                            if(!expectedState.first.empty()){
                                if(tempState != Grammar::$ERROR){
                                    if(expectedState.first != "r"){
                                        tempState = i;
                                        break;
                                    }
                                }
                                else tempState = i;
                            }
                        }
                    }


                    if(tempState != Grammar::$ERROR){
                        symbolStack.push(Entry{ false,Grammar::indexOfProduction(tempState),Grammar::PRODUCTION_STR[tempState], 0});
                        symbolStack.push(Entry{ true, temp[tempState] });
                        continue;
                    }
                    else{
                        index++;
                        continue;
                    }
                }
                else if (action.first == "acc") return true && (errors.empty());
                if (action.first == "s")
                {
                    symbolStack.push(Entry{ false, token,input.at(index).strVal, input.at(index).value });
                    symbolStack.push(Entry{ true, action.second });
                    index++;
                }
                else if (action.first == "r")
                {
                    auto[repl, rhs] = table.rules[action.second];
                    string strTemp = {};
                    int valTemp = {};
                    bool firstRead = true;

                    using namespace Grammar;
                    vector<Grammar::Type> multipication{Z7, Z9,Z10,Z13};
                    while (!rhs.empty())
                    {
                        // pooping the state entry
                        symbolStack.pop();
                        // checking the token entry
                        if (symbolStack.top().stored != rhs.back())
                            return false;

                        //checking for
                        auto item= symbolStack.top();
                        bool f = (find(multipication.begin(),multipication.end(),repl) == multipication.end());

                        if(firstRead || (f)){
                            valTemp += item.val;
                            if(!firstRead)std::cout<<"+";
                            firstRead = false;

                        }
                        else{
                            valTemp *= item.val;
                            cout<<"*";
                        }


                        cout<<item.val;
                        if(isProduction(item.stored))
                            cout<<"["<< Grammar::PRODUCTION_STR[item.stored - START]<<"]" ;

                        else cout<<"["<<Grammar::TOKEN_STR[item.stored]<<"]" ;


                        symbolStack.pop();
                        rhs.pop_back();
                    }
                    std::cout<<"=" <<valTemp<<"["<<Grammar::PRODUCTION_STR[repl - START] <<"]"<<'\n';
                    symbolStack.push(Entry{ false, repl,Grammar::PRODUCTION_STR[repl - START],valTemp });
                }
                else throw("a");
            }
            else // goto
            {
                auto rule = symbolStack.top();
                symbolStack.pop();
                auto state = symbolStack.top().stored;
                symbolStack.push(rule);
                symbolStack.push(Entry{ true, table.goTo[state][rule.stored] });
            }
        }
        return false;
    }

};





#endif //ASDFASDF_PARSER_HPP

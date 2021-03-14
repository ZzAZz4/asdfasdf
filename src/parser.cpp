#include "../include/parser.hpp"
#include "../include/followSet.hpp"
#include "../include/firstSet.hpp"




void print(SetMap m){
    for(const auto & it: m){
        std::cout<<it.first<<" :";
        for(auto const & it2: it.second){
            std::cout<<" "<<it2;
        }
        std::cout<<std::endl;
    }
}



SLRparser::SLRparser(const Rules &rules, ID start) {
    // cretate first and follow
    for (auto[lhs, _] : rules)
        nonTerms.emplace(lhs);

    first = makeFirst(rules, nonTerms);
    follow = makeFollow(rules, start, first, nonTerms);
    print(first);
    std::cout<<'\n';
    print(follow);

    // create the table


}

bool SLRparser::parse(std::vector<ID> s) {
    //TO DO
    return false;
}

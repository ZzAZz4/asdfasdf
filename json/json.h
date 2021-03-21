#ifndef JSON_HPP
#define JSON_HPP

#include "../include/common.hpp"
#include "../include/scanner.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
#include <sstream>


using namespace std;

using json = nlohmann::json;

namespace jsonTable{

    const string  file = "/home/ubuntu/Desktop/2020-0/compPy/json/table.json";
    const string ruleFile = "/home/ubuntu/Desktop/2020-0/compPy/json/rules.txt";

    json getJson(){
        json j;
        std::ifstream i(file);
        if(!i) throw("there was an error when opening the file");
        i >> j;
        i.close();
        return j;
    }

}


class slrTable{
    private:

    using actionType = std::string_view;
    using actionCont = std::vector<actionType>;
    
    using goToType = unsigned;
    using goToCont = std::vector<unsigned>;
    
    
    
    json j;

    std::vector<actionCont> action;
    std::vector<goToCont> goTo;
    std::vector<std::pair<string, std::vector<string>>>rules;
    


    public:
    
    // action(numTokens + $)
    slrTable():j(jsonTable::getJson()), action(j.size(), actionCont(Grammar::NUM_ALL_TOKENS + 1)),
    goTo(j.size(), goToCont(Grammar::NUM_NON_TERM + 1)){


        for (json::iterator it = j.begin(); it != j.end(); ++it) {
            auto row = *it;

            unsigned state = stoi(row.at("state").get<string>());
                
            for(int i = 1; i <= Grammar::NUM_ALL_TOKENS; i++){

                string val = row.at(string(Grammar::TOKEN_STR[i])).get<string>();
                // why the fuck does -62 char mean
                // also checking it because fuck me
                if(val[0] == -62 || val == " ") val = "";
                

                action[state][i] = val;
                
            }


            for(int i = 1; i <= Grammar::NUM_NON_TERM; i++){
                string val = row.at(string(Grammar::NON_TERM_STR[i])).get<string>();
                unsigned intVal;
                
                // seriusly why the fuck does -62 is there
                // does it even exist
                // or is god fucking with me?
                if(val[0] == -62 || val == " ") val = ""; 
                
                if(val==" " || val == "") intVal = Grammar::NONE;
                else intVal = stoi(val);

                
                goTo[state][i] = intVal;    
            }

             

        }

        std::ifstream i(jsonTable::ruleFile);
        std::string word;
        if(!i) throw("there was an error when opening the file");
        bool flag = true;

        std::string line;
        while (std::getline(i, line)){
            std::istringstream iss(line);
            string lhs, arrow;
            if (!(iss >> lhs >> arrow)) { break; } // error

            string str;
            rules.push_back({lhs,{}});

            while (iss>> str){
                rules[rules.size() - 1].second.push_back(str);
            }
            
        }

        /*
        for(int i = 0; i < rules.size();i++){
            cout<<rules[i].first<<" -> ";
            for(auto it: rules[i].second){
                cout<<it<<" ";
            }
            cout<<'\n';
        }
        */
        

    }

};

#endif //JSON_HPP

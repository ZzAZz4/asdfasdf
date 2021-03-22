#ifndef JSON_HPP
#define JSON_HPP

#include "../include/common.hpp"
#include "../include/scanner.hpp"
#include "json.hpp"
#include <fstream>
#include <sstream>


using namespace std;

using json = nlohmann::json;

namespace jsonTable{

    // insert yout full path
    const string  file = "C:/Users/Jean Paul/CLionProjects/compPy/json/table.json";
    const string ruleFile = "C:/Users/Jean Paul/CLionProjects/compPy/json/rules.txt";

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

    using grammarType = Grammar::Type ;

    using actionType = unsigned;
    using actionCont = std::vector<std::pair<string, grammarType>>;
    
    using goToType = unsigned;
    using goToCont = std::vector<goToType>;
    
    
    
    json j;

    std::vector<actionCont> action;
    std::vector<goToCont> goTo;
    std::vector<std::pair<grammarType, std::vector<grammarType>>>rules;



    public:
    
    // action(numTokens + $)
    slrTable():j(jsonTable::getJson()), action(j.size(), actionCont(Grammar::NUM_ALL_TOKENS)),
    goTo(j.size(), goToCont(Grammar::NUM_NON_TERM)){



            for (json::iterator it = j.begin(); it != j.end(); ++it) {
            auto row = *it;

            row.at("state").get<string>();
            unsigned state = stoi(row.at("state").get<string>());

            for(int i = 0; i < Grammar::NUM_ALL_TOKENS; i++){

                string val = row.at(string(Grammar::TOKEN_STR[i])).get<string>();
                // why the fuck does -62 char mean
                // also checking it because fuck me
                if(val[0] == -62 || val == " ") val = "";

                if(val[0] == 's') {
                    action[state][i].first = "s";
                    val = val.substr(1);
                    action[state][i].second = stoi(val);
                }
                else if(val[0] == 'r') {
                    action[state][i].first = "r";
                    val = val.substr(1);
                    action[state][i].second = stoi(val);

                }
                else if(val == "$"){
                    cout<<state;
                    action[state][i].first = "$";
                    action[state][i].second = Grammar::$end;
                }

                if(i == 1) cout<<Grammar::TOKEN_STR[i]<<"|"<<val<<'\n';

            }





            for(int i = 0; i < Grammar::NUM_NON_TERM; i++){
                string val = row.at(string(Grammar::NON_TERM_STR(i))).get<string>();
                unsigned intVal;

                // seriusly why the fuck does -62 is there
                // does it even exist
                // or is god fucking with me?
                if(val[0] == -62 || val == " ") val = "";

                if(val==" " || val == "") intVal = Grammar::$error;
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
            if (!(iss >> lhs >> arrow)) { cout<<"error";break; } // error

            string str;
            rules.push_back({Grammar::toEnum(lhs),{}});

            while (iss>> str){
                rules[rules.size() - 1].second.push_back(Grammar::toEnum(str));
            }

        }
        i.close();

        j.clear();




    }

};

#endif //JSON_HPP

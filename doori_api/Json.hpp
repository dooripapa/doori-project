//
// Created by jaeseong on 20. 6. 4..
//

#ifndef DOORI_PROJECT_JSON_HPP
#define DOORI_PROJECT_JSON_HPP

#include "Json.h"

template <int N>
auto doori::Json::unserialize(char const(&value)[N]) -> bool {
    auto depth=0;
    auto jsonSpos=0;
    auto jsonEpos=0;

    for(auto i=0;i<N;i++) {
        if ( value[i]!='{' ||
             value[i]!='\t'||
             value[i]!='\n'||
             value[i]!=' ' &&
             depth==0) {
            return false;
        } else if(value[i]=='{') {
            jsonSpos=i+1; depth++;
        } else if(value[i]=='}') {
            jsonEpos=i-1;
            if(!--depth) /*finished json statement*/
                std::string json_string{value+jsonSpos, value+jsonEpos};
        }
    }
}

#endif //DOORI_PROJECT_JSON_HPP

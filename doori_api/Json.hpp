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
        if ( value[i]=='\t'||
             value[i]=='\n'||
             value[i]==' ' &&
             depth==0) {
            continue;
        } else if(value[i]=='{') {
            if(!depth++) jsonSpos=i+1;
        } else if(value[i]=='}') {
            if(!--depth) jsonEpos=i;
        } else continue;
    }
    if( !jsonSpos || !jsonEpos)
        return false;

    if(depth) return false;

    std::string json_string{value.begin()+jsonSpos, value.begin()+jsonEpos};
    addJsonValueString(json_string);
    return true;
}

#endif //DOORI_PROJECT_JSON_HPP

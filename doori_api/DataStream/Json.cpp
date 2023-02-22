
//
// Created by jaeseong on 20. 4. 20..
//
#include <istream>
#include <sstream>
#include <term_entry.h>
#include <algorithm>
#include <limits>
#include "Json.h"

namespace doori::DataStream{

    auto Json::operator[](const std::string &jsonkey) -> Json_value & {
        for (auto &item : mFactors) {
            if(item.first == jsonkey)
                return item.second;
        }
        mFactors.push_back({jsonkey,Json_value()});
        auto it = mFactors.end()-1;
        return it->second;
    }

    /**
     * Json clear
     */
    auto Json::clear() -> void {
        mFactors.clear();
    }

    auto Json::serialize() const-> std::string {
        std::ostringstream jsonStr(std::ostringstream::ate);
        jsonStr<<"{";
        auto iCnt=0;
        for (auto &it:mFactors)
        {
            switch(it.second.TYPE)
            {
                case Json_value::BOOL:
                case Json_value::FLOAT:
                case Json_value::INT32S:
                case Json_value::STRING:
                case Json_value::JSON:
                    jsonStr<<"\""<<it.first.c_str()<<"\""<<":"<<it.second.toString();
                    break;
                case Json_value::ARRAY:
                    jsonStr<<"\""<<it.first.c_str()<<"\""<<":"<<it.second.toString();
                    break;
                case Json_value::NIL:
                default:
                    abort();
            }
            if(++iCnt < mFactors.size() )
                jsonStr<<",";
        }
        jsonStr<<"}";
        return jsonStr.str();
    }

    auto Json::append(const char *jsonKey, const Json_value &jsonValue) -> void {
        Json::operator[](jsonKey) = jsonValue;
    }

    Json::Json(const Json &rhs) {
        copyFrom(rhs);
    }

    Json::Json(Json &&rhs) {
        copyFrom(rhs);
        if (rhs.mFactors.size()) {
            rhs.mFactors.clear();
        }
    }

    auto Json::copyFrom(const Json &rhs) noexcept -> void {
        mFactors = rhs.mFactors;
    }

    auto Json::operator=(const Json &rhs) noexcept -> Json & {
        if( this ==&rhs  )
            return *this;

        copyFrom( rhs );
        return *this;
    }

    auto Json::operator=(Json &&rhs) noexcept -> Json & {
        if( this ==&rhs  )
            return *this;

        mFactors = std::move(rhs.mFactors);
        return *this;
    }

    Json::Json() {

    }

    /**
     * @param jsonValueString  "key":"value", ... ignore blank between key and value.
     * @return
     */
    auto Json::parserJsonString(const std::string &jsonValueString) -> bool {
        decltype(Json_value::TYPE) type;
        auto jsonDepth=0;
        auto IsFinishedKey=false;
        auto IsFinishedValue= false;
        auto jsonValueSKey=0;
        auto jsonValueEKey=0;
        auto jsonValueSValue=0;
        auto jsonValueEValue=0;

        auto max=jsonValueString.size();
        for(int i=0;i<max;++i) {
            if( isspace(jsonValueString[i]) ) continue; //ignore space.

            /*
             * if matching ',' and finishing value, is next Json_value
             */
            if(jsonValueString[i] == ',' && IsFinishedValue ) {
                IsFinishedKey=false;
                IsFinishedValue= false;
                jsonValueSKey=0;
                jsonValueEKey=0;
                jsonValueSValue=0;
                jsonValueEValue=0;
                continue;
            }
            /*
             * Json_value's Key
             */
            if(jsonValueString[i] == '"') { // "~~~~" as type
                jsonValueSKey=i;
                do{
                    ++i;
                    if(jsonValueString[i] == '\\')
                        ++i;
                    jsonValueEKey=i;
                }while(jsonValueString[i] != '"' );
                IsFinishedKey = true;
            } else
                return false;

            if(++i == max)
                return false;
            /*
             * if is space, ignore
             */
            while( isspace(jsonValueString[i]) ) ++i; //ignore space.

            if (jsonValueString[i] == ':' && IsFinishedKey)
                ;
            else
                return false;

            if(++i == max)
                return false;

            while( isspace(jsonValueString[i]) ) ++i; //ignore space.

            /*
             * Json_value's Value
             * if {~} -> Json
             */
            if(jsonValueString[i] == '{'){
                ++jsonDepth;
                if(jsonDepth==1) jsonValueSValue=i;

                while(1) {
                    if(++i == max)
                        return false;
                    if(jsonValueString[i] == '{') ++jsonDepth;
                    else if(jsonValueString[i] == '}'){
                        if(!--jsonDepth) {
                            jsonValueEValue=i;
                            IsFinishedValue=true;
                            break;
                        }
                    }
                }
                if(jsonDepth > 0 || !IsFinishedValue) // error
                    return false;

                auto key=jsonValueString.substr(jsonValueSKey + 1, jsonValueEKey - (jsonValueSKey + 1));
                auto value=jsonValueString.substr(jsonValueSValue, (jsonValueEValue + 1) - jsonValueSValue);

                Json json;
                if(!json.unserialize(value))
                    return false;
                mFactors.emplace_back(key, json);
                continue;
            }

            /*
             * Json_value's Value
             * if [~] -> Json_value array
             */
            if(jsonValueString[i] == '['){
                jsonValueSValue=i;

                while(1) {
                    if(++i == max)
                        return false;
                    if(jsonValueString[i] == ']'){
                        jsonValueEValue=i;
                        IsFinishedValue=true;
                        break;
                    }
                }
                if(!IsFinishedValue) // error
                    return false;

                auto key=jsonValueString.substr(jsonValueSKey + 1, jsonValueEKey - (jsonValueSKey + 1));
                auto value=jsonValueString.substr(jsonValueSValue + 1, jsonValueEValue - (jsonValueSValue + 1));
                Json_value jsonValue;
                if(!jsonValue.parserJsonArray(value))
                    return false;
                mFactors.emplace_back(key, jsonValue);
                continue;
            }

            /*
             * Json_value's Value
             * "~"
             */
            if(jsonValueString[i] == '"') { // "~~~~" as type
                jsonValueSValue = i;
                do{
                    if(++i==max)
                        return false;
                    if(jsonValueString[i] == '\\')
                        if(++i==max)
                            return false;
                    jsonValueEValue=i;
                }while(jsonValueString[i] != '"' );
                IsFinishedValue = true;
                type=Json_value::STRING;
            }

            /*
             * Json_value's Value
             * number
             * there is not separator. Spos-=1, Epos+=1
             */
            if( isdigit(jsonValueString[i]) ) { // ex) 02.23, 12345, 2.
                type=Json_value::INT32S;
                jsonValueSValue = i-1;
                do{
                    if(jsonValueString[i] == '.')
                        type=Json_value::FLOAT;
                    ++i;
                    jsonValueEValue=i+1;
                    if(i==max || isspace(jsonValueString[i])) {
                        break;
                    }
                }while(isdigit(jsonValueString[i] ) || jsonValueString[i] == '.');
                IsFinishedValue = true;
            }

            /*
             * Json_value's Value
             * true
             * there is not separator. Spos-=1, Epos+=1
             */
            if(jsonValueString[i] == 't' ) {
                if(i+3==max)
                    return false;
                if(jsonValueString[i + 1] == 'r' &&
                   jsonValueString[i + 2] == 'u' &&
                   jsonValueString[i + 3] == 'e' ) {
                    jsonValueSValue=i-1;
                    jsonValueEValue=i+3+1;
                } else
                    return false;
                i+=3;
                if( !isspace(i+1)||(i+1)!=max )
                    return false;
                IsFinishedValue=true;
                type=Json_value::BOOL;
            }

            /*
             * Json_value's Value
             * true
             * there is not separator. Spos-=1, Epos+=1
             */
            if(jsonValueString[i] == 'f' ) {
                if(i+4 == max)
                    return false;
                else if(jsonValueString[i + 1] == 'a' &&
                        jsonValueString[i + 2] == 'l' &&
                        jsonValueString[i + 3] == 's' &&
                        jsonValueString[i + 4] == 'e' ) {
                    jsonValueSValue=i-1;
                    jsonValueEValue=i+4+1;
                } else
                    return false;
                i+=4;
                if( !isspace(i+1)||(i+1)!=max )
                    return false;
                IsFinishedValue=true;
                type=Json_value::BOOL;
            }

            if( jsonValueSValue<jsonValueEValue &&
                jsonValueEKey  <jsonValueSValue &&
                jsonValueSKey  <jsonValueEKey );
            else return false;

            auto key=jsonValueString.substr(jsonValueSKey + 1, jsonValueEKey - (jsonValueSKey + 1));
            auto value=jsonValueString.substr(jsonValueSValue + 1, jsonValueEValue - (jsonValueSValue + 1));

            switch (type) {
                case Json_value::INT32S:
                    mFactors.emplace_back(key, std::stoi(value.c_str(),nullptr,10));
                    break;
                case Json_value::FLOAT:
                    mFactors.emplace_back(key, std::strtof(value.c_str(), nullptr) );
                    break;
                case Json_value::BOOL:
                    if(value=="true")
                        mFactors.emplace_back(key, true );
                    else if(value=="false")
                        mFactors.emplace_back(key, false);
                    else
                        mFactors.emplace_back(key, "error");
                    break;
                case Json_value::STRING:
                    mFactors.emplace_back(key, value);
                    break;
                case Json_value::NIL:
                case Json_value::JSON:
                    break;
                default:
                    return false;
            }
        }
        return true;
    }



    /**
     *
     * @param value {"key":"value,...}
     * @return
     */
    auto Json::unserialize(const std::string &value) -> bool {
        auto depth=0;
        auto jsonSpos=0;
        auto jsonEpos=0;
        auto i=0;
        for(auto it=value.begin();it!=value.end();++it, ++i) {
            if ( *it=='\t'||
                 *it=='\n'||
                 *it==' ' &&
                 depth==0) {
                continue;
            } else if(*it=='{') {
                if(!depth++) jsonSpos=i+1;
            } else if(*it=='}') {
                if(!--depth) jsonEpos=i;
            } else continue;
        }
        if( !jsonSpos || !jsonEpos)
            return false;

        if(depth) return false;

        std::string json_string{value.begin()+jsonSpos, value.begin()+jsonEpos};
        parserJsonString(json_string);
        return true;
    }

    auto Json_value::set(int32_t value) -> void {
        TYPE = INT32S;
        mInt = value;
    }

    auto Json_value::set(std::string value) -> void {
        TYPE = STRING;
        mStr = value;
    }

    auto Json_value::set(float value) -> void {
        TYPE = FLOAT;
        mFloat = value;
    }

    auto Json_value::set(bool value) -> void {
        TYPE = BOOL;
        mBool = value;
    }

    auto Json_value::set(Json value) -> void {
        TYPE = JSON;
        mJson = std::make_shared<Json>(value);
    }

    auto Json_value::toString() const-> std::string {
        std::ostringstream jsonV(std::ostringstream::ate);
        switch(TYPE)
        {
            case Json_value::BOOL:
                return(mBool?"true":"false");
            case Json_value::FLOAT:
                jsonV<<mFloat;
                return jsonV.str();
            case Json_value::INT32S:
                jsonV<<mInt;
                return jsonV.str();
            case Json_value::STRING:
//                jsonV<<"\"";
                for(auto&i:mStr) {
                    if (i=='"')
                        jsonV<<"\\";
                    jsonV<<i;
                }
//                jsonV<<"\"";
                return jsonV.str();
            case Json_value::ARRAY:
                jsonV<<"[";
                for(auto it=mArray.begin();it!=mArray.end();++it) {
                    jsonV<<it->toString();
                    if( it+1 == mArray.end() );
                    else jsonV<<",";
                }
                jsonV<<"]";
                return jsonV.str();
            case Json_value::JSON:
                return mJson->serialize();
            case Json_value::NIL:
            default:
                abort();
        }
    }

    auto Json_value::copyFrom(const Json_value &rhs) noexcept -> void {
        TYPE   =rhs.TYPE;
        mInt   =rhs.mInt;
        mStr   =rhs.mStr;
        mFloat =rhs.mFloat;
        mBool  =rhs.mBool;
        mArray =rhs.mArray;
        mJson  =rhs.mJson;
    }

    Json_value::Json_value(const Json_value &rhs) {
        copyFrom(rhs);
    }

    Json_value::Json_value(Json_value &&rhs) {
        TYPE = rhs.TYPE;
        mInt = rhs.mInt;
        mStr = rhs.mStr;
        mFloat= rhs.mFloat;
        mBool = rhs.mBool;
        mArray = std::move(rhs.mArray);
        mJson = std::move(rhs.mJson);
    }

    Json_value::Json_value(int32_t value) : mInt(value), TYPE(INT32S){

    }

    Json_value::Json_value(std::string value) : mStr(value), TYPE(STRING){

    }

    Json_value::Json_value(float value) : mFloat(value), TYPE(FLOAT){

    }

    Json_value::Json_value(bool value) : mBool(value), TYPE(BOOL){

    }

    Json_value::Json_value(const Json &value) : mJson( std::make_shared<Json>(value) ), TYPE(JSON){

    }

    Json_value::Json_value(Json &&value) : mJson( std::make_shared<Json>(std::move(value)) ), TYPE(JSON){

    }

    auto Json_value::operator=(const Json_value &rhs) noexcept -> Json_value & {
        if( this ==&rhs  )
            return *this;

        copyFrom( rhs );
        return *this;
    }

    auto Json_value::operator=(Json_value &&rhs) noexcept -> Json_value & {
        if( this ==&rhs  )
            return *this;

        TYPE = rhs.TYPE;
        mInt = rhs.mInt;
        mStr = rhs.mStr;
        mFloat= rhs.mFloat;
        mBool = rhs.mBool;
        mArray = std::move(rhs.mArray);
        mJson = std::move(rhs.mJson);

        return *this;
    }

    Json_value::Json_value() {
        mInt    = std::numeric_limits<decltype(mInt  )>::quiet_NaN();
        mStr    = std::numeric_limits<decltype(mStr  )>::quiet_NaN();
        mFloat  = std::numeric_limits<decltype(mFloat)>::quiet_NaN();
        mBool   = std::numeric_limits<decltype(mBool )>::quiet_NaN();
        TYPE=NIL;
    }

    auto Json_value::set(const Json_value &rhs) -> void {
        copyFrom(rhs);
    }

    auto Json_value::set(Json_value &&rhs) -> void {
        TYPE = rhs.TYPE;
        mInt = rhs.mInt;
        mStr = rhs.mStr;
        mFloat= rhs.mFloat;
        mBool = rhs.mBool;
        mArray = std::move(rhs.mArray);
        mJson = std::move(rhs.mJson);
    }

    auto Json_value::operator=(const Json &value) noexcept -> Json_value & {
        TYPE = JSON;
        mJson = std::make_shared<Json>(value);
        return *this;
    }

    auto Json_value::operator=(Json &&value) noexcept -> Json_value & {
        TYPE = JSON;
        mJson = std::make_shared<Json>(std::move(value));
        return *this;
    }

    auto Json_value::append(const Json_value & value) -> void {
        TYPE = ARRAY;
        mArray.push_back(value);
    }

    /**
     * @param str  "value" , { } ... 결합된 json array 문자열
     * @return
     */
    auto Json_value::parserJsonArray(const std::string &str) -> bool{
        decltype(Json_value::TYPE) type;
        auto jsonDepth=0;
        auto IsFinishedValue= false;
        auto jsonValueSValue=0;
        auto jsonValueEValue=0;

        auto max=str.size();
        for(int i=0;i<max;++i) {
            if( isspace(str[i]) ) continue; //ignore space.

            /*
             * if matching ',' and finishing value, is next Json_value
             */
            if(str[i] == ',' && IsFinishedValue ) {
                IsFinishedValue= false;
                jsonValueSValue=0;
                jsonValueEValue=0;
                continue;
            }
            /*
             * Json_value's Value
             * if {~} -> Json
             */
            if(str[i] == '{'){
                ++jsonDepth;
                if(jsonDepth==1) jsonValueSValue=i;

                while(1) {
                    if(++i == max)
                        return false;
                    if(str[i] == '{') ++jsonDepth;
                    else if(str[i] == '}'){
                        if(!--jsonDepth) {
                            jsonValueEValue=i;
                            IsFinishedValue=true;
                            break;
                        }
                    }
                }
                if(jsonDepth > 0 || !IsFinishedValue) // error
                    return false;

                auto value=str.substr(jsonValueSValue, (jsonValueEValue + 1) - jsonValueSValue);

                Json json;
                if(!json.unserialize(value))
                    return false;
                mArray.emplace_back(json);
                continue;
            }

            /*
             * Json_value's Value
             * "~"
             */
            if(str[i] == '"') { // "~~~~" as type
                jsonValueSValue = i;
                do{
                    if(++i==max)
                        return false;
                    if(str[i] == '\\')
                        if(++i==max)
                            return false;
                    jsonValueEValue=i;
                }while(str[i] != '"' );
                IsFinishedValue = true;
                type=Json_value::STRING;
            }

            /*
             * Json_value's Value
             * number
             * there is not separator. Spos-=1, Epos+=1
             */
            if( isdigit(str[i]) ) { // ex) 02.23, 12345, 2.
                type=Json_value::INT32S;
                jsonValueSValue = i-1;
                do{
                    if(str[i] == '.')
                        type=Json_value::FLOAT;
                    ++i;
                    jsonValueEValue=i+1;
                    if(i==max || isspace(str[i])) {
                        break;
                    }
                }while(isdigit(str[i] ) || str[i] == '.');
                IsFinishedValue = true;
            }

            /*
             * Json_value's Value
             * true
             * there is not separator. Spos-=1, Epos+=1
             */
            if(str[i] == 't' ) {
                if(i+3==max)
                    return false;
                if(str[i + 1] == 'r' &&
                   str[i + 2] == 'u' &&
                   str[i + 3] == 'e' ) {
                    jsonValueSValue=i-1;
                    jsonValueEValue=i+3+1;
                } else
                    return false;
                i+=3;
                if( !isspace(i+1)||(i+1)!=max )
                    return false;
                IsFinishedValue=true;
                type=Json_value::BOOL;
            }

            /*
             * Json_value's Value
             * true
             * there is not separator. Spos-=1, Epos+=1
             */
            if(str[i] == 'f' ) {
                if(i+4 == max)
                    return false;
                else if(str[i + 1] == 'a' &&
                        str[i + 2] == 'l' &&
                        str[i + 3] == 's' &&
                        str[i + 4] == 'e' ) {
                    jsonValueSValue=i-1;
                    jsonValueEValue=i+4+1;
                } else
                    return false;
                i+=4;
                if( !isspace(i+1)||(i+1)!=max )
                    return false;
                IsFinishedValue=true;
                type=Json_value::BOOL;
            }

            if( jsonValueSValue<jsonValueEValue  );
            else
                return false;

            auto value=str.substr(jsonValueSValue + 1, jsonValueEValue - (jsonValueSValue + 1));

            switch (type) {
                case Json_value::INT32S:
                    mArray.emplace_back(std::stoi(value.c_str(),nullptr,10));
                    break;
                case Json_value::FLOAT:
                    mArray.emplace_back(std::strtof(value.c_str(), nullptr) );
                    break;
                case Json_value::BOOL:
                    if(value=="true")
                        mArray.emplace_back(true );
                    else if(value=="false")
                        mArray.emplace_back(false);
                    else
                        return false;
                case Json_value::STRING:
                    mArray.emplace_back(value);
                    break;
                case Json_value::NIL:
                case Json_value::JSON:
                    break;
                default:
                    return false;
            }
        }
        TYPE=Json_value::ARRAY;
        return true;
    }
}

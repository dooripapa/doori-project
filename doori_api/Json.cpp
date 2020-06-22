
//
// Created by jaeseong on 20. 4. 20..
//
#include <istream>
#include <sstream>
#include <term_entry.h>
#include <algorithm>
#include "Json.h"

namespace doori{

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
     * @param jsonValueStr  "key":"value", ... ignore blank between key and value.
     * @return
     */
    auto Json::addJsonValueString(const std::string &jsonValueStr) -> bool {
        decltype(Json_value::TYPE) type;
        auto jsonDepth=0;
        auto IsFinishedKey=false;
        auto IsFinishedValue= false;
        auto jsonValueSKey=0;
        auto jsonValueEKey=0;
        auto jsonValueSValue=0;
        auto jsonValueEValue=0;

        auto max=jsonValueStr.size();
        for(int i=0;i<max;++i) {
            if( isspace(jsonValueStr[i]) ) continue; //ignore space.

            /*
             * if matching ',' and finishing value, is next Json_value
             */
            if( jsonValueStr[i]==',' && IsFinishedValue ) {
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
            if(jsonValueStr[i] == '"') { // "~~~~" as type
                jsonValueSKey=i;
                do{
                    ++i;
                    if(jsonValueStr[i] == '\\')
                        ++i;
                    jsonValueEKey=i;
                }while( jsonValueStr[i] != '"' );
                IsFinishedKey = true;
            } else
                return false;

            if(++i == max)
                return false;
            /*
             * if is space, ignore
             */
            while( isspace(jsonValueStr[i]) ) ++i; //ignore space.

            if (jsonValueStr[i]==':' && IsFinishedKey)
                ;
            else
                return false;

            if(++i == max)
                return false;

            while( isspace(jsonValueStr[i]) ) ++i; //ignore space.

            /*
             * Json_value's Value
             * if {~} -> Json
             */
            if(jsonValueStr[i]=='{'){
                ++jsonDepth;
                if(jsonDepth==1) jsonValueSValue=i;

                while(1) {
                    if(++i == max)
                        return false;
                    if(jsonValueStr[i]=='{') ++jsonDepth;
                    else if(jsonValueStr[i]=='}'){
                        if(!--jsonDepth) {
                            jsonValueEValue=i;
                            IsFinishedValue=true;
                            break;
                        }
                    }
                }
                if(jsonDepth > 0 || !IsFinishedValue) // error
                    return false;

                auto key=jsonValueStr.substr(jsonValueSKey+1, jsonValueEKey-(jsonValueSKey+1));
                auto value=jsonValueStr.substr(jsonValueSValue, (jsonValueEValue+1)-jsonValueSValue);

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
            if(jsonValueStr[i]=='['){
                jsonValueSValue=i;

                while(1) {
                    if(++i == max)
                        return false;
                    if(jsonValueStr[i]==']'){
                        jsonValueEValue=i;
                        IsFinishedValue=true;
                        break;
                    }
                }
                if(!IsFinishedValue) // error
                    return false;

                auto key=jsonValueStr.substr(jsonValueSKey+1, jsonValueEKey-(jsonValueSKey+1));
                auto value=jsonValueStr.substr(jsonValueSValue, (jsonValueEValue+1)-jsonValueSValue);

                Json json;
                if(!json.unserialize(value))
                    return false;
                mFactors.emplace_back(key, json);
                continue;
            }

            /*
             * Json_value's Value
             * "~"
             */
            if(jsonValueStr[i] == '"') { // "~~~~" as type
                jsonValueSValue = i;
                do{
                    if(++i==max)
                        return false;
                    if(jsonValueStr[i] == '\\')
                        if(++i==max)
                            return false;
                    jsonValueEValue=i;
                }while( jsonValueStr[i] != '"' );
                IsFinishedValue = true;
                type=Json_value::STRING;
            }

            /*
             * Json_value's Value
             * number
             * there is not separator. Spos-=1, Epos+=1
             */
            if( isdigit(jsonValueStr[i]) ) { // ex) 02.23, 12345, 2.
                type=Json_value::INT32S;
                jsonValueSValue = i-1;
                do{
                    if(jsonValueStr[i]=='.')
                        type=Json_value::FLOAT;
                    ++i;
                    jsonValueEValue=i+1;
                    if(i==max || isspace(jsonValueStr[i])) {
                        break;
                    }
                }while( isdigit( jsonValueStr[i] ) || jsonValueStr[i]=='.');
                IsFinishedValue = true;
            }

            /*
             * Json_value's Value
             * true
             * there is not separator. Spos-=1, Epos+=1
             */
            if( jsonValueStr[i]=='t' ) {
                if(i+3==max)
                    return false;
                if( jsonValueStr[i+1] == 'r' &&
                    jsonValueStr[i+2] == 'u' &&
                    jsonValueStr[i+3] == 'e' ) {
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
            if( jsonValueStr[i]=='f' ) {
                if(i+4 == max)
                    return false;
                else if(jsonValueStr[i+1] == 'a' &&
                        jsonValueStr[i+2] == 'l' &&
                        jsonValueStr[i+3] == 's' &&
                        jsonValueStr[i+4] == 'e' ) {
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

            auto key=jsonValueStr.substr(jsonValueSKey+1, jsonValueEKey-(jsonValueSKey+1));
            auto value=jsonValueStr.substr(jsonValueSValue+1, jsonValueEValue-(jsonValueSValue+1));

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
        addJsonValueString(json_string);
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
                jsonV<<"\"";
                for(auto&i:mStr) {
                    if (i=='"')
                        jsonV<<"\\";
                    jsonV<<i;
                }
                jsonV<<"\"";
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
     * @param arryValueStr  "key", "value" , { } ... 결합된 json array 문자열
     * @return
     */
    auto Json_value::addArrayString(const std::string &arryValueStr) -> bool{
        decltype(Json_value::TYPE) type;
        auto jsonDepth=0;
        auto IsFinishedValue= false;
        auto jsonValueSValue=0;
        auto jsonValueEValue=0;

        auto max=arryValueStr.size();
        for(int i=0;i<max;++i) {
            if( isspace(arryValueStr[i]) ) continue; //ignore space.

            /*
             * if matching ',' and finishing value, is next Json_value
             */
            if( arryValueStr[i]==',' && IsFinishedValue ) {
                IsFinishedValue= false;
                jsonValueSValue=0;
                jsonValueEValue=0;
                continue;
            }
            /*
             * Json_value's Value
             * if {~} -> Json
             */
            if(arryValueStr[i]=='{'){
                ++jsonDepth;
                if(jsonDepth==1) jsonValueSValue=i;

                while(1) {
                    if(++i == max)
                        return false;
                    if(arryValueStr[i]=='{') ++jsonDepth;
                    else if(arryValueStr[i]=='}'){
                        if(!--jsonDepth) {
                            jsonValueEValue=i;
                            IsFinishedValue=true;
                            break;
                        }
                    }
                }
                if(jsonDepth > 0 || !IsFinishedValue) // error
                    return false;

                auto value=arryValueStr.substr(jsonValueSValue, (jsonValueEValue+1)-jsonValueSValue);

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
            if(arryValueStr[i] == '"') { // "~~~~" as type
                jsonValueSValue = i;
                do{
                    if(++i==max)
                        return false;
                    if(arryValueStr[i] == '\\')
                        if(++i==max)
                            return false;
                    jsonValueEValue=i;
                }while( arryValueStr[i] != '"' );
                IsFinishedValue = true;
                type=Json_value::STRING;
            }

            /*
             * Json_value's Value
             * number
             * there is not separator. Spos-=1, Epos+=1
             */
            if( isdigit(arryValueStr[i]) ) { // ex) 02.23, 12345, 2.
                type=Json_value::INT32S;
                jsonValueSValue = i-1;
                do{
                    if(arryValueStr[i]=='.')
                        type=Json_value::FLOAT;
                    ++i;
                    jsonValueEValue=i+1;
                    if(i==max || isspace(arryValueStr[i])) {
                        break;
                    }
                }while( isdigit( arryValueStr[i] ) || arryValueStr[i]=='.');
                IsFinishedValue = true;
            }

            /*
             * Json_value's Value
             * true
             * there is not separator. Spos-=1, Epos+=1
             */
            if( arryValueStr[i]=='t' ) {
                if(i+3==max)
                    return false;
                if( arryValueStr[i+1] == 'r' &&
                    arryValueStr[i+2] == 'u' &&
                    arryValueStr[i+3] == 'e' ) {
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
            if( arryValueStr[i]=='f' ) {
                if(i+4 == max)
                    return false;
                else if(arryValueStr[i+1] == 'a' &&
                        arryValueStr[i+2] == 'l' &&
                        arryValueStr[i+3] == 's' &&
                        arryValueStr[i+4] == 'e' ) {
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

            auto value=arryValueStr.substr(jsonValueSValue+1, jsonValueEValue-(jsonValueSValue+1));

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
        return true;

    }
}


//
// Created by jaeseong on 20. 4. 20..
//
#include <istream>
#include <sstream>
#include "json.h"

namespace doori{

    auto Json::operator[](const std::string &jsonkey) -> Json_value & {
        return mFactors[jsonkey];
    }

    auto Json::clear() -> void {
        mFactors.clear();
    }

    auto Json::toString() -> std::string {
        std::ostringstream jsonStr(std::ostringstream::ate);
        jsonStr<<"{";
        for(auto &i:mFactors)
        {
            switch(i.second.TYPE)
            {
                case Json_value::BOOL:
                case Json_value::FLOAT:
                case Json_value::INT32S:
                case Json_value::STRING:
                case Json_value::JSON:
                    jsonStr<<"\""<<i.first.c_str()<<"\""<<":"<<i.second.toString();
                    break;
                case Json_value::NIL:
                default:
                    abort();
            }
        }
        jsonStr<<"}";
    }

    auto Json::fromString(const char *json) -> bool {
        return false;
    }

    auto Json::append(const char *jsonKey, const Json_value &jsonValue) -> void {
        mFactors[jsonKey] = jsonValue;
    }

    auto Json_value::set(int32_t value) -> void {
        TYPE = INT32S;
        mInt = value;
    }

    auto Json_value::set(std::string value) -> void {
        TYPE = INT32S;
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
        mJson = std::make_unique<Json>(value);
    }

    auto Json_value::toString() -> std::string {
        std::ostringstream jsonV(std::ostringstream::ate);
        switch(TYPE)
        {
            case Json_value::BOOL:
                return(mBool?"true":"false");
            case Json_value::FLOAT:
                jsonV<<"\""<<mFloat<<"\"";
                return jsonV.str();
            case Json_value::INT32S:
                jsonV<<"\""<<mInt<<"\"";
                return jsonV.str();
            case Json_value::STRING:
                jsonV<<"\""<<mStr<<"\"";
                return jsonV.str();
            case Json_value::JSON:
                return mJson->toString();
            case Json_value::NIL:
            default:
                abort();
        }
    }

    Json_value::Json_value(int32_t value) : mInt(value), TYPE(INT32S)  {}
}

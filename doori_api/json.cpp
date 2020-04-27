
//
// Created by jaeseong on 20. 4. 20..
//
#include "json.h"

namespace doori{

    auto Json::operator[](const std::string &jsonkey) -> Json_value & {
        return mFactors[jsonkey];
    }

    auto Json::clear() -> void {
        mFactors.clear();
    }

    auto Json::toString() -> std::string {
        for(auto &i:mFactors)
        {
            switch(i.second.TYPE)
            {
                case Json_value::BOOL:
            }
        }
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
}

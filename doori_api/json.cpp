
//
// Created by jaeseong on 20. 4. 20..
//
#include "json.h"

namespace doori{

    auto Json::operator[](const char *jsonkey) -> Json & {
        return <#initializer#>;
    }

    auto Json::clear() -> void {

    }

    auto Json::toString() -> std::string {
        return std::__cxx11::string();
    }

    auto Json::fromString(const char *json) -> bool {
        return false;
    }

    auto Json::append(const char *jsonkey, const Json &json) -> void {
        std::unordered_map<const char*, Json>
        mFactors.push_back()
    }

    auto Json::Json_value::set(int32_t value) -> void {
        Json::Json_value::TYPE = INT32;
        Json::Json_value::mInt = value;
    }

    auto Json::Json_value::set(std::string value) -> void {
        Json::Json_value::TYPE = Json::Json_value::INT32;
        Json::Json_value::mInt = value;
    }
}

//
// Created by jaeseong on 20. 4. 20..
//

#ifndef DOORI_API_JSON_H
#define DOORI_API_JSON_H



#include <cstdint>
#include <string>>
#include <memory>
#include <vector>
#include <unordered_map>


namespace doori{
    class Json;

    class Json_value{
    public:
        enum {
            NIL,
            INT32S,
            STRING,
            FLOAT,
            BOOL,
            JSON
        }TYPE;
        auto set(int32_t value) -> void;
        auto set(std::string value) -> void;
        auto set(float value) -> void;
        auto set(bool value) -> void;
        auto set(Json value) -> void;
        auto toString() -> std::string;
    private:
        int32_t     mInt;
        std::string mStr;
        float       mFloat;
        bool        mBool;
        std::shared_ptr<Json> mJson;
    };

    class Json {
    public:
        auto append(const char* jsonKey, const Json_value &jsonValue) -> void;
        auto operator[](const std::string &jsonkey) -> Json_value&;
        auto toString() -> std::string;
        auto fromString(const char *json) -> bool;
        auto clear() -> void;
    private:
        std::unordered_map<std::string, Json_value> mFactors;
    };
}
#endif //DOORI_API_JSON_H

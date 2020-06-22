//
// Created by jaeseong on 20. 4. 20..
//

#ifndef DOORI_API_JSON_H
#define DOORI_API_JSON_H



#include <cstdint>
#include <string>
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
            ARRAY,
            JSON
        }TYPE;
        Json_value();
        Json_value(const Json_value &rhs);
        Json_value(Json_value &&rhs);
        Json_value(int32_t value);
        Json_value(std::string value);
        template <int N>
        Json_value(char const(&value)[N]){mStr=std::string(value),TYPE=STRING;};
        Json_value(float value);
        Json_value(bool value);
        Json_value(const Json& value);
        Json_value(Json&& value);

        auto set(const Json_value &rhs) -> void;
        auto set(Json_value &&rhs) -> void;
        auto set(int32_t value) -> void;
        auto set(std::string value) -> void;
        template <int N>
        auto set(char const(&value)[N]){mStr=std::string(value),TYPE=STRING;};
        auto set(float value) -> void;
        auto set(bool value) -> void;
        auto set(Json value) -> void;
        [[nodiscard]] auto toString() const -> std::string;
        auto append(const Json_value & value) -> void;
        auto operator=(const Json_value &rhs) noexcept -> Json_value&;
        auto operator=(Json_value &&rhs) noexcept -> Json_value&;
        auto operator=(const Json &value) noexcept -> Json_value&;
        auto operator=(Json &&value) noexcept -> Json_value&;
    private:
        auto addArrayString(const std::string& str) -> bool;
        auto copyFrom(const Json_value &rhs) noexcept -> void;
        int32_t     mInt;
        std::string mStr;
        float       mFloat;
        bool        mBool;
        std::vector<Json_value> mArray;
        std::shared_ptr<Json> mJson;
    };

    class Json {
    public:
        Json();
        Json(Json&& rhs);
        Json(const Json& rhs);
        auto append(const char* jsonKey, const Json_value &jsonValue) -> void;
        auto operator[](const std::string &jsonkey) -> Json_value&;
        [[nodiscard]] auto serialize() const-> std::string;
        template <int N>
        auto unserialize(char const(&value)[N]) -> bool;
        auto unserialize(const std::string &value) -> bool;
        auto clear() -> void;
        auto operator=(const Json &rhs) noexcept -> Json&;
        auto operator=(Json &&rhs) noexcept -> Json&;
    private:
        auto addJsonValueString(const std::string &jsonValueString) -> bool;
        auto copyFrom(const Json &rhs) noexcept -> void;
        std::vector< std::pair<std::string, Json_value> > mFactors;
    };
}

#include "Json.hpp"
#endif //DOORI_API_JSON_H

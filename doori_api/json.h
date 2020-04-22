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
    class Json {
    public:
        class Json_value{
        public:
            enum TYPE{
                NIL,
                INT32,
                STRING,
                FLOAT,
                BOOL,
                JSON
            };
            auto set(int32_t value) -> void;
            auto set(std::string value) -> void;
            auto set(float value) -> void;
            auto set(bool value) -> void;
        private:
            int32_t     mInt;
            std::string mStr;
            float       mFloat;
            bool        mBool;
        };
        auto append(const char* jsonkey, const Json& json) -> void;
        auto operator[](const char* jsonkey) -> Json&;
        auto toString() -> std::string;
        auto fromString(const char *json) -> bool;
        auto clear() -> void;
    private:
        auto set(const Json_value& value) -> void;
        auto link( std::unordered_map<const char*, Json> factor ) -> void;
        std::vector< std::unordered_map<const char*, Json> > mFactors;
    };
}
#endif //DOORI_API_JSON_H

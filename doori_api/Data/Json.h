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

namespace doori::api::Data{
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
        /**
         * 기본생성자
         */
        Json_value();
        /**
         * 복사생성자
         * @param rhs
         */
        Json_value(const Json_value &rhs);
        /**
         * 이동생성자
         * @param rhs
         */
        Json_value(Json_value &&rhs);
        /**
         * int32 초기화
         * @param value int32
         */
        Json_value(int32_t value);
        /**
         * string 초기화
         * @param value
         */
        Json_value(std::string value);
        /**
         * C스타일 리터럴 문자열로 초기화
         * @tparam N
         * @param value "~~" 문자열
         */
        template <int N>
        Json_value(char const(&value)[N]){mStr=std::string(value),TYPE=STRING;};
        /**
         * float 초기화
         * @param value float
         */
        Json_value(float value);
        /**
         * bool 초기화
         * @param value bool
         */
        Json_value(bool value);
        /**
         * Json 으로 초기화(복사)
         * @param value const Json&
         */
        Json_value(const Json& value);
        /**
         * Json 으로 초기화(이동)
         * @param value Json&&
         */
        Json_value(Json&& value);
        /**
         * Json_value 형식으로 초기화(복사)
         * @param rhs
         */
        auto set(const Json_value &rhs) -> void;
        /**
         * Json_value 형식으로 초기화(이동)
         * @param rhs
         */
        auto set(Json_value &&rhs) -> void;
        /**
         * int32형식으로 초기화
         * @param value
         */
        auto set(int32_t value) -> void;
        /**
         * string 초기화
         * @param value
         */
        auto set(std::string value) -> void;
        /**
         * C스타일 리터럴 문자열로 초기화
         * @tparam N
         * @param value "~~" 문자열
         * @return
         */
        template <int N>
        auto set(char const(&value)[N]){mStr=std::string(value),TYPE=STRING;};
        /**
         * float 초기화
         * @param value  float
         */
        auto set(float value) -> void;
        /**
         * bool 초기화
         * @param value bool
         */
        auto set(bool value) -> void;
        /**
         * Json 초기화
         * @param value Json
         */
        auto set(Json value) -> void;
        /**
         * 문자열로 출력
         * @return std::string
         */
        [[nodiscard]] auto toString() const -> std::string;
        /**
         * 배열형식으로 추가하기
         * @param value const Json_value&
         */
        auto append(const Json_value & value) -> void;
        /**
         * 복사 대입연산자
         * @param rhs const Json_value&
         * @return 자기자신의 참조값
         */
        auto operator=(const Json_value& rhs) noexcept -> Json_value&;
        /**
         * 이동 대입연산자
         * @param rhs  Json_value&&
         * @return 자기자신의 참조값
         */
        auto operator=(Json_value&& rhs) noexcept -> Json_value&;
        /**
         * 복사 대입연산자
         * @param value const Json&
         * @return 자기자신의 참조값
         */
        auto operator=(const Json& value) noexcept -> Json_value&;
        /**
         * 이동 대입연산자
         * @param value Json&&
         * @return 자기자신의 참조값
         */
        auto operator=(Json &&value) noexcept -> Json_value&;
        /**
         * Json array형식의 문자열을 Json_value의 Array형식으로 변경
         * "Json, {Json, ... }, Json, ..."
         * @param str Json_value Array포멧문자열
         * @return bool true 성공, false 실패
         */
        auto parserJsonArray(const std::string& str) -> bool;
    private:
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
        /**
         * 기본생성자
         */
        Json();
        /**
         * 이동생성자
         * @param rhs
         */
        Json(Json&& rhs);
        /**
         * 복사생성자
         * @param rhs
         */
        Json(const Json& rhs);
        /**
         * Json 하나를 구성한다. Key : Json_value
         * @param jsonKey const char*
         * @param jsonValue const Json_value&
         */
        auto append(const char* jsonKey, const Json_value& jsonValue) -> void;
        auto append(const std::string& jsonKey, const Json_value& jsonValue) -> void;
        /**
         * []연산자를 구현된 Key값으로 접근 가능한, 참조값 리턴
         * @param jsonkey const std::string&
         * @return Key에 해당되는 참조값리턴
         */
        auto operator[](const std::string &jsonkey) -> Json_value&;
        /**
         * Json -> sericalize된 문자열을 리턴함
         * @return
         */
        [[nodiscard]] auto serialize() const-> std::string;
        /**
         * Json형식의 문자열을 -> Json객체로 만듦, C스타일 리터럴문자열
         * @tparam N
         * @param value
         * @return bool true 파서성공, false 파서실패
         */
        template <int N>
        auto unserialize(char const(&value)[N]) -> bool;
        /**
         * Json형식의 문자열을 -> Json객체로 만듦
         * @param value const std::string&
         * @return bool true 파서성공, false 파서실패
         */
        auto unserialize(const std::string &value) -> bool;
        /**
         * 해당 Json 객체를 클리어한 상태로 둠
         */
        auto clear() -> void;
        /**
         * 복사 대입연산자
         * @param rhs
         * @return
         */
        auto operator=(const Json &rhs) noexcept -> Json&;
        /**
         * 이동 대입연산자
         * @param rhs
         * @return
         */
        auto operator=(Json &&rhs) noexcept -> Json&;
        /**
         * {,} 뺀, "key":"value", .... 북수개의 문자열을 Json객체로 구성함
         * @param jsonValueString
         * @return
         */
        auto parserJsonString(const std::string &jsonValueString) -> bool;
    private:
        auto copyFrom(const Json &rhs) noexcept -> void;
        std::vector< std::pair<std::string, Json_value> > mFactors;
    };
}

#endif //DOORI_API_JSON_H

#include "Json.hpp"

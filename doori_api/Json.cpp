
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
                return(mBool?"\"true\"":"\"false\"");
            case Json_value::FLOAT:
                jsonV<<"\""<<mFloat<<"\"";
                return jsonV.str();
            case Json_value::INT32S:
                jsonV<<"\""<<mInt<<"\"";
                return jsonV.str();
            case Json_value::STRING:
                jsonV<<"\"";
                for(auto&i:mStr) {
                    if (i==','||i=='"'||i==':')
                        jsonV<<"\\";
                    jsonV<<i;
                }
                jsonV<<"\"";
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
}

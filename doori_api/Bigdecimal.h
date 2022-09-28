//
// Created by jaeseong on 21. 11. 1..
//

#ifndef DOORI_PROJECT_BIGDECIMAL_H
#define DOORI_PROJECT_BIGDECIMAL_H

#include "Log.h"
#include <iostream>
#include <forward_list>
#include <list>
#include <algorithm>

namespace doori {
    /**
     * C/C++ 용 Big Decimal 클래스. 아주 큰 값을 문자열방식으로 산수처리함 
     */
    class Bigdecimal {
        public:
        /*
         * 기본 생성자 호출 금지
         */
        Bigdecimal() = delete;

        /**
         * 생성자
         * @param value
         */
        explicit Bigdecimal(const std::string& value);
        explicit Bigdecimal(std::string&& value);

        /**
         * 복사생성자, 이동생성자
         * @param rhs
         */
        Bigdecimal(const Bigdecimal& rhs);
        Bigdecimal(Bigdecimal&& rhs) noexcept ;

        /**
         * = 연산자 오버라이딩
         * @param rhs
         * @return
         */
        auto operator=(const Bigdecimal& rhs)-> Bigdecimal&;
        auto operator=(Bigdecimal&& rhs)-> Bigdecimal&;
        auto operator=(const std::string& rhs)-> Bigdecimal&;
        auto operator=(std::string&& rhs)-> Bigdecimal&;

        /**
         * * 연산자 오버라이딩
         * @param rhs
         * @return
         */
        auto operator*(const Bigdecimal& rhs)-> Bigdecimal&;

        /**
         * + 연산 오버라이딩
         * @param rhs
         * @return
         */
        auto operator+(const Bigdecimal& rhs)-> Bigdecimal&;

        /**
         * >, >= 비교연산자 오버라이딩
         * @param rhs
         * @return
         */
        auto operator>(const Bigdecimal& rhs) const -> bool;
        auto operator>=(const Bigdecimal& rhs) const -> bool;

        /**
         * - 오버라이딩
         * @param rhs
         * @return
         */
        auto operator-(const Bigdecimal& rhs)-> Bigdecimal;
        auto operator-(Bigdecimal&& rhs)-> Bigdecimal;
        auto operator-(std::string&& rhs)-> Bigdecimal;
        auto operator-(const std::string& rhs)-> Bigdecimal;

        /**
         * == 오버라이딩
         * @param rhs
         * @return
         */
        auto operator==(const Bigdecimal& rhs) const -> bool;
        auto operator==(Bigdecimal&& rhs) const -> bool;
        auto operator==(const std::string& rhs) const -> bool;
        auto operator==(std::string&& rhs) const -> bool;
        template <int N>
        auto operator==(char const(&value)[N]) const -> bool
        {
            return (m_sValue == std::string(value));
        }

        /**
         * 값을 문자열로 리턴 합니다.
         * @return std::string
         */
        auto toString() -> std::string;

        /*
         * private
         */
        private:
        auto init(const std::string& value) -> void;
        auto multiply(std::string value, char c, uint zeroCharCnt) -> std::string;
        static auto plus(std::string value1, std::string value2) -> std::string;
        auto minus(std::string value1, std::string value2, bool minusFlag) -> std::string;
//        auto divide(std::string value1, std::string value2) -> std::string;
        auto copyFrom(const Bigdecimal& rhs) noexcept ->void;
        auto copyFrom(Bigdecimal&& rhs) noexcept ->void;
        auto getFloatStyleInfo(std::string value
                               , ushort &uAboveZeroLen
                               , ushort &uZeroPos
                               , ushort &uBelowZeroLen ) noexcept -> void;
        auto compareFloatStyleBelowZero(const std::string& lhs, const std::string& rhs) noexcept -> bool;
        auto plusFloatStyleBelowZero(const string &rhs, const string &lhs) noexcept -> std::tuple<uint8_t ,string>;
        std::string m_sValue;
        bool m_bMinusFlag;
        bool m_bFloatTypeFlag;
    };
}


#endif //DOORI_PROJECT_BIGDECIMAL_H

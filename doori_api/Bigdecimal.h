//
// Created by jaeseong on 21. 11. 1..
//

#ifndef DOORI_PROJECT_BIGDECIMAL_H
#define DOORI_PROJECT_BIGDECIMAL_H

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
        Bigdecimal() = delete;

        explicit Bigdecimal(const std::string& value);
        explicit Bigdecimal(std::string&& value);

        Bigdecimal(const Bigdecimal& rhs);
        Bigdecimal(Bigdecimal&& rhs) noexcept ;

        auto operator=(const Bigdecimal& rhs)-> Bigdecimal&;
        auto operator=(Bigdecimal&& rhs)-> Bigdecimal&;
        auto operator=(const std::string& rhs)-> Bigdecimal&;
        auto operator=(std::string&& rhs)-> Bigdecimal&;

        auto operator*(const Bigdecimal& rhs)-> Bigdecimal&;

        auto operator+(const Bigdecimal& rhs)-> Bigdecimal&;

        auto operator>(const Bigdecimal& rhs) const -> bool;
        auto operator>=(const Bigdecimal& rhs) const -> bool;


        /**
         * 마이너스
         * @param rhs
         * @return
         */
        auto operator-(const Bigdecimal& rhs)-> Bigdecimal;
        auto operator-(Bigdecimal&& rhs)-> Bigdecimal;
        auto operator-(std::string&& rhs)-> Bigdecimal;
        auto operator-(const std::string& rhs)-> Bigdecimal;

        auto operator==(const Bigdecimal& rhs) const -> bool;
        auto operator==(Bigdecimal&& rhs) const -> bool;
        auto operator==(const std::string& rhs) const -> bool;
        auto operator==(std::string&& rhs) const -> bool;
        template <int N>
        auto operator==(char const(&value)[N]) const -> bool
        {
            return (coreValue == std::string(value));
        }

        /**
         * 값을 문자열로 리턴 합니다.
         * @return
         */
        auto toString() -> std::string;

        private:
        auto multiply(std::string value, char c, uint zeroCharCnt) -> std::string;
        static auto plus(std::string value1, std::string value2) -> std::string;
        auto minus(std::string value1, std::string value2, bool minusFlag) -> std::string;
//        auto divide(std::string value1, std::string value2) -> std::string;
        auto copyFrom(const Bigdecimal& rhs) noexcept ->void;
        auto copyFrom(Bigdecimal&& rhs) noexcept ->void;
        std::string coreValue;
    };
}


#endif //DOORI_PROJECT_BIGDECIMAL_H
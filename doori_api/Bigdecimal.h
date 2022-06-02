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
class Bigdecimal{
public:
    /**
     * 기본생성자 사용금지.
     */
    Bigdecimal() = delete;
    /**
     * 기본생성자.
     * @param value
     */
    Bigdecimal(const std::string& value);
    /**
     * 기본생성자.
     * @param value
     */
    Bigdecimal(std::string&& value);
    /**
     * 복사생성자
     * @param rhs
     */
    Bigdecimal(const Bigdecimal& rhs);
    /**
     * 이동생성자
     * @param rhs
     */
    Bigdecimal(Bigdecimal&& rhs);
    /**
     * 대입연산자
     * @param rhs
     * @return
     */
    auto operator=(const Bigdecimal& rhs)-> Bigdecimal&;
    /**
     * 대입연산자
     * @param rhs
     * @return
     */
    auto operator=(Bigdecimal&& rhs)-> Bigdecimal&;
    /**
     * 대입연산자
     * @param rhs
     * @return
     */
    auto operator=(const std::string& rhs)-> Bigdecimal&;
    /**
     * 대입연산자
     * @param rhs
     * @return
     */
    auto operator=(std::string&& rhs)-> Bigdecimal&;
    /**
     * 곱하기
     * @param rhs
     * @return
     */
    auto operator*(const Bigdecimal& rhs)-> Bigdecimal&;
    /**
     * 더하기
     * @param rhs
     * @return
     */
    auto operator+(const Bigdecimal& rhs)-> Bigdecimal&;
    /**
     * 마이너스
     * @param rhs
     * @return
     */
    auto operator-(const Bigdecimal& rhs)-> Bigdecimal&;
    /**
     * 마이너스
     * @param rhs
     * @return
     */
    auto operator-(Bigdecimal&& rhs)-> Bigdecimal&;
    /**
     * 마이너스
     * @param rhs
     * @return
     */
    auto operator-(std::string&& rhs)-> Bigdecimal&;
    /**
     * 마이너스
     * @param rhs
     * @return
     */
    auto operator-(const std::string& rhs)-> Bigdecimal&;
    /**
     * 같은지 비교
     * @param rhs
     * @return
     */
    auto operator==(const Bigdecimal& rhs) const -> bool;
    /**
     * 같은지 비교
     * @param rhs
     * @return
     */
    auto operator==(Bigdecimal rhs) const -> bool;
    /**
     * 같은지 비교
     * @param rhs
     * @return
     */
    auto operator==(Bigdecimal&& rhs) const -> bool;
    /**
     * 같은지 비교
     * @param rhs
     * @return
     */
    auto operator==(const std::string& rhs) const -> bool;
    /**
     * 같은지 비교
     * @param rhs
     * @return
     */
    template <int N>
    auto operator==(char const(&value)[N]) const -> bool
    {
        return (coreValue == std::string(value));
    }
    /**
     * 같은지 비교
     * @param rhs
     * @return
     */
    auto operator==(std::string&& rhs) const -> bool;
    /**
     * 값을 문자열로 리턴 합니다.
     * @return
     */
    auto toString() -> std::string;
private:
    auto muliplicated(std::string value, char c, uint zeroCharCnt) -> std::string;
    auto plus(std::string value1, std::string value2) -> std::string;
    auto minus(std::string value1, std::string value2) -> std::string;
//    auto divide(std::string value1, std::string value2) -> std::string;
    auto copyFrom(const Bigdecimal& rhs) noexcept ->void;
    auto copyFrom(Bigdecimal&& rhs) noexcept ->void;
    std::string coreValue;
};
}


#endif //DOORI_PROJECT_BIGDECIMAL_H
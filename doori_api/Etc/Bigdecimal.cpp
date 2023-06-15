//
// Created by jaeseong on 21. 11. 1..
//
#include "Bigdecimal.h"

namespace doori::api::Etc{

    Bigdecimal::Bigdecimal(const std::string &value) {
        init(value);
    }

    Bigdecimal::Bigdecimal(std::string &&value) {
        init(std::move(value));
    }

    Bigdecimal::Bigdecimal(const Bigdecimal &rhs) {
        if (this == &rhs)
            return;

        copyFrom(rhs);
    }

    Bigdecimal::Bigdecimal(Bigdecimal &&rhs) noexcept {
        if (this == &rhs)
            return;

        copyFrom(std::move(rhs));
    }

    /**
     * 곱하기를 합니다. A * B, 단, B는 1의 자리수이며, 10의 자리라고 가정하면, zeroCharCnt을 1로 셋팅
     * ex) 100 * 100 이라고 하면, multiply("100", '1', 2)
     * ex) 123 * 456 이라고 하면,
     *   multiply("123", "6", 0)
     * + multiply("123", "5", 1)
     * + multiply("123", "4", 2)
     *  와 같다.
     * @param value
     * @param c
     * @param zeroCharCnt
     * @return
     */
    auto Bigdecimal::multiply(std::string value, char c, uint zeroCharCnt) noexcept -> std::string {
        int i = value.size() - 1;
        int iValue = 0;
        int jValue = c - '0';
        int multiValue = 0;
        char upValueC = '0';
        std::forward_list<char> ret;
        std::string itos;

        for (int i = 0; i < zeroCharCnt; i++) {
            ret.push_front('0');
        }

        for (;; --i) {
            if (i < 0) {
                if (upValueC > '0')
                    ret.push_front(upValueC);
                break;
            }

            iValue = value[i] - '0';

            multiValue = iValue * jValue;

            multiValue += (upValueC - '0');

            itos = std::to_string(multiValue);
            if (itos.length() > 1) {
                upValueC = itos[0];
                ret.push_front(itos[1]);
            } else {
                upValueC = '0';
                ret.push_front(itos[0]);
            }
        }
        return std::string{ret.begin(), ret.end()};
    }

    /**
    * 부동소수점, 정수형타입 문자열을 두수를 더합니다.
    * @param rhs
    * @return Bigdecimal
    */
    auto Bigdecimal::operator+(const Bigdecimal &rhs) -> Bigdecimal {
        if (this == &rhs)
            return {*this + *this};

        auto belowZeroLen1 = getDecimalLength(this->m_String);
        auto belowZeroLen2 = getDecimalLength(rhs.m_String);
        auto v = correctAsIntegerString(this->m_String, rhs.m_String);
        auto belowZeroLen = belowZeroLen1 > belowZeroLen2 ? belowZeroLen1 : belowZeroLen2;

        Bigdecimal tLeft{"0"};
        Bigdecimal tRight{"0"};
        string r;
        switch (compairSign(this->m_IsNegative, rhs.m_IsNegative)) {
            case SIGN::BOTH_MINUS:
                r = makeUpPoint(plus(v.first, v.second), belowZeroLen);
                r = "-" + r;
                break;
                // 예를 들어, (-100) + (90) = 90 - 100 같으므로, 빼기로 보정
            case SIGN::ONLY_FRIST_MINUS:
                tLeft = *this;
                tRight = rhs;
                tLeft.m_IsNegative = false;
                return {tRight - tLeft};
                // 예를 들어, 100 + (-90) = 100 - 90 같으므로, 빼기로 보정
            case SIGN::ONLY_SECOND_MINUS:
                tLeft = *this;
                tRight = rhs;
                tRight.m_IsNegative = false;
                return {tLeft - tRight};
            case SIGN::BOTH_PLUS:
                r = makeUpPoint(plus(v.first, v.second), belowZeroLen);
                break;
        }
        if(r.empty())
            r = "0";
        return Bigdecimal{r};
    }

    auto Bigdecimal::operator*(const Bigdecimal &rhs) -> Bigdecimal {
        if (rhs == "0" || this->toString() == "0")
            return Bigdecimal{"0"};

        auto belowZeroLen1 = getDecimalLength(this->m_String);
        auto belowZeroLen2 = getDecimalLength(rhs.m_String);
        auto v = correctAsIntegerString(this->m_String, rhs.m_String);

        // 0이면 한쪽은 float타입이 아님.
        // correctAsIntegerString 함수에서 보정해서 두 수의 양의 문자열을 리턴함
        uint corrctZeroLen = belowZeroLen1 > belowZeroLen2 ? belowZeroLen1 : belowZeroLen2;
        corrctZeroLen *= 2;

        auto sum = Bigdecimal("0");

        if (!ge(v.first, v.second))
            swap(v.first, v.second);

        if (ge(v.first, v.second)) {
            for (int i = v.second.length() - 1, zeroSpace = 0; i > -1; i--, zeroSpace++) {
                auto value = multiply(v.first, v.second[i], zeroSpace);

                sum = Bigdecimal(sum) + Bigdecimal(value);
            }
        }
        auto corrctValue = makeUpPoint(sum.m_String, corrctZeroLen);

        auto sign = IsNegative(this->m_IsNegative, rhs.m_IsNegative) ? "-" : "";

        return Bigdecimal{sign + removeSuffixZero(corrctValue)};
    }


    /**
    * 양의 수 문자열을 입력 받는다. 양의 수 문자열의 합을 리턴한다.
    * @param value1 양의 수 문자열
    * @param value2 양의 수 문자열
    * @return
    */
    auto Bigdecimal::plus(std::string value1, std::string value2) noexcept -> std::string {
        std::forward_list<char> ret;
        int i = static_cast<int>(value1.size() - 1);
        int j = static_cast<int>(value2.size() - 1);

        std::string itos;
        int sum, iValue, jValue, upValue;
        upValue = 0;
        for (;; --i, --j) {
            if (i < 0 && j < 0) {
                if (upValue > 0)
                    ret.push_front('1');
                break;
            }

            if (i < 0) iValue = 0;
            else iValue = value1[i] - '0';

            if (j < 0) jValue = 0;
            else jValue = value2[j] - '0';

            if (upValue > 0)
                iValue += 1;

            itos = std::to_string(iValue + jValue);
            if (itos.length() > 1) {
                upValue = 1;
                ret.push_front(itos[1]);
            } else {
                upValue = 0;
                ret.push_front(itos[0]);
            }
            iValue = jValue = 0;
        }
        return std::string{ret.begin(), ret.end()};
    }

    auto Bigdecimal::toString() -> std::string {
        return removeSuffixZero(m_IsNegative ? ("-" + m_String) : m_String);
    }

    auto Bigdecimal::copyFrom(Bigdecimal &&rhs) noexcept -> void {
        m_IsNegative = rhs.m_IsNegative;
        m_IsFloatType = rhs.m_IsFloatType;
        m_String = std::move(rhs.m_String);
    }

    auto Bigdecimal::copyFrom(const Bigdecimal &rhs) noexcept -> void {
        m_IsNegative = rhs.m_IsNegative;
        m_IsFloatType = rhs.m_IsFloatType;
        m_String = rhs.m_String;
    }

    /**
    * @note value1값이 항상 value2보다 커야 한다.
    * @param value1 : 양의 정수 문자열
    * @param value2 : 양의 정수 문자열
    * @return
    */
    auto Bigdecimal::minus(string value1, string value2) noexcept -> string {
        std::forward_list<char> ret;

        int i = static_cast<int>(value1.length());
        int j = static_cast<int>(value2.length());
        //배열은 0부터 시작
        i--;
        j--;

        int r, v1, v2;
        for (; i > -1 || j > -1; --i, --j) {
            v1 = value1[i] - '0';
            if (j > -1)
                v2 = value2[j] - '0';
            else
                v2 = 0;

            r = v1 - v2;

            if (r < 0) {
                r = 10 + r;
                value1[i - 1]--;
                ret.push_front('0' + r);
            } else {
                ret.push_front('0' + r);
            }
        }

        return std::string{ret.begin(), ret.end()};
    }


    auto Bigdecimal::operator=(const Bigdecimal &rhs) -> Bigdecimal & {
        if (this == &rhs)
            return *this;

        copyFrom(rhs);
        return *this;
    }

    auto Bigdecimal::operator=(Bigdecimal &&rhs) -> Bigdecimal & {
        if (this == &rhs)
            return *this;

        copyFrom(rhs);
        return *this;
    }

    auto Bigdecimal::operator=(const std::string &rhs) -> Bigdecimal & {
        init(rhs);
        return *this;
    }

    auto Bigdecimal::operator=(std::string &&rhs) -> Bigdecimal & {
        init(rhs);
        return *this;
    }

    auto Bigdecimal::operator==(const std::string &rhs) const -> bool {
        return (*this == Bigdecimal{rhs});
    }

    auto Bigdecimal::operator==(std::string &&rhs) const -> bool {
        return (*this == Bigdecimal{rhs});
    }

    auto Bigdecimal::operator==(const Bigdecimal &rhs) const -> bool {
        if (this == &rhs)
            return true;

        return (this->m_IsNegative == rhs.m_IsNegative
                && this->m_IsFloatType == rhs.m_IsFloatType
                && this->m_String == rhs.m_String);
    }

    auto Bigdecimal::operator==(Bigdecimal &&rhs) const -> bool {
        if (this == &rhs)
            return true;

        return (this->m_IsNegative == rhs.m_IsNegative
                && this->m_IsFloatType == rhs.m_IsFloatType
                && this->m_String == rhs.m_String);
    }

    auto Bigdecimal::operator-(std::string &&rhs) -> Bigdecimal {
        return *this - Bigdecimal{std::move(rhs)};
    }

    auto Bigdecimal::operator-(const std::string &rhs) -> Bigdecimal {
        return *this - Bigdecimal{std::move(rhs)};
    }

    auto Bigdecimal::operator-(const Bigdecimal &rhs) -> Bigdecimal {
        //자기자신을 빼면 0을 리턴
        if (this == &rhs)
            return Bigdecimal{"0"};

        auto belowZeroLen1 = getDecimalLength(this->m_String);
        auto belowZeroLen2 = getDecimalLength(rhs.m_String);
        auto v = correctAsIntegerString(this->m_String, rhs.m_String);
        auto belowZeroLen = belowZeroLen1 > belowZeroLen2 ? belowZeroLen1 : belowZeroLen2;
        //둘다 마이너스

        Bigdecimal tLeft{"0"};
        Bigdecimal tRight{"0"};
        string result, r;
        switch (compairSign(this->m_IsNegative, rhs.m_IsNegative)) {
            case SIGN::BOTH_MINUS:
                if (ge(v.first, v.second)) {
                    r = makeUpPoint(minus(v.first, v.second), belowZeroLen);
                    result = "-" + r;
                } else if (eq(v.first, v.second))
                    result = "0";
                else
                    result = makeUpPoint(minus(v.second, v.first), belowZeroLen);
                break;
            case SIGN::ONLY_FRIST_MINUS:
                r = makeUpPoint(plus(this->m_String, rhs.m_String), belowZeroLen);
                result = "-" + r;
                break;
            case SIGN::ONLY_SECOND_MINUS:
                result = makeUpPoint(plus(this->m_String, rhs.m_String), belowZeroLen);
                break;
            case SIGN::BOTH_PLUS:
                if (gt(v.first, v.second))
                    result = makeUpPoint(minus(v.first, v.second), belowZeroLen);
                else if (eq(v.first, v.second))
                    result = "0";
                else {
                    r = makeUpPoint(minus(v.second, v.first), belowZeroLen);
                    result = "-" + r;
                }
                break;
        }
        if(result.empty())
            result = "0";
        return Bigdecimal{result};
    }

    auto Bigdecimal::operator>(const Bigdecimal &rhs) const -> bool {
        if (this == &rhs)
            return false;

        auto v = correctAsIntegerString(this->m_String, rhs.m_String);

        bool result;

        switch (compairSign(this->m_IsNegative, rhs.m_IsNegative)) {
            case SIGN::BOTH_MINUS:
                if (gt(v.first, v.second))
                    result = false;
                else
                    result = true;
                break;
            case SIGN::ONLY_FRIST_MINUS:
                result = false;
                break;
            case SIGN::ONLY_SECOND_MINUS:
                result = true;
                break;
            case SIGN::BOTH_PLUS:
                if (ge(v.first, v.second))
                    result = true;
                else
                    result = false;
                break;
        }
        return result;
    }

    auto Bigdecimal::operator>=(const Bigdecimal &rhs) const -> bool {
        if (this == &rhs)
            return true;

        auto v = correctAsIntegerString(this->m_String, rhs.m_String);

        bool result;

        switch (compairSign(this->m_IsNegative, rhs.m_IsNegative)) {
            case SIGN::BOTH_MINUS:
                if (gt(v.first, v.second))
                    result = false;
                else
                    result = true;
                break;
            case SIGN::ONLY_FRIST_MINUS:
                result = false;
                break;
            case SIGN::ONLY_SECOND_MINUS:
                result = true;
                break;
            case SIGN::BOTH_PLUS:
                if (ge(v.first, v.second))
                    result = true;
                else
                    result = false;
                break;
        }
        return result;
    }

    /**
    * Float 형식의 문자열  xxxxx.bbbbb
    * @param value : 입력받은 문자열
    * @param uAboveZero : 소수점 앞 xxxxx 정수값 길이
    * @param uZero : 소소점 위치값
    * @param uBelowZero : 소수점 아래의 bbbbb 소수점 길이값
    */
    auto Bigdecimal::getDecimalMetaInfo(const std::string &value, uint &uAboveZeroLen, uint &uZeroPos,
                                        uint &uBelowZeroLen) noexcept -> void {
        uAboveZeroLen = 0;
        uZeroPos = 0;
        uBelowZeroLen = value.length();
        for (int i = 0; i < value.length(); ++i) {
            if (value[i] == '.') {
                uZeroPos = i;
                break;
            }
        }

        if (uZeroPos == 0) {
            uAboveZeroLen = value.size() + 1;
            uBelowZeroLen = 0;
        } else {
            uAboveZeroLen = uZeroPos;
            uBelowZeroLen = value.size() - (uZeroPos + 1);
        }
    }

    auto Bigdecimal::init(const std::string &value) -> void {
        int startPos = -1;
        int prefixNumPos = -1;
        int minusSignPos = -1;
        int plusSignPos = -1;
        int spaceSignPos = -1;
        int pointSignPos = -1;
        bool minusSignOnce = false;
        bool plusSignOnce = false;
        bool pointSignOnce = false;

        // 정합성 체크
        for (int i = 0; i < value.size(); i++) {
            if ( '0' <= value[i] && value[i] <= '9') {
                if(startPos==-1)
                    startPos = i;
                prefixNumPos = i;
                if(minusSignOnce && prefixNumPos < minusSignPos && prefixNumPos != -1 && minusSignPos !=-1)
                    throw runtime_error("bad number:" + value);
                if(plusSignOnce && prefixNumPos < plusSignPos && prefixNumPos != -1 && plusSignPos !=-1)
                    throw runtime_error("bad number:" + value);
            }
            else if(value[i] == '-') {
                if(startPos==-1)
                    startPos = i;
                minusSignPos = i;
                if (!minusSignOnce)
                    minusSignOnce = true;
                else
                    throw runtime_error("bad number:" + value);
                if( minusSignPos > prefixNumPos && minusSignPos != -1 && prefixNumPos != -1)
                    throw runtime_error("bad number:" + value);
            }
            else if(value[i] == '+') {
                if(startPos==-1)
                    startPos = i;
                plusSignPos = i;
                if (!plusSignOnce)
                    plusSignOnce = true;
                else
                    throw runtime_error("bad number:" + value);
                if( plusSignPos > prefixNumPos && plusSignPos != -1 && prefixNumPos != -1)
                    throw runtime_error("bad number:" + value);
            }
            else if(value[i] == ' ') {
                spaceSignPos = i;
                if( spaceSignPos > minusSignPos && spaceSignPos != -1 && minusSignPos != -1)
                    throw runtime_error("bad number:" + value);
                if( spaceSignPos > plusSignPos && spaceSignPos != -1 && plusSignPos != -1)
                    throw runtime_error("bad number:" + value);
                if( spaceSignPos > prefixNumPos && spaceSignPos != -1 && prefixNumPos != -1 )
                    throw runtime_error("bad number:" + value);
            }
            else if(value[i] == '.') {
                pointSignPos = i;
                if(!pointSignOnce)
                    pointSignOnce = true;
                else
                    throw runtime_error("bad number:" + value);
                if( pointSignPos < minusSignPos && pointSignPos != -1 && minusSignPos != -1)
                    throw runtime_error("bad number:" + value);
                if( pointSignPos < plusSignPos && pointSignPos != -1 && plusSignPos != -1)
                    throw runtime_error("bad number:" + value);
            }
            else{
                throw runtime_error("bad number:" + value);
            }
        }

        //앞 부분 기호(+, -) 판단
        if (value[startPos] == '-') {
            m_IsNegative = true;
            startPos++;
        } else if (value[startPos] == '+') {
            m_IsNegative = false;
            startPos++;
        } else {
            m_IsNegative = false;
        }

        //마이너스기호가 있으면, 뒤에서부터 복사
        auto sTmpValue = value.substr(startPos, value.size() - startPos);

        // 0.0000 => 0
        // -0 => 0
        // +0 => 0
        // 00.00000 => 0
        // 00.00001 => 0.00001
        bool bZero = true;
        m_IsFloatType = false;
        int nZeroRepeatPos = 0;
        for ( int i = 0; i < sTmpValue.size(); ++i) {
            if (sTmpValue[i] == '0' && !m_IsFloatType)
                nZeroRepeatPos++;
            if (sTmpValue[i] == '.') {
                nZeroRepeatPos--;
                m_IsFloatType = true;
                continue;
            }
            if (sTmpValue[i] != '0') {
                bZero = false;
                break;
            }
        }

        if (bZero) {
            m_String = "0";
            m_IsNegative = false;
            m_IsFloatType = false;
        } else {
            if (nZeroRepeatPos > 0) {
                m_String = sTmpValue.substr(nZeroRepeatPos, sTmpValue.size() - nZeroRepeatPos);
            } else {
                m_String = sTmpValue;
            }
        }
    }

    /**
    * v1 >= v2보다 크면 true 그외 false
    * @param v1 양의 수
    * @param v2 양의 수
    * @return
    */
    auto Bigdecimal::ge(std::string v1, std::string v2) noexcept -> bool {
        bool bStatus = true;

        auto Len1 = v1.length();
        auto Len2 = v2.length();

        if (Len1 > Len2)
            return true;
        else if (Len1 < Len2)
            return false;
            // 두 숫자의 길이가 같으면, scan
        else {
            for (int i = 0; i < Len1; ++i) {
                if (v1[i] > v2[i])
                    return true;
                else if (v1[i] == v2[i]);
                else
                    return false;
            }
        }
        return bStatus;
    }

    /**
    * v1 == v2보다 크면 true 그외 false
    * @param v1 양의 수
    * @param v2 양의 수
    * @return
    */
    auto Bigdecimal::eq(std::string v1, std::string v2) noexcept -> bool {
        bool bStatus = true;

        reverse(v1.begin(), v1.end());
        reverse(v2.begin(), v2.end());

        auto Len1 = v1.length();
        auto Len2 = v2.length();

        if (Len1 != Len2)
            return false;
        else {
            for (int i = 0; i < Len1; ++i) {
                if (v1[i] == v2[i])
                    continue;
                else
                    return false;
            }
        }
        return true;
    }

    /**
    * v1 > v2 이면 true 그외 false
    * @param v1 양의 수
    * @param v2 양의 수
    * @return
    */
    auto Bigdecimal::gt(std::string v1, std::string v2) noexcept -> bool {
        if (ge(v1, v2)) {
            if (eq(v1, v2))
                return false;
            else
                return true;
        }
        return false;
    }


    /**
    * float string 두개를 소수점 길이에 대해서 보전한다.
    * 1.234, 12.34 이면  1234, 12340
    * 111.002 111.00002 이면 1110020, 1110002
    * 111.01, 0.001 이면 111010, 1
    * @note
    * @param v1 양의 소수점 문자열, 또는 양의 수 문자열
    * @param v2 양의 소수점 문자열, 또는 양의 수 문자열
    * @return
    */
    auto Bigdecimal::correctAsIntegerString(const string &v1,
                                            const string &v2) noexcept -> std::pair<std::string, std::string> {
        uint v1AboveLen = 0;
        uint v1PointPos = 0;
        uint v1BelowLen = 0;

        uint v2AboveLen = 0;
        uint v2PointPos = 0;
        uint v2BelowLen = 0;

        getDecimalMetaInfo(v1, v1AboveLen, v1PointPos, v1BelowLen);
        getDecimalMetaInfo(v2, v2AboveLen, v2PointPos, v2BelowLen);

        uint AbsolutedMultiplyLen1 = 0;
        uint AbsolutedMultiplyLen2 = 0;

        AbsolutedMultiplyLen1 = v1BelowLen >= v2BelowLen ? 0 : v2BelowLen - v1BelowLen;
        AbsolutedMultiplyLen2 = v2BelowLen >= v1BelowLen ? 0 : v1BelowLen - v2BelowLen;

        string v1Temp;
        string v2Temp;

        v1Temp = v1.substr(0, v1AboveLen) + v1.substr(v1PointPos + 1, v1BelowLen);
        for (int i = 0; i < AbsolutedMultiplyLen1; ++i)
            v1Temp += '0';

        v2Temp = v2.substr(0, v2AboveLen) + v2.substr(v2PointPos + 1, v2BelowLen);
        for (int i = 0; i < AbsolutedMultiplyLen2; ++i)
            v2Temp += '0';

        return make_pair(removePrefixZero(v1Temp), removePrefixZero(v2Temp));
    }

    /**
    * float type, >= true 그외 false
    * @param v1 양의 수
    * @param v2 양의 수
    * @return
    */
    auto Bigdecimal::fge(std::string v1, std::string v2) const noexcept -> bool {
        auto vPair = correctAsIntegerString(v1, v2);
        return ge(vPair.first, vPair.second);
    }

    /**
    * float type, == true 그외 false
    * @param v1 양의 수
    * @param v2 양의 수
    * @return
    */
    auto Bigdecimal::feq(std::string v1, std::string v2) const noexcept -> bool {
        auto vPair = correctAsIntegerString(v1, v2);
        return eq(vPair.first, vPair.second);
    }

    /**
    * float type, > true 그외 false
    * @param v1 양의 수
    * @param v2 양의 수
    * @return
    */
    auto Bigdecimal::fgt(std::string v1, std::string v2) const noexcept -> bool {
        auto vPair = correctAsIntegerString(v1, v2);
        return gt(vPair.first, vPair.second);
    }

    /**
    * 000010 -> 10
    * @param v1
    * @return
    */
    auto Bigdecimal::removePrefixZero(const string &v1) noexcept -> std::string {
        uint startPos = 0;
        for (int i = 0; i < v1.length(); ++i) {
            if (v1[i] == '0')
                startPos++;
            else
                break;
        }
        return v1.substr(startPos, v1.length() - startPos);
    }

    /**
    * 소수부의 길이를 리턴함
    * @param value
    * @return
    * @example 1234.567 -> 소주점은 567이므로 길이 3값을 리턴함
    */
    auto Bigdecimal::getDecimalLength(const string &value) noexcept -> uint {
        uint uZeroPos = 0;
        uint uBelowZeroLen = value.length();
        for (int i = 0; i < value.length(); ++i) {
            if (value[i] == '.') {
                uZeroPos = i;
                break;
            }
        }

        if (uZeroPos == 0) {
            uBelowZeroLen = 0;
        } else {
            uBelowZeroLen = value.size() - (uZeroPos + 1);
        }
        return uBelowZeroLen;
    }

    /**
    * v1 문자열에 소수점 길에 문자열 "." :at 넣는다.
    * @param v1
    * @param belowZeroLen : 소수점 길이값
    * @return
    */
    auto Bigdecimal::makeUpPoint(const string &v1, uint belowZeroLen) -> std::string {
        string r = v1;
        if (belowZeroLen > 0) {
            int v = r.length() - belowZeroLen;
            if (v < 0) {
                r.insert(0, abs(v), '0');
            }
            r.insert(r.length() - belowZeroLen, ".");
            if (r[0] == '.')
                r.insert(0, "0");
        }
        return r;
    }

    /**
    * 소수점자리의 있는 0를 삭제 함
    * 0.10000 -> 0.1
    * 1000 -> 1000
    * @param v1
    * @return
    */
    auto Bigdecimal::removeSuffixZero(const string &v1) noexcept -> std::string {
        uint count = 0;
        bool stop = false;
        bool can = false;
        for (int i = v1.length() - 1; i > -1; --i) {
            if (v1[i] == '0' && !stop)
                count++;
            else {
                if (v1[i] == '.') {
                    can = true;
                    break;
                }
                stop = true;
            }
        }
        if (can) {
            if (v1[v1.length() - (count + 1)] == '.')
                return v1.substr(0, v1.length() - (count + 1));
            return v1.substr(0, v1.length() - count);
        }

        return v1;
    }

    auto Bigdecimal::operator/(const Bigdecimal &rhs) -> Bigdecimal {
        if (rhs == "0")
            throw runtime_error("Attempted to findTheRest by zero\n");

        if (this->toString() == "0")
            return Bigdecimal{"0"};

        if (*this == rhs)
            return Bigdecimal{"1"};

        auto vPair = correctAsIntegerString(this->m_String, rhs.m_String);

        int nDecimalPointCount = 0;
        std::tuple<string, string> vRet{"0", "0"};

        string quotient;
        string remainder;
        bool OnComma = false;

        int zeroCharCount = 0;
        for (int i = 1;; ++i) {
            if (gt(vPair.first, vPair.second)) {
                zeroCharCount = 0;
                vRet = findTheRest(vPair.first, vPair.second);
                quotient += get<0>(vRet);

                if (get<1>(vRet) == "0") //나머지가 0이면, 완벽하게 나누어짐
                    break;

                vPair.first = get<1>(vRet);

            } else if (eq(vPair.first, vPair.second)) {
                quotient += "1"; //같으면 몫은 1
                break;
            } else {
                if (i == 1) {
                    quotient += "0.";
                    OnComma = true;
                } else if (!OnComma) {
                    quotient += ".";
                    OnComma = true;
                }

                if (OnComma) {
                    nDecimalPointCount++;
                    if (nDecimalPointCount > MAX_DECIMAL_POINT)
                        break;
                }

                vPair.first += "0";

                if (zeroCharCount++ > 0)
                    quotient += "0";
            }
        }

        string result;
        switch (compairSign(this->m_IsNegative, rhs.m_IsNegative)) {
            case SIGN::BOTH_MINUS:
            case SIGN::BOTH_PLUS:
                result = quotient;
                break;
            case SIGN::ONLY_FRIST_MINUS:
            case SIGN::ONLY_SECOND_MINUS:
                result = "-" + quotient;
                break;
        }
        if(result.empty())
            result = "0";
        return Bigdecimal{result};
    }

    /**
    * V1 / V2. 몫과 나머지를 구한다.
    * @note value1값이 항상 value2보다 커야 한다.
    * @param value1 : 양의 정수 문자열
    * @param value2 : 양의 정수 문자열
    * @return tuple<string, string>{quotient, remainder}
    */
    auto Bigdecimal::findTheRest(std::string v1, std::string v2) noexcept -> std::tuple<std::string, std::string> {
        auto lenV1 = v1.length();
        auto lenV2 = v2.length();

        string minusV;

        string subV1;
        string r2;
        string quotient;
        string remainder{"0"};

        size_t startP, limitLen;
        auto const copyPossibleLen = lenV2;

        int i = 0;
        for (i = 0; i < v1.length(); ++i) {
            subV1.append(1, v1[i]);

            if (ge(subV1, v2)) {
                auto r = findMaxLimit(subV1, v2);
                quotient.append(to_string(get<0>(r)));

                minusV = minus(subV1, get<1>(r));
                minusV = removePrefixZero(minusV);
                remainder = minusV;
                //나머지기 0이면
                if (remainder.empty()) {
                    remainder = "0";
                }
                subV1 = minusV;
            } else {
                quotient.append(1, '0');
            }
        }

        quotient = removePrefixZero(quotient);

        return {quotient, remainder};
    }

    /**
    * @note v1값이 v2값보다 항상 커야 한다. 성능을 위해서, v1길이는 v2보다 최대 한자리 더 크다
    * v2 1...9까지 곱하기를 해서, v1를 넘지 않는 값을 리턴
    * @param value1 : 양의 정수 문자열
    * @param value2 : 양의 정수 문자열
    * @return 넘지않는 최대 곱하기 수, 최대곱하기 수의 결과값.
    */
    auto Bigdecimal::findMaxLimit(const string &v1, const string &v2) noexcept -> tuple<int, std::string> {
        if (v1.length() > v2.length() + 1)
            abort();

        string b, a;
        int prevB;
        for (int i = 1; i <= 10; i++) {
            a = multiply(v2, '0' + i, 0);
            if (gt(a, v1))
                return {prevB, b};

            b = a;
            prevB = i;
        }
        return {0, ""};
    }

    auto Bigdecimal::IsNegative(bool minusFlag1, bool minusFlag2) noexcept -> bool {
        if (minusFlag1 != minusFlag2)
            return true;
        else
            return false;
    }

    /**
    * 부호비교
    * @param minusFlag1
    * @param minusFlag2
    * @return 부호가 같으면 0, +,- 이면 1,  -,+이면 2
    */
    auto Bigdecimal::compairSign(bool minusFlag1, bool minusFlag2) noexcept -> SIGN {
        if (minusFlag1 && minusFlag2)
            return SIGN::BOTH_MINUS;
        else if (minusFlag1 && !minusFlag2)
            return SIGN::ONLY_FRIST_MINUS;
        else if (!minusFlag1 && minusFlag2)
            return SIGN::ONLY_SECOND_MINUS;
        else
            return SIGN::BOTH_PLUS;
    }

    auto Bigdecimal::operator*=(const Bigdecimal &rhs) -> Bigdecimal {
        *this = *this * rhs;
        return Bigdecimal{*this};
    }

    auto Bigdecimal::operator+=(const Bigdecimal &rhs) -> Bigdecimal {
        *this = *this + rhs;
        return Bigdecimal{*this};
    }

    auto Bigdecimal::operator<(const Bigdecimal &rhs) const -> bool {
        return !(*this >= rhs);
    }

    auto Bigdecimal::operator<=(const Bigdecimal &rhs) const -> bool {
        return !(*this > rhs);
    }

    auto Bigdecimal::operator-=(const Bigdecimal &rhs) -> Bigdecimal {
        *this = *this - rhs;
        return Bigdecimal{*this};
    }

    auto Bigdecimal::operator/=(const Bigdecimal &rhs) -> Bigdecimal {
        *this = *this / rhs;
        return Bigdecimal{*this};
    }

}//doori end

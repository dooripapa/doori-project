//
// Created by jaeseong on 21. 11. 1..
//
#include "Bigdecimal.h"

namespace doori {

    Bigdecimal::Bigdecimal(const std::string& value) {
        init(value);
    }

    Bigdecimal::Bigdecimal(std::string&& value) {
        init(std::move(value));
    }

    Bigdecimal::Bigdecimal(const Bigdecimal &rhs) {
        if(this==&rhs)
            return;

        copyFrom(rhs);
    }

    Bigdecimal::Bigdecimal(Bigdecimal &&rhs) noexcept {
        if(this==&rhs)
            return;

        copyFrom(std::move(rhs));
    }

    auto Bigdecimal::multiply(std::string value, char c, uint zeroCharCnt) -> std::string {
        int i=value.size()-1;
        int iValue = 0;
        int jValue = c-'0';
        int multiValue = 0;
        char upValueC='0';
        std::forward_list<char> ret;
        std::string itos;

        for(int i=0;i<zeroCharCnt;i++) {
            ret.push_front('0');
        }

        for(;;--i) {
            if(i<0) {
                if(upValueC>'0')
                    ret.push_front(upValueC);
                break;
            }

            iValue = value[i]-'0';

            multiValue = iValue*jValue;

            multiValue+=(upValueC-'0');

            itos = std::to_string(multiValue);
            if(itos.length()>1){
                upValueC = itos[0];
                ret.push_front(itos[1]);
            }
            else {
                upValueC = '0';
                ret.push_front(itos[0]);
            }
        }
        return std::string(ret.begin(), ret.end());
    }

    /**
     * 부동소수점, 정수형타입 문자열을 두수를 더합니다.
     * @param rhs
     * @return Bigdecimal
     */
    auto Bigdecimal::operator+(const Bigdecimal &rhs) -> Bigdecimal& {
        ushort uLhsAboveZeroLen = 0;
        ushort uLhsZeroPos = 0;
        ushort uLhsBelowZeroLen = 0;
        getFloatStyleInfo(this->m_sValue, uLhsAboveZeroLen, uLhsZeroPos, uLhsBelowZeroLen );

        ushort uRhsAboveZeroLen = 0;
        ushort uRhsZeroPos = 0;
        ushort uRhsBelowZeroLen = 0;
        getFloatStyleInfo(this->m_sValue, uRhsAboveZeroLen, uRhsZeroPos, uRhsBelowZeroLen );
        return *this;
    }

    auto Bigdecimal::operator*(const Bigdecimal &rhs) -> Bigdecimal & {
        std::string sI;
        std::string sBefore;
        std::string sSum="1";
        std::string sSumRet="0";
        std::string sRet="1";

        int zeroCharCnt=0;
        std::list<std::string> stringList;

        for_each(rhs.m_sValue.begin(), rhs.m_sValue.end(), [&](int i){
            auto el = std::to_string(i);
            for(int m=el.length()-1 ;m>=0;m--, zeroCharCnt++) {
                stringList.push_back(multiply(sRet, el[m], zeroCharCnt));
            }
            zeroCharCnt=0;

            for(auto ori:stringList) {
                sSumRet = plus(sSumRet, ori);
            }
            stringList.clear();

            sRet = sSumRet;
            sSumRet = "0";
        });
        return *this;
    }


    auto Bigdecimal::plus(std::string value1, std::string value2) -> std::string {
        std::forward_list<char> ret;
        int i=static_cast<int>(value1.size()-1);
        int j=static_cast<int>(value2.size()-1);

        std::string itos;
        int sum, iValue, jValue, upValue;
        upValue=0;
        for(;; --i, --j) {
            if(i<0 && j<0) {
                if(upValue>0)
                    ret.push_front('1');
                break;
            }

            if(i<0) iValue = 0;
            else iValue = value1[i]-'0';

            if(j<0) jValue = 0;
            else jValue = value2[j]-'0';

            if(upValue>0)
                iValue+=1;

            itos = std::to_string(iValue + jValue);
            if( itos.length() > 1) {
                upValue=1;
                ret.push_front(itos[1]);
            }
            else {
                upValue=0;
                ret.push_front(itos[0]);
            }
            iValue=jValue=0;
        }
        return std::string{ret.begin(),ret.end()};
    }

    auto Bigdecimal::toString() -> std::string {
        return (m_bMinusFlag?("-"+m_sValue):m_sValue);
    }

    auto Bigdecimal::copyFrom(Bigdecimal &&rhs) noexcept -> void {
        m_bMinusFlag = rhs.m_bMinusFlag;
        m_bFloatTypeFlag = rhs.m_bFloatTypeFlag;
        m_sValue = std::move(rhs.m_sValue) ;
    }
    auto Bigdecimal::copyFrom(const Bigdecimal &rhs) noexcept -> void {
        m_bMinusFlag = rhs.m_bMinusFlag;
        m_bFloatTypeFlag = rhs.m_bFloatTypeFlag;
        m_sValue = rhs.m_sValue;
    }

    /// 숫자 형식의 문자열의 값을 마이너스를 한다.
    /// \param value1
    /// \param value2
    /// \param minusFlag  value1 > value2 이면, false, 반대면, true
    /// \return

    auto Bigdecimal::minus(std::string value1, std::string value2, bool minusFlag) -> std::string {
        LOG(DEBUG, "value1[", value1, "]", " value2[", value2, "]", " Minus Flag", minusFlag );
        std::forward_list<char> ret;

        if ( minusFlag ) {
            std::swap(value1, value2);
        }

        int i=static_cast<int>(value1.size()-1);
        int j=static_cast<int>(value2.size()-1);

        std::string itos;
        int sum, iValue, jValue;
        for(;; --i, --j) {
            if(i<0 && j<0) {
                if(minusFlag)
                    ret.push_front('-');
                break;
            }

            if(i<0) iValue = 0;
            else iValue = value1[i]-'0';

            if(j<0) jValue = 0;
            else jValue = value2[j]-'0';

            sum = iValue - jValue;
            if( sum < 0) {
                if( i-1 < 0 )
                    ;
                else
                {
                    value1[i-1]--;
                    sum = 10 + iValue - jValue;
                }
            }
            sum = abs(sum);
            itos = std::to_string(sum);
            if( itos[0] == '0' )
                ;
            else
                ret.push_front(itos[0]);
        }
        return std::string{ret.begin(),ret.end()};
    }

    auto Bigdecimal::operator==(const Bigdecimal &rhs) const -> bool {
        if ( this == &rhs )
            return true;
        return (this->m_sValue == rhs.m_sValue);
    }

    auto Bigdecimal::operator==(Bigdecimal &&rhs) const -> bool {
        if ( this == &rhs )
            return true;

        return (this->m_sValue == rhs.m_sValue);
    }

    auto Bigdecimal::operator=(const Bigdecimal &rhs) -> Bigdecimal & {
        if ( this == &rhs )
            return *this;

        copyFrom(rhs);
        return *this;
    }

    auto Bigdecimal::operator=(Bigdecimal &&rhs) -> Bigdecimal & {
        if ( this == &rhs )
            return *this;

        copyFrom(rhs);
        return *this;
    }

    auto Bigdecimal::operator==(const std::string &rhs) const -> bool {
        return (this->m_sValue == rhs);
    }

    auto Bigdecimal::operator==(std::string &&rhs) const -> bool {
        return (this->m_sValue == rhs);
    }

    auto Bigdecimal::operator=(const std::string &rhs) -> Bigdecimal & {
        this->m_sValue = rhs;
        return *this;
    }

    auto Bigdecimal::operator=(std::string &&rhs) -> Bigdecimal & {
        this->m_sValue = rhs;
        return *this;
    }

    auto Bigdecimal::operator-(Bigdecimal &&rhs) -> Bigdecimal {
        LOG(DEBUG, "this->m_sValue:", this->m_sValue, ",rhs.m_sValue:", rhs.m_sValue);
        std::string ret = minus(this->m_sValue, rhs.m_sValue, !(*this > rhs));
        return Bigdecimal{ret};
    }

    auto Bigdecimal::operator-(std::string &&rhs) -> Bigdecimal {
        std::string ret = minus(this->m_sValue, rhs, !(*this > Bigdecimal{rhs}) );
        LOG(DEBUG, "[", ret, "]");
        return Bigdecimal{ret};
    }

    auto Bigdecimal::operator-(const std::string &rhs) -> Bigdecimal {
        std::string ret = minus(this->m_sValue, rhs, !(*this > Bigdecimal{rhs}));
        LOG(DEBUG, "[", ret, "]");
        return Bigdecimal{ret};
    }

    auto Bigdecimal::operator-(const Bigdecimal &rhs) -> Bigdecimal {
        if( this == &rhs)
            return *this;
        std::string ret = minus(this->m_sValue, rhs.m_sValue, !(*this > rhs) );
        LOG(DEBUG, "[", ret, "]");
        return Bigdecimal{ret};
    }

    auto Bigdecimal::operator>(const Bigdecimal &rhs) const -> bool {
        if( this == &rhs)
            return false;

        //음수 > 양수
        if(this->m_bMinusFlag && !rhs.m_bMinusFlag)
            return false;
        //양수 > 음수
        else if(!this->m_bMinusFlag && rhs.m_bMinusFlag)
            return true;
        //다음단계로
        else
            ;

        std::string reverseString1 = this->m_sValue;
        std::string reverseString2 = rhs.m_sValue;
        reverse(reverseString1.begin(), reverseString1.end());
        reverse(reverseString2.begin(), reverseString2.end());

        bool bStatus = true;

        auto Len1 = reverseString1.length();
        auto Len2 = reverseString2.length();

        if( Len1 > Len2 ) {
            if(this->m_bMinusFlag && rhs.m_bMinusFlag)
                return false;
            else
                return true;
        }
        else if (Len1 < Len2 ) {
            if(this->m_bMinusFlag && rhs.m_bMinusFlag)
                return true;
            else
                return false;
        }
        // 두 숫자의 길이가 같으면, scan
        else {
            for (int i = 0; i < Len1; ++i) {
                if( reverseString1[i] > reverseString2[i] ) {
                    if( this->m_bMinusFlag && rhs.m_bMinusFlag )
                        return false;
                    else
                        return true;
                }
                else if( reverseString1[i] == reverseString2[i] ) {
                    break;
                }
                else {
                    if( this->m_bMinusFlag && rhs.m_bMinusFlag )
                        return true;
                    else
                        return false;
                }
            }
        }
        return false;
    }

    auto Bigdecimal::operator>=(const Bigdecimal &rhs) const -> bool {
        if( this == &rhs)
            return false;

        //음수 > 양수
        if(this->m_bMinusFlag && !rhs.m_bMinusFlag)
            return false;
        //양수 > 음수
        else if(!this->m_bMinusFlag && rhs.m_bMinusFlag)
            return true;
        //다음단계로
        else
            ;

        std::string reverseString1 = this->m_sValue;
        std::string reverseString2 = rhs.m_sValue;
        reverse(reverseString1.begin(), reverseString1.end());
        reverse(reverseString2.begin(), reverseString2.end());

        bool bStatus = true;

        auto Len1 = reverseString1.length();
        auto Len2 = reverseString2.length();

        if( Len1 > Len2 ) {
            if(this->m_bMinusFlag && rhs.m_bMinusFlag)
                return false;
            else
                return true;
        }
        else if (Len1 < Len2 ) {
            if(this->m_bMinusFlag && rhs.m_bMinusFlag)
                return true;
            else
                return false;
        }
        // 두 숫자의 길이가 같으면, scan
        else {
            for (int i = 0; i < Len1; ++i) {
                if( reverseString1[i] > reverseString2[i] ) {
                    if( this->m_bMinusFlag && rhs.m_bMinusFlag )
                        bStatus = false;
                    else
                        bStatus = true;
                }
                else if( reverseString1[i] == reverseString2[i] ) {
                    bStatus = true;
                }
                else {
                    if( this->m_bMinusFlag && rhs.m_bMinusFlag )
                        bStatus = true;
                    else
                        bStatus = false;
                }
            }
        }
        return bStatus;
    }

    /**
     * Float 형식의 문자열  xxxxx.bbbbb
     * @param value : 입력받은 문자열
     * @param uAboveZero : 소수점 앞 xxxxx 깂이값
     * @param uZero : 소소점 위치값
     * @param uBelowZero : 소수점 아래의 bbbbb 길이값
     */
    auto Bigdecimal::getFloatStyleInfo(std::string value
                                       , ushort &uAboveZeroLen
                                       , ushort &uZeroPos
                                       , ushort &uBelowZeroLen) noexcept -> void {
        uAboveZeroLen = 1;
        uZeroPos = 0;
        uBelowZeroLen = value.length();
        auto i = 0;
        for(auto it=value.rbegin();it!=value.rend();++it,++i){
            if(*it == '.')
                uZeroPos = i;
            else {
                uAboveZeroLen++;
            }
        }
        if (uZeroPos == 0)
            uBelowZeroLen = 0;
        else {
            uAboveZeroLen -= uZeroPos;
            uBelowZeroLen -= uZeroPos;
            uBelowZeroLen += 1;
        }
        return ;
    }

    /**
     * 다른 길의 문자열을 입력 받아서, 길이가 다르면 짧은 문자열 뒤에 '0'값으로 길이를 보정 후,
     * 숫자 형식으로 된 두 문자열의 값을 비교한다.
     * @param lhs
     * @param rhs
     * @return bool
     */
    auto Bigdecimal::compareFloatStyleBelowZero(const std::string& lhs, const std::string& rhs) noexcept -> bool {
        auto lhsLen = lhs.length();
        auto rhsLen = rhs.length();

        int differenceLen = lhsLen - rhsLen;

        std::string revisedLhs;
        std::string revisedRhs;

        if (differenceLen ==0) {
            ;
        }else if (differenceLen > 0) {
            std::string zeroString  = std::string(differenceLen, '0');
            revisedRhs = rhs + zeroString ;
            return (lhs == revisedRhs);
        }
        else {
            std::string zeroString  = std::string(differenceLen, '0');
            revisedRhs = rhs + zeroString ;
            return (revisedLhs == rhs);
        }
        return (lhs == rhs);
    }

    /**
     * 소수점 이하의 문자열을 합을 구한다.
     * @param rhs
     * @param lhs
     * @example plusFloatStyleBelowZero( 7123(=0.7123), 3(=0.3)) -> < 1,0123 >
     * @example plusFloatStyleBelowZero( 7525(=0.7525), 0005(=0.0005)) -> < 0,753 >
     * @return std::tuple<uint8_t, string> : tuple<반올림여부, 더한값>
     */
    auto Bigdecimal::plusFloatStyleBelowZero(const std::string& rhs, const std::string& lhs) noexcept -> std::tuple<uint8_t , string> {
        auto lhsLen = lhs.length();
        auto rhsLen = rhs.length();

        int differenceLen = lhsLen - rhsLen;
        int maxLen = lhsLen > rhsLen ? lhsLen : rhsLen;

        std::string revisedLhs;
        std::string revisedRhs;

        if (differenceLen ==0) {
            ;
        }else if (differenceLen > 0) {
            std::string zeroString  = std::string(differenceLen, '0');
            revisedRhs = rhs + zeroString ;
        }
        else {
            std::string zeroString  = std::string(differenceLen, '0');
            revisedRhs = rhs + zeroString ;
        }

        string plusString = plus(revisedRhs, revisedLhs);

        //반올림
        uint8_t Round = 0;
        if( plusString.length() > maxLen ) //더한 문자열이 길이가 길어졌다면,
            uint8_t Round = 1;

        string reversedPlusString;
        bool IsStop = false;
        for(int i= plusString.length()-1; i>-1 ; --i) {
            if( plusString[i]  == '0' && !IsStop )
                reversedPlusString.push_back(plusString[i]);
            else
                IsStop = true;
        }
        std::reverse(reversedPlusString.begin(), reversedPlusString.end() );

        return make_tuple(Round, reversedPlusString);
    }

    auto Bigdecimal::init(const std::string &value)-> void {
        int startPos = 0;
        for(int i=0;i<value.size();i++)
            if(value[i]!=0x20) {
                startPos = i;
                break;
            }

        if(value[startPos] == '-'){
            m_bMinusFlag = true;
            startPos++;
        }
        else if(value[startPos] == '+') {
            m_bMinusFlag = false;
            startPos++;
        }
        else
            m_bMinusFlag = false;

        //마이너스기호가 있으면, 뒤에서부터 복사
        auto sTmpValue = value.substr(startPos,value.size()-startPos);

        // 0.0000 => 0
        // -0 => 0
        // +0 => 0
        // 00.00000 => 0
        // 00.00001 => 0.00001
        bool bZero = true;
        int nZeroRepeatPos = 0;
        bool bFloatStyle = false;
        for (int i = 0; i < sTmpValue.size();++i) {
            if(sTmpValue[i] =='0' && !bFloatStyle)
                nZeroRepeatPos = i;
            if(sTmpValue[i] == '.') {
                bFloatStyle = true;
                continue;
            }
            if(sTmpValue[i] != '0') {
                bZero = false;
                break;
            }
        }

        if(bZero){
            m_sValue = "0";
            m_bMinusFlag = false;
        }else{
            if( nZeroRepeatPos > 0 ) {
                m_sValue = sTmpValue.substr(nZeroRepeatPos, sTmpValue.size()-nZeroRepeatPos);
            }
            else{
                m_sValue = sTmpValue;
            }
        }
    }

}//doori end

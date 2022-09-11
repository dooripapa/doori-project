//
// Created by jaeseong on 21. 11. 1..
//

#include "Bigdecimal.h"

namespace doori {

    Bigdecimal::Bigdecimal(const std::string& value) {
        coreValue = value;
    }

    Bigdecimal::Bigdecimal(std::string&& value) {
        coreValue = value;
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

    auto Bigdecimal::operator+(const Bigdecimal &rhs) -> Bigdecimal& {
        std::forward_list<char> ret;
        int i=coreValue.size()-1;
        int j=rhs.coreValue.size()-1;

        std::string itos;
        int iValue, jValue, upValue;
        upValue=0;
        for(;; --i, --j) {
            if(i<0 && j<0) {
                if(upValue>0)
                    ret.push_front('1');
                break;
            }

            if(i<0)
                iValue = 0;
            else
                iValue = coreValue[i]-'0';

            if(j<0)
                jValue = 0;
            else
                jValue = rhs.coreValue[j]-'0';

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
        coreValue = std::string(ret.begin(),ret.end());
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

        for_each(rhs.coreValue.begin(), rhs.coreValue.end(), [&](int i){
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
        return coreValue;
    }

    auto Bigdecimal::copyFrom(Bigdecimal &&rhs) noexcept -> void {
        coreValue = rhs.coreValue;
    }
    auto Bigdecimal::copyFrom(const Bigdecimal &rhs) noexcept -> void {
        coreValue = rhs.coreValue;
    }

    /// 숫자 형식의 문자열의 값을 마이너스를 한다.
    /// \param value1
    /// \param value2
    /// \param minusFlag  value1 > value2 이면, false, 반대면, true
    /// \return

    auto Bigdecimal::minus(std::string value1, std::string value2, bool minusFlag) -> std::string {
        std::forward_list<char> ret;

        std::cout<< "flag:"<< std::boolalpha << minusFlag << std::endl;

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
            ret.push_front(itos[0]);
        }
        return std::string{ret.begin(),ret.end()};
    }

    auto Bigdecimal::operator==(const Bigdecimal &rhs) const -> bool {
        if ( this == &rhs )
            return true;
        std::cout<< "operator==(const Bigdecimal &rhs)"<<this->coreValue<<":"<<rhs.coreValue <<std::endl;
        return (this->coreValue == rhs.coreValue);
    }

    auto Bigdecimal::operator==(Bigdecimal &&rhs) const -> bool {
        if ( this == &rhs )
            return true;

        std::cout<< "operator==(Bigdecimal &&rhs)" <<std::endl;
        return (this->coreValue == rhs.coreValue);
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
        return (this->coreValue == rhs);
    }

    auto Bigdecimal::operator==(std::string &&rhs) const -> bool {
        return (this->coreValue == rhs);
    }

    auto Bigdecimal::operator=(const std::string &rhs) -> Bigdecimal & {
        this->coreValue = rhs;
        return *this;
    }

    auto Bigdecimal::operator=(std::string &&rhs) -> Bigdecimal & {
        this->coreValue = rhs;
        return *this;
    }

    auto Bigdecimal::operator-(Bigdecimal &&rhs) -> Bigdecimal {
        std::cout<< this->coreValue << std::endl;
        std::cout<< rhs.coreValue << std::endl;
        std::string ret = minus(this->coreValue, rhs.coreValue, !(*this>rhs));
        return Bigdecimal{ret};
    }

    auto Bigdecimal::operator-(std::string &&rhs) -> Bigdecimal {
        std::string ret = minus(this->coreValue, rhs, !(*this>Bigdecimal{rhs}) );
        return Bigdecimal{ret};
    }

    auto Bigdecimal::operator-(const std::string &rhs) -> Bigdecimal {
        std::string ret = minus(this->coreValue, rhs, !(*this>Bigdecimal{rhs}));
        return Bigdecimal{ret};
    }

    auto Bigdecimal::operator-(const Bigdecimal &rhs) -> Bigdecimal {
        if( this == &rhs)
            return *this;
        std::string ret = minus(this->coreValue, rhs.coreValue, !(*this>rhs));
        return Bigdecimal{ret};
    }

    auto Bigdecimal::operator>(const Bigdecimal &rhs) const -> bool {
        if( this == &rhs)
            return false;


        for(auto it=this->coreValue.rbegin();it!=this->coreValue.rend();++it) {

        }

        std::string reverseString1 = this->coreValue;
        std::string reverseString2 = rhs.coreValue;
        reverse(reverseString1.begin(), reverseString1.end());
        reverse(reverseString2.begin(), reverseString2.end());

        bool bStatus = true;

        auto Len1 = reverseString1.length();
        auto Len2 = reverseString2.length();

        if( Len1 > Len2 )
            return true;
        else if (Len1 < Len2 )
            return false;
        else {
            for (int i = 0; i < Len1; ++i) {
                if( reverseString1[i] > reverseString2[i] )
                    bStatus = true;
                else
                    bStatus = false;
            }
        }
        return false;
    }

    auto Bigdecimal::operator>=(const Bigdecimal &rhs) const -> bool {
        if( this == &rhs)
            return true;

        std::string reverseString1 =  this->coreValue;
        std::string reverseString2 =  rhs.coreValue;
        reverse(reverseString1.begin(), reverseString1.end());
        reverse(reverseString2.begin(), reverseString2.end());

        bool bStatus = true;

        auto Len1 = reverseString1.length();
        auto Len2 = reverseString2.length();

        if( Len1 > Len2 )
            return true;
        else if (Len1 < Len2 )
            return false;
        else {
            for (int i = 0; i < Len1; ++i) {
                if( reverseString1[i] > reverseString2[i] )
                    bStatus = true;
                else
                    bStatus = false;
            }
        }
        return false;
    }

    /**
     * Float 형식의 문자열  xxxxx.bbbbb
     * @param value : 입력받은 문자열
     * @param uAboveZero : 소수점 앞 xxxxx 깂이값
     * @param uZero : 소소점 위치값
     * @param uBelowZero : 소수점 아래의 bbbbb 길이값
     */
    auto Bigdecimal::setFloatStringInfo( std::string value
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
     * 두 문자열의 값을 비교한다.
     * @param lhs
     * @param rhs
     * @return bool
     */
    auto Bigdecimal::compareRevisedFloatString(std::string lhs, std::string rhs) noexcept -> bool {
        auto lhsLen = lhs.length();
        auto rhsLen = rhs.length();

        int differenceLen = lhsLen - rhsLen;

        std::string revisedLhs;
        std::string revisedRhs;

        if (differenceLen ==0) {
            ;
        }else if (differenceLen > 0) {
            std::string zeroString  = std::string(differenceLen, '0');
            revisedRhs = rhs.append( zeroString );
            return (lhs == revisedRhs);
        }
        else {
            std::string zeroString  = std::string(differenceLen, '0');
            revisedRhs = rhs.append( zeroString );
            return (revisedLhs == rhs);
        }
        return (lhs == rhs);
    }


}//doori end

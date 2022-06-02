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
    return;
}

Bigdecimal::Bigdecimal(Bigdecimal &&rhs) {
    if(this==&rhs)
        return;

    copyFrom(std::move(rhs));
    return;
}

auto Bigdecimal::muliplicated(std::string value, char c, uint zeroCharCnt) -> std::string {
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
            stringList.push_back(muliplicated(sRet, el[m] , zeroCharCnt));
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

auto Bigdecimal::operator-(const Bigdecimal &rhs) -> Bigdecimal & {
    if( this == &rhs)
        return *this;
    coreValue = minus(this->coreValue, rhs.coreValue);
    return *this;
}

auto Bigdecimal::plus(std::string value1, std::string value2) -> std::string {
    std::forward_list<char> ret;
    int i=value1.size()-1;
    int j=value2.size()-1;

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
    return std::string(ret.begin(),ret.end());
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

auto Bigdecimal::minus(std::string value1, std::string value2) -> std::string {
    std::forward_list<char> ret;

    bool minusFlag = false;
    if ( value2 > value1 ) {
        std::swap(value1, value2);
        minusFlag = true;
    }

    int i=value1.size()-1;
    int j=value2.size()-1;

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
    return std::string(ret.begin(),ret.end());
}

auto Bigdecimal::operator==(const Bigdecimal &rhs) const -> bool {
    if ( this == &rhs )
        return true;

    return (this->coreValue == rhs.coreValue);
}

auto Bigdecimal::operator==(Bigdecimal &&rhs) const -> bool {
    if ( this == &rhs )
        return true;

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

auto Bigdecimal::operator-(Bigdecimal &&rhs) -> Bigdecimal & {
    if( this == &rhs )
        return *this;

    std::string ret = minus(this->coreValue, rhs.coreValue);
    this->coreValue = ret;

    return *this;
}

auto Bigdecimal::operator-(std::string &&rhs) -> Bigdecimal & {
    std::string ret = minus(this->coreValue, rhs);
    this->coreValue = ret;

    return *this;
}

auto Bigdecimal::operator-(const std::string &rhs) -> Bigdecimal & {
    std::string ret = minus(this->coreValue, rhs);
    this->coreValue = ret;

    return *this;
}

auto Bigdecimal::operator==(Bigdecimal rhs) const -> bool {
    return (this->coreValue==coreValue);
}

}//doori end

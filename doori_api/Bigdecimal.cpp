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
        return std::string{ret.begin(), ret.end()};
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
        return revisionRemoveBackZero(m_bMinusFlag?("-"+m_sValue):m_sValue) ;
    }

    auto Bigdecimal::copyFrom(Bigdecimal &&rhs) noexcept -> void {
        m_bMinusFlag = rhs.m_bMinusFlag;
        m_bFloatTypeFlag = rhs.m_bFloatTypeFlag;
        m_sBelowPointValue = rhs.m_sBelowPointValue;
        m_sAbovePointValue = rhs.m_sAbovePointValue;
        m_sValue = std::move(rhs.m_sValue) ;
    }
    auto Bigdecimal::copyFrom(const Bigdecimal &rhs) noexcept -> void {
        m_bMinusFlag = rhs.m_bMinusFlag;
        m_bFloatTypeFlag = rhs.m_bFloatTypeFlag;
        m_sBelowPointValue = rhs.m_sBelowPointValue;
        m_sAbovePointValue = rhs.m_sAbovePointValue;
        m_sValue = rhs.m_sValue;
    }

    /**
     * @note value1값이 항상 value2보다 커야 한다.
     * @param value1 : 양의 정수 문자열
     * @param value2 : 양의 정수 문자열
     * @return
     */
    auto Bigdecimal::minus(string value1, string value2) -> string {
        std::forward_list<char> ret;

        int i=static_cast<int>(value1.length());
        int j=static_cast<int>(value2.length());
        //배열은 0부터 시작
        i--;
        j--;

        short r,v1, v2;
        for(;i>-1 || j>-1; --i, --j) {
            v1=value1[i]-'0';
            if(j>-1)
                v2=value2[j]-'0';
            else
                v2=0;
            r=v1-v2;

            if(r<0){
                r=10+r;
                value1[i-1]--;
                ret.push_front('0'+r);
            }
            else {
                ret.push_front('0'+r);
            }
        }

        return std::string{ret.begin(),ret.end()};
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
        if ( this == &rhs )
            return true;

        if(this->m_bMinusFlag == rhs.m_bMinusFlag
        && this->m_bFloatTypeFlag == rhs.m_bFloatTypeFlag
        && this->m_sValue == rhs.m_sValue )
            return true;
        else
            return false;
    }

    auto Bigdecimal::operator==(Bigdecimal &&rhs) const -> bool {
        if ( this == &rhs )
            return true;

        if(this->m_bMinusFlag     == rhs.m_bMinusFlag
        && this->m_bFloatTypeFlag == rhs.m_bFloatTypeFlag
        && this->m_sValue         == rhs.m_sValue )
            return true;
        else
            return false;
    }


    auto Bigdecimal::operator-(Bigdecimal &&rhs) -> Bigdecimal {
        //자기자신을 빼면 0을 리턴
        if( this == &rhs)
            return Bigdecimal{"0"};

        auto belowZeroLen1 = getFloatStyleInfo(this->m_sValue);
        auto belowZeroLen2 = getFloatStyleInfo(rhs.m_sValue);
        auto v = revisionSameString(this->m_sValue, rhs.m_sValue);
        auto belowZeroLen = belowZeroLen1>belowZeroLen2?belowZeroLen1:belowZeroLen2;
        //둘다 마이너스
        if(this->m_bMinusFlag && rhs.m_bMinusFlag) {
            if( ge(v.first, v.second) ) {
                auto r = revisionAt(minus(v.first, v.second), belowZeroLen);
                return Bigdecimal{"-"+r};
            }else if( eq( v.first, v.second)) {
                return Bigdecimal{"0"};
            }else {
                auto r = revisionAt(minus(v.first, v.second), belowZeroLen);
                return Bigdecimal{r};
            }
        }
        else if(this->m_bMinusFlag && !rhs.m_bMinusFlag) {
            auto r = revisionAt(plus(this->m_sValue, rhs.m_sValue), belowZeroLen);
            return Bigdecimal{"-"+r};
        }
        else if(!this->m_bMinusFlag && rhs.m_bMinusFlag) {
            auto r = revisionAt(plus(this->m_sValue, rhs.m_sValue), belowZeroLen);
            return Bigdecimal{r};
        }
        else{
            if( ge(v.first, v.second) )
                return Bigdecimal{revisionAt(minus(v.first, v.second), belowZeroLen )};
            else if(eq(v.first, v.second) )
                return Bigdecimal{"0"};
            else
                return Bigdecimal{"-"+ revisionAt(minus(v.second, v.first), belowZeroLen)};
        }
    }

    auto Bigdecimal::operator-(std::string &&rhs) -> Bigdecimal {
//        std::string ret = minus(this->m_sValue, rhs, !(*this > Bigdecimal{rhs}) );
        std::string ret = "0";
        return Bigdecimal{ret};
    }

    auto Bigdecimal::operator-(const std::string &rhs) -> Bigdecimal {
//        std::string ret = minus(this->m_sValue, rhs, !(*this > Bigdecimal{rhs}));
        std::string ret = "0";
        return Bigdecimal{ret};
    }

    auto Bigdecimal::operator-(const Bigdecimal &rhs) -> Bigdecimal {
        //자기자신을 빼면 0을 리턴
        if( this == &rhs)
            return Bigdecimal{"0"};

        auto belowZeroLen1 = getFloatStyleInfo(this->m_sValue);
        auto belowZeroLen2 = getFloatStyleInfo(rhs.m_sValue);
        auto v = revisionSameString(this->m_sValue, rhs.m_sValue);
        auto belowZeroLen = belowZeroLen1>belowZeroLen2?belowZeroLen1:belowZeroLen2;
        //둘다 마이너스
        if(this->m_bMinusFlag && rhs.m_bMinusFlag) {
            if( ge(v.first, v.second) ) {
                auto r = revisionAt(minus(v.first, v.second), belowZeroLen);
                return Bigdecimal{"-"+r};
            }else if( eq( v.first, v.second)) {
                return Bigdecimal{"0"};
            }else {
                auto r = revisionAt(minus(v.first, v.second), belowZeroLen);
                return Bigdecimal{r};
            }
        }
        else if(this->m_bMinusFlag && !rhs.m_bMinusFlag) {
            auto r = revisionAt(plus(this->m_sValue, rhs.m_sValue), belowZeroLen);
            return Bigdecimal{"-"+r};
        }
        else if(!this->m_bMinusFlag && rhs.m_bMinusFlag) {
            auto r = revisionAt(plus(this->m_sValue, rhs.m_sValue), belowZeroLen);
            return Bigdecimal{r};
        }
        else{
            if( gt(v.first, v.second) )
                return Bigdecimal{revisionAt(minus(v.first, v.second), belowZeroLen )};
            else if(eq(v.first, v.second) )
                return Bigdecimal{"0"};
            else
                return Bigdecimal{"-"+ revisionAt(minus(v.second, v.first), belowZeroLen)};
        }
    }

    auto Bigdecimal::operator>(const Bigdecimal &rhs) const -> bool {
        if(this==&rhs)
            return false;

        auto v= revisionSameString(this->m_sValue, rhs.m_sValue);

        if(this->m_bMinusFlag && rhs.m_bMinusFlag) {
            if(gt(v.first, v.second))
                return false;
            else
                return true;
        }else if( !this->m_bMinusFlag && rhs.m_bMinusFlag )
            return true;
        else if( !this->m_bMinusFlag && !rhs.m_bMinusFlag ){
            if( ge(v.first, v.second) )
                return true;
            else
                return false;
        }else
            return false;
    }

    auto Bigdecimal::operator>=(const Bigdecimal &rhs) const -> bool {
        if(this==&rhs)
            return true;

        auto v= revisionSameString(this->m_sValue, rhs.m_sValue);

        if(this->m_bMinusFlag && rhs.m_bMinusFlag) {
            if(gt(v.first, v.second))
                return false;
            else
                return true;
        }else if( !this->m_bMinusFlag && rhs.m_bMinusFlag )
            return true;
        else if( !this->m_bMinusFlag && !rhs.m_bMinusFlag ){
            if( ge(v.first, v.second) )
                return true;
            else
                return false;
        }else
            return false;
    }

    /**
     * Float 형식의 문자열  xxxxx.bbbbb
     * @param value : 입력받은 문자열
     * @param uAboveZero : 소수점 앞 xxxxx 정수값 길이
     * @param uZero : 소소점 위치값
     * @param uBelowZero : 소수점 아래의 bbbbb 소수점 길이값
     */
    auto Bigdecimal::getFloatStyleInfo(const std::string& value
                                       , ushort &uAboveZeroLen
                                       , ushort &uZeroPos
                                       , ushort &uBelowZeroLen) const noexcept -> void {
        uAboveZeroLen = 0;
        uZeroPos = 0;
        uBelowZeroLen = value.length();
        for (int i = 0; i < value.length(); ++i) {
            if(value[i] == '.') {
                uZeroPos = i;
                break;
            }
        }

        if (uZeroPos == 0) {
            uAboveZeroLen = value.size()+1;
            uBelowZeroLen = 0;
        }
        else {
            uAboveZeroLen = uZeroPos;
            uBelowZeroLen = value.size() - (uZeroPos+1);
        }
    }

    auto Bigdecimal::init(const std::string &value)-> void {
        int startPos = 0;
        for(int i=0;i<value.size();i++)
            if(value[i]!=0x20) {
                startPos = i;
                break;
            }

        //앞 부분 기호(+, -) 판단
        if(value[startPos] == '-'){
            m_bMinusFlag = true;
            startPos++;
        }
        else if(value[startPos] == '+') {
            m_bMinusFlag = false;
            startPos++;
        }
        else {
            m_bMinusFlag = false;
        }

        //마이너스기호가 있으면, 뒤에서부터 복사
        auto sTmpValue = value.substr(startPos,value.size()-startPos);

        // 0.0000 => 0
        // -0 => 0
        // +0 => 0
        // 00.00000 => 0
        // 00.00001 => 0.00001
        bool bZero = true;
        bool bFloatStyle = false;
        int  nZeroRepeatPos = 0;
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
            m_bFloatTypeFlag = false;
        }else{
            if( nZeroRepeatPos > 0 ) {
                m_sValue = sTmpValue.substr(nZeroRepeatPos, sTmpValue.size()-nZeroRepeatPos);
            }
            else{
                m_sValue = sTmpValue;
            }
        }

        if( bFloatStyle ) {
            m_bFloatTypeFlag = true;
            ushort uAbovePointLen=0;
            ushort uBelowPointLen=0;
            ushort uPointPos=0;
            getFloatStyleInfo(m_sValue,uAbovePointLen, uPointPos, uBelowPointLen );
            m_sAbovePointValue = m_sValue.substr(0, uAbovePointLen);
            m_sBelowPointValue = m_sValue.substr(uPointPos+1, uBelowPointLen);
        }
        else
            m_bFloatTypeFlag = false;
    }

    /**
     * v1 >= v2보다 크면 true 그외 false
     * @param v1 양의 수
     * @param v2 양의 수
     * @return
     */
    auto Bigdecimal::ge(std::string v1, std::string v2) const noexcept -> bool {
        bool bStatus = true;

        auto Len1 = v1.length();
        auto Len2 = v2.length();

        if( Len1 > Len2 )
            return true;
        else if (Len1 < Len2 )
            return false;
        // 두 숫자의 길이가 같으면, scan
        else {
            for (int i = 0; i < Len1; ++i) {
                if( v1[i] > v2[i] )
                    return true;
                else if( v1[i] == v2[i] )
                    ;
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
    auto Bigdecimal::eq(std::string v1, std::string v2) const noexcept  -> bool {
        bool bStatus = true;

        reverse(v1.begin(), v1.end());
        reverse(v2.begin(), v2.end());

        auto Len1 = v1.length();
        auto Len2 = v2.length();

        if( Len1 != Len2 )
            return false;
        else {
            for (int i = 0; i < Len1; ++i) {
                if( v1[i] == v2[i] )
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
    auto Bigdecimal::gt(std::string v1, std::string v2) const noexcept -> bool {
        if( ge(v1,v2) )
        {
            if( eq(v1,v2) )
                return false;
            else
                return true;
        }
        return false;
    }


    /**
     * float string 두개를 소수점 이하 길이에 대해서 보전한다.
     * 1.234, 12.34 이면  1234, 12340
     * 111.002 111.00002 이면 1110020, 1110002
     * 111.01, 0.001 이면 111010, 1
     * @note
     * @param v1 양의 소수점 문자열, 또는 양의 수 문자열
     * @param v2 양의 소수점 문자열, 또는 양의 수 문자열
     * @return
     */
    auto Bigdecimal::revisionSameString(const string &v1,
                                        const string &v2) const noexcept -> std::pair<std::string, std::string> {
        ushort v1AboveLen=0;
        ushort v1PointPos=0;
        ushort v1BelowLen=0;

        ushort v2AboveLen=0;
        ushort v2PointPos=0;
        ushort v2BelowLen=0;

        getFloatStyleInfo(v1, v1AboveLen, v1PointPos, v1BelowLen);
        getFloatStyleInfo(v2, v2AboveLen, v2PointPos, v2BelowLen);

        uint AbsolutedMultiplyLen1 = 0;
        uint AbsolutedMultiplyLen2 = 0;

        AbsolutedMultiplyLen1 = v1BelowLen>=v2BelowLen?0:v2BelowLen-v1BelowLen;
        AbsolutedMultiplyLen2 = v2BelowLen>=v1BelowLen?0:v1BelowLen-v2BelowLen;

        string v1Temp;
        string v2Temp;

        v1Temp = v1.substr(0, v1AboveLen) + v1.substr(v1PointPos+1, v1BelowLen);
        for(int i=0;i<AbsolutedMultiplyLen1;++i)
            v1Temp+='0';

        v2Temp = v2.substr(0, v2AboveLen) + v2.substr(v2PointPos+1, v2BelowLen);
        for(int i=0;i<AbsolutedMultiplyLen2;++i)
            v2Temp+='0';

        return make_pair(revisionRemoveFrontZero(v1Temp), revisionRemoveFrontZero(v2Temp));
    }

    /**
     * float type, >= true 그외 false
     * @param v1 양의 수
     * @param v2 양의 수
     * @return
     */
    auto Bigdecimal::fge(std::string v1, std::string v2) const noexcept -> bool {
        auto vPair = revisionSameString(v1,v2);
        return ge(vPair.first, vPair.second);
    }

    /**
     * float type, == true 그외 false
     * @param v1 양의 수
     * @param v2 양의 수
     * @return
     */
    auto Bigdecimal::feq(std::string v1, std::string v2) const noexcept -> bool {
        auto vPair = revisionSameString(v1,v2);
        return eq(vPair.first, vPair.second);
    }

    /**
     * float type, > true 그외 false
     * @param v1 양의 수
     * @param v2 양의 수
     * @return
     */
    auto Bigdecimal::fgt(std::string v1, std::string v2) const noexcept -> bool {
        auto vPair = revisionSameString(v1,v2);
        return gt(vPair.first, vPair.second);
    }

    /**
     * 000010 -> 10
     * @param v1
     * @return
     */
    auto Bigdecimal::revisionRemoveFrontZero(const string &v1) const noexcept -> std::string {
        uint startPos = 0;
        for (int i = 0; i < v1.length(); ++i) {
            if(v1[i] =='0')
                startPos++;
            else
                break;
        }
        return v1.substr(startPos, v1.length()-startPos);
    }

    auto Bigdecimal::getFloatStyleInfo(const string &value) const noexcept -> ushort {
        ushort uZeroPos = 0;
        ushort uBelowZeroLen = value.length();
        for (int i = 0; i < value.length(); ++i) {
            if(value[i] == '.') {
                uZeroPos = i;
                break;
            }
        }

        if (uZeroPos == 0) {
            uBelowZeroLen = 0;
        }
        else {
            uBelowZeroLen = value.size() - (uZeroPos+1);
        }
        return uBelowZeroLen;
    }

    /*
     * v1 문자열에 소수점 길에 문자열 "." :at 넣는다.
     */
    auto Bigdecimal::revisionAt(const string &v1, ushort belowZeroLen) -> std::string {
        string r = v1;
        if(belowZeroLen>0)
            r.insert(r.length()-belowZeroLen, ".");
        return r;
    }

    /**
     * 뒤에 있는 0를 삭제 함
     * 0.10000 -> 0.1
     * 1000 -> 1000
     * @param v1
     * @return
     */
    auto Bigdecimal::revisionRemoveBackZero(const string &v1) const noexcept -> std::string {
        uint count= 0;
        bool stop = false;
        bool can = false;
        for (int i = v1.length()-1; i > -1; --i) {
            if( v1[i] =='0' && !stop )
                count++;
            else {
                if(v1[i]=='.')
                {
                    can = true;
                    break;
                }
                stop = true;
            }
        }
        if(can) {
            if(v1[v1.length()-(count+1)] =='.')
                return v1.substr(0, v1.length()-(count+1));
            return v1.substr(0, v1.length()-count);
        }

        return v1;
    }

}//doori end

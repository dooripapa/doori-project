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
    /// \param value1 양의 수
    /// \param value2 양의 수
    /// \param minusFlag  value1 > value2 이면, false, 반대면, true
    /// \return
    auto Bigdecimal::minus(std::string value1, std::string value2) -> std::string {
        std::forward_list<char> ret;

        if ( value2 > value1 ) {
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
        return Bigdecimal{ret};
    }

    auto Bigdecimal::operator-(const std::string &rhs) -> Bigdecimal {
        std::string ret = minus(this->m_sValue, rhs, !(*this > Bigdecimal{rhs}));
        return Bigdecimal{ret};
    }

    auto Bigdecimal::operator-(const Bigdecimal &rhs) -> Bigdecimal {
        //자기자신을 빼면 0을 리턴
        if( this == &rhs)
            return Bigdecimal{"0"};
        std::string ret = minus(this->m_sValue, rhs.m_sValue, !(*this > rhs) );
        return Bigdecimal{ret};
    }

    auto Bigdecimal::operator>(const Bigdecimal &rhs) const -> bool {
        if(this==&rhs)
            return true;

        if( !m_bFloatTypeFlag )
        {
            if(this->m_bMinusFlag && rhs.m_bMinusFlag) {
                if(gt(this->m_sValue, rhs.m_sValue))
                    return false;
                else
                    return true;
            }else if( !this->m_bMinusFlag && rhs.m_bMinusFlag )
                return true;
            else if( !this->m_bMinusFlag && !rhs.m_bMinusFlag ){
                if( ge(this->m_sValue, rhs.m_sValue) )
                    return true;
                else
                    return false;
            }else
                return false;
        }
        else{
            if(this->m_bMinusFlag && rhs.m_bMinusFlag) {
                if(fgt(*this, rhs))
                    return false;
                else
                    return true;
            }else if( !this->m_bMinusFlag && rhs.m_bMinusFlag )
                return true;
            else if( !this->m_bMinusFlag && !rhs.m_bMinusFlag ){
                if(fge(*this, rhs))
                    return true;
                else
                    return false;
            }else
                return false;
        }
    }

    auto Bigdecimal::operator>=(const Bigdecimal &rhs) const -> bool {
        if(this==&rhs)
            return true;

        if( !m_bFloatTypeFlag )
        {
            if(this->m_bMinusFlag && rhs.m_bMinusFlag) {
                if(gt(this->m_sValue, rhs.m_sValue))
                    return false;
                else
                    return true;
            }else if( !this->m_bMinusFlag && rhs.m_bMinusFlag )
                return true;
            else if( !this->m_bMinusFlag && !rhs.m_bMinusFlag ){
                if( ge(this->m_sValue, rhs.m_sValue) )
                    return true;
                else
                    return false;
            }else
                return false;
        }
        else{
            if(this->m_bMinusFlag && rhs.m_bMinusFlag) {
                if(fgt(*this, rhs))
                    return false;
                else
                    return true;
            }else if( !this->m_bMinusFlag && rhs.m_bMinusFlag )
                return true;
            else if( !this->m_bMinusFlag && !rhs.m_bMinusFlag ){
                if(fgt(*this, rhs))
                    return true;
                else
                    return false;
            }else
                return false;
        }
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
                                       , ushort &uBelowZeroLen) const noexcept -> void {
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
            m_sAbovePoint = m_sValue.substr(0, uAbovePointLen);
            m_sBelowPoint = m_sValue.substr(uPointPos, uBelowPointLen);
        }
    }

    /**
     * v1 >= v2보다 크면 true 그외 false
     * @param v1 양의 수
     * @param v2 양의 수
     * @return
     */
    auto Bigdecimal::ge(std::string v1, std::string v2) const noexcept -> bool {
        bool bStatus = true;

        reverse(v1.begin(), v1.end());
        reverse(v2.begin(), v2.end());

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
     * float string 두개를 소수점 이하 길이를 보전한다
     * @param v1 양의 수
     * @param v2 양의 수
     * @return
     */
    auto Bigdecimal::returnSameStringAsLen(const string &v1,
                                           const string &v2) const noexcept -> std::pair<std::string, std::string> {
        LOG(DEBUG, "v1[",v1,"]", "  ", "v2[", v2, "]");
        ushort v1AboveLen=0;
        ushort v1PointPos=0;
        ushort v1BelowLen=0;

        ushort v2AboveLen=0;
        ushort v2PointPos=0;
        ushort v2BelowLen=0;

        getFloatStyleInfo(v1, v1AboveLen, v1PointPos, v1BelowLen);
        getFloatStyleInfo(v2, v2AboveLen, v2PointPos, v2BelowLen);

        if( v1PointPos > 0 && v2PointPos > 0 )


        return std::pair<std::string, std::string>();
    }

    /**
     * float type, >= true 그외 false
     * @param v1 양의 수
     * @param v2 양의 수
     * @return
     */
    auto Bigdecimal::fge(const string& v1, const string& v2) const noexcept -> bool {
    }

    /**
     * float type, == true 그외 false
     * @param v1 양의 수
     * @param v2 양의 수
     * @return
     */
    auto Bigdecimal::feq(const string& v1, const string& v2) const noexcept -> bool {
    }

    /**
     * float type, > true 그외 false
     * @param v1 양의 수
     * @param v2 양의 수
     * @return
     */
    auto Bigdecimal::fgt(const string &v1, const string &v2) const noexcept -> bool {
    }

}//doori end

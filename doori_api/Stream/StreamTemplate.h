//
// Created by jaeseong on 23. 6. 5.
//

#ifndef DOORI_PROJECT_STREAMTEMPLATE_H
#define DOORI_PROJECT_STREAMTEMPLATE_H


#include "IHeader.h"
#include "IBody.h"
#include <vector>
#include "Common/Error.h"
#include "Common/Util.h"
#include <iostream>

using namespace std;

namespace doori::Stream {

    /**
     * 컨셉을 정의 합니다. StreamTemplate<Header, Body>에서
     * Header는 특정 인터페이스를 상속받은 객체로 강제합니다.
     * @tparam T
     */
    template<typename T>
    concept IHeaderInterface = requires(T obj) {
        { obj.GetLength() } -> std::same_as<int>;
        { obj.ToStream() } -> std::same_as<vector<char>>;
    };
    /**
     * 컨셉을 정의 합니다. StreamTemplate<Header, Body>에서
     * Body특정 인터페이스를 상속받은 객체로 강제합니다.
     * @tparam T
     */
    template<typename T>
    concept IBodyInterface = requires(T obj) {
        { obj.GetLength() } -> std::same_as<int>;
        { obj.ToStream() } -> std::same_as<vector<char>>;
    };

    template<IHeaderInterface Header, IBodyInterface Body>
    class StreamTemplate : Common::Error{
    public:
        StreamTemplate(Header h, Body B) : Common::Error(0, true) {

        }

    private:

        class Protocol {
        public:
            Protocol();
            Protocol(const Protocol& rhs);
            Protocol(Protocol&&) = delete;
            Protocol& operator=(const Protocol& rhs);
            Protocol& operator=(Protocol&&) = delete;
        private:
            char mLen[8];
            char mCoder[8];
            char mEndian[8];
            char mDataFormat[8];
        };
        Protocol mProtocol;
    };

    template<typename T>
    StreamTemplate<T>::Protocol &StreamTemplate<T>::Protocol::operator=(const StreamTemplate::Protocol &rhs) {
        if(this == &rhs)
            return *this;
        memcpy(this->mLen, rhs.mLen, sizeof(this->mLen) );
        memcpy(this->mCoder, rhs.mCoder, sizeof(this->mCoder) );
        memcpy(this->mEndian, rhs.mEndian, sizeof(this->mEndian) );
        memcpy(this->mDataFormat, rhs.mDataFormat, sizeof(this->mDataFormat) );

        return *this;
    }

    template<typename T>
    StreamTemplate<T>::Protocol::Protocol(const StreamTemplate::Protocol &rhs) {
        if(this == &rhs)
            return;
        memcpy(this->mLen, rhs.mLen, sizeof(this->mLen) );
        memcpy(this->mCoder, rhs.mCoder, sizeof(this->mCoder) );
        memcpy(this->mEndian, rhs.mEndian, sizeof(this->mEndian) );
        memcpy(this->mDataFormat, rhs.mDataFormat, sizeof(this->mDataFormat) );
    }
};

#endif //DOORI_PROJECT_STREAMTEMPLATE_H

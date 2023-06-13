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

    enum struct CODER{
        ASCII
        ,UTF8
        ,UTF16
    };

    enum struct ENDIAN{
        BIG
        ,LITTLE
    };

    enum struct DATA_FORMAT{
        SOLID
        ,JSON
        ,XML
        ,CSV
    };

    /**
     * 컨셉을 정의 합니다. StreamTemplate<Header, Body>에서
     * Header는 특정 인터페이스를 상속받은 객체로 강제합니다.
     * @tparam T
     */
    template<typename T>
    concept IHeaderInterface = requires(T obj) {
        { obj.GetLength() } -> std::same_as<long>;
        { obj.ToStream() } -> std::same_as<vector<char>>;
    };
    /**
     * 컨셉을 정의 합니다. StreamTemplate<Header, Body>에서
     * Body특정 인터페이스를 상속받은 객체로 강제합니다.
     * @tparam T
     */
    template<typename T>
    concept IBodyInterface = requires(T obj) {
        { obj.GetLength() } -> std::same_as<long>;
        { obj.ToStream() } -> std::same_as<vector<char>>;
    };

    /**
     * StreamTemplate 객체는, Topology의 node간 송수신데이터를 만드는 Stream 추상객체
     * StreamTemplate 전용 protocol와, 유저가 정의하는 Header, Body로 구성한다.
     * 유저가 정의하는 Header와, Body는 참조로 입력 받기 때문에, Lifetime은 개발자가 관리해야 한다.
     * @note Header, Body lifetime 의 직접관리
     * @tparam Header Stream/IHeader 인터페이스 상속받아 구현된 객체
     * @tparam Body Stream/IBody 인터페이스 상속받아 구현된 객체
     */
    template<IHeaderInterface Header, IBodyInterface Body>
    class StreamTemplate : Common::Error{
    public:

        StreamTemplate(CODER coder, ENDIAN endian, DATA_FORMAT dataFormat, Header& h, Body& b);
        StreamTemplate(const StreamTemplate&) = delete;
        StreamTemplate(StreamTemplate&&) = delete;
        StreamTemplate& operator=(const StreamTemplate&) = delete;
        StreamTemplate& operator=(StreamTemplate&&) = delete;

        vector<char> ToStream();

    private:

        constexpr static int K_CODER_LEN = 8;
        constexpr static int K_ENDIAN_LEN = 8;
        constexpr static int K_DATAFORMAT_LEN = 8;

        string convertCoder();
        string convertEndian();
        string convertDataFormat();

        class Protocol {
        public:
            Protocol() = default;
            Protocol(const Protocol& rhs);
            Protocol(Protocol&&) = delete;
            Protocol& operator=(const Protocol& rhs);
            Protocol& operator=(Protocol&&) = delete;
        private:
            char mCoder[K_CODER_LEN];
            char mEndian[K_ENDIAN_LEN];
            char mDataFormat[K_DATAFORMAT_LEN];
        };

        CODER mCoder;
        ENDIAN mEndian;
        DATA_FORMAT mDataFormat;

        Protocol mStreamProtocol;
        Header& mHeader;
        Body& mBody;

    };

    /**
     * ENUM -> string
     * @tparam Header IHeader인터페이스를 상속받은 객체형
     * @tparam Body IBody인터페이스를 상속받은 객체형
     * @return string
     */
    template<IHeaderInterface Header, IBodyInterface Body>
    string StreamTemplate<Header, Body>::convertDataFormat() {
        ostringstream oss;
        switch (mDataFormat) {
            case DATA_FORMAT::SOLID:
                oss << setw(K_DATAFORMAT_LEN) << "SOLID";
                break;
            case DATA_FORMAT::JSON:
                oss << setw(K_DATAFORMAT_LEN) << "JSON";
                break;
            case DATA_FORMAT::XML:
                oss << setw(K_DATAFORMAT_LEN) << "XML";
                break;
            case DATA_FORMAT::CSV:
                oss << setw(K_DATAFORMAT_LEN) << "CSV";
                break;
        }
        return oss.str();
    }

    /**
     * ENUM -> string
     * @tparam Header IHeader인터페이스를 상속받은 객체형
     * @tparam Body IBody인터페이스를 상속받은 객체형
     * @return string
     */
    template<IHeaderInterface Header, IBodyInterface Body>
    string StreamTemplate<Header, Body>::convertCoder() {
        ostringstream oss;
        switch (mCoder) {
            case CODER::ASCII:
                oss << setw(K_CODER_LEN) << "ASCII";
                break;
            case CODER::UTF8:
                oss << setw(K_CODER_LEN) << "UTF8";
                break;
            case CODER::UTF16:
                oss << setw(K_CODER_LEN) << "UTF16";
                break;
        }
        return oss.str();
    }

    /**
     * ENUM -> string
     * @tparam Header IHeader인터페이스를 상속받은 객체형
     * @tparam Body IBody인터페이스를 상속받은 객체형
     * @return string
     */
    template<IHeaderInterface Header, IBodyInterface Body>
    string StreamTemplate<Header, Body>::convertEndian() {
        ostringstream oss;
        switch (mEndian) {
            case ENDIAN::BIG:
                oss << setw(K_ENDIAN_LEN) << "BIG";
                break;
            case ENDIAN::LITTLE:
                oss << setw(K_ENDIAN_LEN) << "LITTLE";
                break;
        }
        return oss.str();
    }

    /**
     * Topologies 노드간 송수신 bytes를 만듭니다.
     * @tparam Header IHeader인터페이스를 상속받은 객체형
     * @tparam Body IBody인터페이스를 상속받은 객체형
     * @return vector<char> == bytes
     */
    template<IHeaderInterface Header, IBodyInterface Body>
    vector<char> StreamTemplate<Header, Body>::ToStream() {

        long bytesLength = sizeof(mCoder) + sizeof(mEndian) + sizeof(DATA_FORMAT) + mHeader.GetLength() + mBody.GetLength();
        long bytesLengthBufferSize = sizeof(bytesLength);

        vector<char> stream{};

        char acTemp[8];
        sprintf(acTemp, "%08ld", bytesLength);

        // 길이값
        int i = 0;
        for (i = 0; i < sizeof(acTemp); i++)
            stream.push_back(acTemp[i]);

        // Coder값
        memset(acTemp, 0x00, sizeof(8));
        convertCoder(acTemp);
        for (i = 0; i < sizeof(acTemp); i++)
            stream.push_back(acTemp[i]);

        // Endian 값
        memset(acTemp, 0x00, sizeof(8));
        convertEndian(acTemp);
        for (i = 0; i < sizeof(acTemp); i++)
            stream.push_back(acTemp[i]);

        memset(acTemp, 0x00, sizeof(8));
        convertDataFormat(acTemp);
        for (i = 0; i < sizeof(acTemp); i++)
            stream.push_back(acTemp[i]);

        auto headerVector=mHeader.ToStream();
        for(auto it=headerVector.cbegin(); it!=headerVector.cend();it++)
            stream.push_back(*it);

        auto bodyVector=mBody.ToStream();
        for(auto it=bodyVector.cbegin(); it!=bodyVector.cend();it++)
            stream.push_back(*it);

        return stream;
    }

    /**
     * StreamTemplate 생성자
     * @tparam Header IHeader인터페이스를 상속받은 객체형
     * @tparam Body IBody인터페이스를 상속받은 객체형
     * @param coder Stream::CODER::ASCII
     * @param endian Stream::ENDIAN::LITTLE
     * @param dataFormat Stream::ENDIAN::DATA_FORMAT
     * @param h IHeader인터페이스를 상속받은 구상객체
     * @param b IBody인터페이스를 상속받은 구상객체
     */
    template<IHeaderInterface Header, IBodyInterface Body>
    StreamTemplate<Header, Body>::StreamTemplate(CODER coder, ENDIAN endian, DATA_FORMAT dataFormat, Header& h, Body& b)
    : mCoder(coder), mEndian(endian), mDataFormat(dataFormat), mHeader(h), mBody(b), Common::Error(0, true){

    }

    /**
     * StreamTemplate 전용프로토콜 복사대입연산자
     * @tparam Header
     * @tparam Body
     * @param rhs
     * @return
     */
    template<IHeaderInterface Header, IBodyInterface Body>
    StreamTemplate<Header, Body>::Protocol &StreamTemplate<Header, Body>::Protocol::operator=(const StreamTemplate::Protocol &rhs) {
        if(this == &rhs)
            return *this;
        memcpy(this->mCoder, rhs.mCoder, sizeof(this->mCoder) );
        memcpy(this->mEndian, rhs.mEndian, sizeof(this->mEndian) );
        memcpy(this->mDataFormat, rhs.mDataFormat, sizeof(this->mDataFormat) );

        return *this;
    }

    /**
     * StreamTemplate 전용프로토콜 복사생성자
     * @tparam Header
     * @tparam Body
     * @param rhs
     */
    template<IHeaderInterface Header, IBodyInterface Body>
    StreamTemplate<Header, Body>::Protocol::Protocol(const StreamTemplate::Protocol &rhs) {
        if(this == &rhs)
            return;
        memcpy(this->mCoder, rhs.mCoder, sizeof(this->mCoder) );
        memcpy(this->mEndian, rhs.mEndian, sizeof(this->mEndian) );
        memcpy(this->mDataFormat, rhs.mDataFormat, sizeof(this->mDataFormat) );
    }

};

#endif //DOORI_PROJECT_STREAMTEMPLATE_H

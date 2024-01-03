//
// Created by jaeseong on 23. 6. 5.
//

#ifndef DOORI_PROJECT_STREAMTEMPLATE_H
#define DOORI_PROJECT_STREAMTEMPLATE_H


#include "IHeader.h"
#include "IBody.h"
#include <vector>
#include <string>
#include "Common/Error.h"
#include "Common/Util.h"
#include "Communication/IIPCTopologyNode.h"
#include <iostream>

using namespace std;

namespace doori::api::Stream {

    enum struct ENCODING_TYPE{
        ASCII ,UTF8 ,UTF16
    };

    enum struct ENDIAN{
        BIG ,LITTLE
    };

    enum struct DATA_FORMAT{
        SOLID ,JSON ,XML ,CSV
    };

    /**
     * 컨셉을 정의 합니다. StreamTemplate<Header, Body>에서
     * Header는 특정 인터페이스를 상속받은 객체로 강제합니다.
     * @tparam T
     */
    template<typename T>
    concept IHeaderInterface = requires(T obj, string str) {
        { obj.GetLength() } -> std::same_as<long>;
        { obj.ToStream() } -> std::same_as<vector<char>>;
        { obj.FromStream(str) } -> std::same_as<int>;
    };
    /**
     * 컨셉을 정의 합니다. StreamTemplate<Header, Body>에서
     * Body특정 인터페이스를 상속받은 객체로 강제합니다.
     * @tparam T
     */
    template<typename T>
    concept IBodyInterface = requires(T obj, string str) {
        { obj.GetLength() } -> std::same_as<long>;
        { obj.ToStream() } -> std::same_as<vector<char>>;
        { obj.FromStream(str) } -> std::same_as<int>;
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

        StreamTemplate(ENCODING_TYPE coder, ENDIAN endian, DATA_FORMAT dataFormat, Header& h, Body& b);
        StreamTemplate(Header& h, Body& b);
        StreamTemplate(const StreamTemplate&) = delete;
        StreamTemplate(StreamTemplate&&) = delete;
        StreamTemplate& operator=(const StreamTemplate&) = delete;
        StreamTemplate& operator=(StreamTemplate&&) = delete;

        vector<char> ToStream();
        void FromStream(const string& buffer);

        constexpr static int K_STREAM_UTIL_LEN = 8;
        constexpr static int K_CODER_LEN = 8;
        constexpr static int K_ENDIAN_LEN = 8;
        constexpr static int K_DATAFORMAT_LEN = 8;
        constexpr static int K_STREAM_PROTOCOL_LEN = K_CODER_LEN + K_ENDIAN_LEN + K_DATAFORMAT_LEN;

    private:

        string convertCoder();
        string convertEndian();
        string convertDataFormat();

        class StreamProtocol {
        public:
            StreamProtocol() = default;
            StreamProtocol(const StreamProtocol& rhs);
            StreamProtocol(StreamProtocol&&) = delete;
            StreamProtocol& operator=(const StreamProtocol& rhs);
            StreamProtocol& operator=(StreamProtocol&&) = delete;
            void FromStream(const string& buffer);
        private:
            char mCoder[K_CODER_LEN]{};
            char mEndian[K_ENDIAN_LEN]{};
            char mDataFormat[K_DATAFORMAT_LEN]{};
        };

        ENCODING_TYPE mEncodingType;
        ENDIAN mEndian;
        DATA_FORMAT mDataFormat;

        StreamProtocol mStreamProtocol;
        Header& mHeader;
        Body& mBody;

    };

    template<IHeaderInterface Header, IBodyInterface Body>
    StreamTemplate<Header, Body>::StreamTemplate(Header &h, Body &b)
    : mEncodingType(ENCODING_TYPE::ASCII), mEndian(ENDIAN::LITTLE), mDataFormat(DATA_FORMAT::JSON), mHeader(h), mBody(b), Common::Error(0, true){
    }

    template<IHeaderInterface Header, IBodyInterface Body>
    void StreamTemplate<Header, Body>::StreamProtocol::FromStream(const string& buffer) {
        strncpy(mCoder, buffer.c_str(), K_CODER_LEN);
        strncpy(mEndian, buffer.c_str()+K_CODER_LEN, K_ENDIAN_LEN);
        strncpy(mDataFormat, buffer.c_str()+K_CODER_LEN +K_ENDIAN_LEN, K_DATAFORMAT_LEN);
    }

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
                oss << setw(K_DATAFORMAT_LEN) << std::left << "SOLID";
                break;
            case DATA_FORMAT::JSON:
                oss << setw(K_DATAFORMAT_LEN) << std::left << "JSON";
                break;
            case DATA_FORMAT::XML:
                oss << setw(K_DATAFORMAT_LEN) << std::left << "XML";
                break;
            case DATA_FORMAT::CSV:
                oss << setw(K_DATAFORMAT_LEN) << std::left << "CSV";
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
        switch (mEncodingType) {
            case ENCODING_TYPE::ASCII:
                oss << setw(K_CODER_LEN) << std::left << "ASCII";
                break;
            case ENCODING_TYPE::UTF8:
                oss << setw(K_CODER_LEN) << std::left << "UTF8";
                break;
            case ENCODING_TYPE::UTF16:
                oss << setw(K_CODER_LEN) << std::left << "UTF16";
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
                oss << setw(K_ENDIAN_LEN) << std::left << "BIG";
                break;
            case ENDIAN::LITTLE:
                oss << setw(K_ENDIAN_LEN) << std::left << "LITTLE";
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

        // StreamTemplate 프로토콜, User Protocol[IHeader, IBody]
        long bytesLength = K_STREAM_PROTOCOL_LEN + mHeader.GetLength() + mBody.GetLength();

        vector<char> stream{};

        char acTemp[K_STREAM_UTIL_LEN+1]={0,};
        snprintf(acTemp, K_STREAM_UTIL_LEN+1, "%0.*ld", K_STREAM_UTIL_LEN, bytesLength);

        // 길이값
        int i = 0;
        for (i = 0; i < K_STREAM_UTIL_LEN; i++)
            stream.push_back(acTemp[i]);

        // Coder값
        auto coder = convertCoder();
        for (i = 0; i < coder.size(); i++)
            stream.push_back(coder[i]);

        // Endian 값
        auto endian = convertEndian();
        for (i = 0; i < endian.size(); i++)
            stream.push_back(endian[i]);

        auto dataformat = convertDataFormat();
        for (i = 0; i < dataformat.size(); i++)
            stream.push_back(dataformat[i]);

        auto headerVector=mHeader.ToStream();
        for(auto it=headerVector.cbegin(); it!=headerVector.cend();it++)
            stream.push_back(*it);

        auto bodyVector=mBody.ToStream();
        for(auto it=bodyVector.cbegin(); it!=bodyVector.cend();it++)
            stream.push_back(*it);

        return stream;
    }

    /**
     * 수신된 데이터를 파싱합니다.
     * @tparam Header IHeader인터페이스를 상속받은 객체형
     * @tparam Body IBody인터페이스를 상속받은 객체형
     * @param buffer 길이값이 정의된 buffer제외한 순수 데이터버퍼
     */
    template<IHeaderInterface Header, IBodyInterface Body>
    void StreamTemplate<Header, Body>::FromStream(const string &buffer) {

        string streamProtocolBuffer;
        string userHeaderBuffer;
        string userBodyBuffer;

        //StreamTemplate 전용 Protocol를 파싱합니다.
        streamProtocolBuffer.assign(buffer, 0, K_STREAM_PROTOCOL_LEN);
        this->mStreamProtocol.FromStream(streamProtocolBuffer);

        //Header 구조체를 파싱합니다.
        userHeaderBuffer.assign(buffer, K_STREAM_PROTOCOL_LEN, mHeader.GetLength());
        auto ret = this->mHeader.FromStream(userHeaderBuffer);
        if(ret < 0) {
            LoggingByClientError("fail to parsing User's Header");
            return;
        }

        //Body 구조체를 파싱합니다.
        userBodyBuffer.assign(buffer, K_STREAM_PROTOCOL_LEN + mHeader.GetLength(),
                              buffer.length() - K_STREAM_PROTOCOL_LEN + mHeader.GetLength());
        ret = this->mBody.FromStream(userBodyBuffer);
        if(ret < 0) {
            LoggingByClientError("fail to parsing User's Body");
            return;
        }

    }

    /**
     * StreamTemplate 생성자
     * @tparam Header IHeader인터페이스를 상속받은 객체형
     * @tparam Body IBody인터페이스를 상속받은 객체형
     * @param coder Stream::ENCODING_TYPE::ASCII
     * @param endian Stream::ENDIAN::LITTLE
     * @param dataFormat Stream::ENDIAN::DATA_FORMAT
     * @param h IHeader인터페이스를 상속받은 구상객체
     * @param b IBody인터페이스를 상속받은 구상객체
     */
    template<IHeaderInterface Header, IBodyInterface Body>
    StreamTemplate<Header, Body>::StreamTemplate(ENCODING_TYPE coder, ENDIAN endian, DATA_FORMAT dataFormat, Header& h, Body& b)
    : mEncodingType(coder), mEndian(endian), mDataFormat(dataFormat), mHeader(h), mBody(b), Common::Error(0, true){

    }

    /**
     * StreamTemplate 전용프로토콜 복사대입연산자
     * @tparam Header
     * @tparam Body
     * @param rhs
     * @return
     */
    template<IHeaderInterface Header, IBodyInterface Body>
    StreamTemplate<Header, Body>::StreamProtocol &StreamTemplate<Header, Body>::StreamProtocol::operator=(const StreamTemplate::StreamProtocol &rhs) {
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
    StreamTemplate<Header, Body>::StreamProtocol::StreamProtocol(const StreamTemplate::StreamProtocol &rhs) {
        if(this == &rhs)
            return;
        memcpy(this->mCoder, rhs.mCoder, sizeof(this->mCoder) );
        memcpy(this->mEndian, rhs.mEndian, sizeof(this->mEndian) );
        memcpy(this->mDataFormat, rhs.mDataFormat, sizeof(this->mDataFormat) );
    }

};

#endif //DOORI_PROJECT_STREAMTEMPLATE_H

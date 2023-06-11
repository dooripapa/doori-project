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
        StreamTemplate(CODER coder, ENDIAN endian, DATA_FORMAT dataFormat, Header h, Body b);
        StreamTemplate(const StreamTemplate&) = delete;
        StreamTemplate(StreamTemplate&&) = delete;
        StreamTemplate& operator=(const StreamTemplate&) = delete;
        StreamTemplate& operator=(StreamTemplate&&) = delete;

        vector<char> ToStream();



    private:

        void convertCoder(char a[8]);
        void convertEndian(char a[8]);
        void convertDataFormat(char a[8]);

        class Protocol {
        public:
            Protocol() = default;
            Protocol(const Protocol& rhs);
            Protocol(Protocol&&) = delete;
            Protocol& operator=(const Protocol& rhs);
            Protocol& operator=(Protocol&&) = delete;
        private:
            char mCoder[8];
            char mEndian[8];
            char mDataFormat[8];
        };

        CODER mCoder;
        ENDIAN mEndian;
        DATA_FORMAT mDataFormat;

        Protocol mProtocol;
        Header& mHeader;
        Body& mBody;

    };

    template<IHeaderInterface Header, IBodyInterface Body>
    void StreamTemplate<Header, Body>::convertDataFormat(char *a) {
        switch (mDataFormat) {
            case DATA_FORMAT::SOLID:
                snprintf(a, 8, "%*.*s", 8, 8, "SOLID");
                return;
            case DATA_FORMAT::JSON:
                snprintf(a, 8, "%*.*s", 8, 8, "JSON");
                return;
            case DATA_FORMAT::XML:
                snprintf(a, 8, "%*.*s", 8, 8, "XML");
                return;
            case DATA_FORMAT::CSV:
                snprintf(a, 8, "%*.*s", 8, 8, "CSV");
                return;
        }

    }

    template<IHeaderInterface Header, IBodyInterface Body>
    void StreamTemplate<Header, Body>::convertCoder(char *a) {
        switch (mCoder) {
            case CODER::ASCII:
                snprintf(a, 8, "%*.*s", 8, 8, "ASCII");
                return;
            case CODER::UTF8:
                snprintf(a, 8, "%*.*s", 8, 8, "UTF8");
                return;
            case CODER::UTF16:
                snprintf(a, 8, "%*.*s", 8, 8, "UTF16");
                return;
        }
    }

    template<IHeaderInterface Header, IBodyInterface Body>
    void StreamTemplate<Header, Body>::convertEndian(char *a) {
        switch (mEndian) {
            case ENDIAN::BIG:
                snprintf(a, 8, "%*.*s", 8, 8, "BIG");
                return;
            case ENDIAN::LITTLE:
                snprintf(a, 8, "%*.*s", 8, 8, "LITTLE");
                return;
        }
    }

    /**
     * 최종으로 쓰여질 bytes를 만듭니다.
     * @tparam Header
     * @tparam Body
     * @return
     */
    template<IHeaderInterface Header, IBodyInterface Body>
    vector<char> StreamTemplate<Header, Body>::ToStream() {

        long bytesLength = sizeof(mCoder) + sizeof(mEndian) + sizeof(DATA_FORMAT) + mHeader.GetLength() + mBody.GetLength();
        long bytesLengthBufferSize = sizeof(bytesLength);

        vector<char> stream(bytesLengthBufferSize + bytesLength);

        char acTemp[8];
        snprintf(acTemp, 8, "%0*d", 8, bytesLength);

        stream.insert(stream.end(), begin(acTemp), end(acTemp));

        memset(acTemp, 0x00, sizeof(8));
        convertCoder(acTemp);
        stream.insert(stream.end(), begin(acTemp), end(acTemp));

        memset(acTemp, 0x00, sizeof(8));
        convertEndian(acTemp);
        stream.insert(stream.end(), begin(acTemp), end(acTemp));

        memset(acTemp, 0x00, sizeof(8));
        convertDataFormat(acTemp);
        stream.insert(stream.end(), begin(acTemp), end(acTemp));

        stream.insert(stream.end(), mHeader.ToStream());
        stream.insert(stream.end(), mBody.ToStream());

        return stream;
    }

    template<IHeaderInterface Header, IBodyInterface Body>
    StreamTemplate<Header, Body>::StreamTemplate(CODER coder, ENDIAN endian, DATA_FORMAT dataFormat, Header h, Body b)
    : mCoder(coder), mEndian(endian), mDataFormat(dataFormat), mHeader(h), mBody(b), Common::Error(0, true){

    }

    template<IHeaderInterface Header, IBodyInterface Body>
    StreamTemplate<Header, Body>::Protocol &StreamTemplate<Header, Body>::Protocol::operator=(const StreamTemplate::Protocol &rhs) {
        if(this == &rhs)
            return *this;
        memcpy(this->mCoder, rhs.mCoder, sizeof(this->mCoder) );
        memcpy(this->mEndian, rhs.mEndian, sizeof(this->mEndian) );
        memcpy(this->mDataFormat, rhs.mDataFormat, sizeof(this->mDataFormat) );

        return *this;
    }

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

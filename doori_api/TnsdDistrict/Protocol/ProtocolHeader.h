//
// Created by jaeseong on 23. 4. 28.
//
#ifndef DOORI_PROJECT_PROTOCOLHEADER_H
#define DOORI_PROJECT_PROTOCOLHEADER_H

/*
c스타일 형식의 구조체. 프로토콜 헤더는 구조체 형태(바이너리타입)
*/

#include <cstdint>
#include <string>

namespace doori {
    namespace TnsdDistrict {
        namespace Protocol {

            class ProtocolHeader {
            public:
                ProtocolHeader() = delete;
                ProtocolHeader(std::string);
                ProtocolHeader(const ProtocolHeader&);

            private:
                uint32_t mTotalSize;
                uint32_t mBodySize;
                uint32_t mIp;
                uint16_t mPort;
                uint16_t mMsgType;
                uint16_t mMsgRetCode;
            };

        }
    }
};

#endif //DOORI_PROJECT_PROTOCOLHEADER_H

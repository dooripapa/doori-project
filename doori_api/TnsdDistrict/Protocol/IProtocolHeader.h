//
// Created by jaeseong on 23. 4. 28.
//
#ifndef DOORI_PROJECT_IPROTOCOLHEADER_H
#define DOORI_PROJECT_IPROTOCOLHEADER_H

/*
c스타일 형식의 구조체. 프로토콜 헤더는 구조체 형태(바이너리타입)
*/

#include <cstdint>

namespace doori::TnsdDistrict::Protocol
{
    struct IProtocolHeader {
        uint32_t mTotalSize;
        uint32_t mIp;
        uint16_t mPort;
        uint16_t mMsgType;
        uint16_t mMsgRetCode;
    };
}

#endif //DOORI_PROJECT_IPROTOCOLHEADER_H

//
// Created by jaeseong on 23. 4. 28.
//
#ifndef DOORI_PROJECT_PROTOCOL_H
#define DOORI_PROJECT_PROTOCOL_H

#include <cstdint>
#include <string>
#include <unordered_map>

namespace doori::StreamInterface {

    enum class MSG: uint16_t
    {
        OK
        ,INTERNAL_ERROR
        ,NOTIFY
        ,LIST_UP
        ,CHANGED
        ,REPORT
        ,ALIVE
        ,PUB
        ,SUB
    };

    class ProtocolMsg {
    public:
        ProtocolMsg()
        {
            mMsgName[MSG::OK] = std::string("O.K.");
            mMsgName[MSG::INTERNAL_ERROR] = std::string("INTERNAL_ERROR");
            mMsgName[MSG::NOTIFY        ] = std::string("NOTIFY");
            mMsgName[MSG::LIST_UP       ] = std::string("LIST_UP");
            mMsgName[MSG::CHANGED       ] = std::string("CHANGED");
            mMsgName[MSG::REPORT        ] = std::string("REPORT");
            mMsgName[MSG::ALIVE         ] = std::string("ALIVE");
            mMsgName[MSG::PUB           ] = std::string("PUB");
            mMsgName[MSG::SUB           ] = std::string("SUB");
        };

    private:
        std::unordered_map<MSG, std::string> mMsgName;
    };

    class Protocol
    {
    public:
        /**
         * 싱글톤이라
         * 복사생성자 금지
         * 복사연산자 금지
         */
        Protocol(Protocol&) = delete;
        Protocol(const Protocol&) = delete;

        /**
         * 싱글톤 endpoint 호출함수
         * @return
         */
        static Protocol* GetInstance();

    private:
        Protocol(MSG msgType);
        MSG mMsgType;
        static Protocol* gInstance;
    };
}

#endif //DOORI_PROJECT_PROTOCOL_H

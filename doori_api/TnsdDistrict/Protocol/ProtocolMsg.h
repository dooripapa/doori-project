//
// Created by jaeseong on 23. 4. 28.
//
#ifndef DOORI_PROJECT_PROTOCOLMSG_H
#define DOORI_PROJECT_PROTOCOLMSG_H

#include <unordered_map>
#include "TnsdDistrict/Protocol/Protocol.h"

namespace doori::TnsdDistrict::Protocol
{
    enum class MSG_TYPE : uint16_t
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
            MSG[MSG_TYPE::OK] = std::string("O.K.");
            MSG[MSG_TYPE::INTERNAL_ERROR] = std::string("INTERNAL_ERROR");
            MSG[MSG_TYPE::NOTIFY        ] = std::string("NOTIFY");
            MSG[MSG_TYPE::LIST_UP       ] = std::string("LIST_UP");
            MSG[MSG_TYPE::CHANGED       ] = std::string("CHANGED");
            MSG[MSG_TYPE::REPORT        ] = std::string("REPORT");
            MSG[MSG_TYPE::ALIVE         ] = std::string("ALIVE");
            MSG[MSG_TYPE::PUB           ] = std::string("PUB");
            MSG[MSG_TYPE::SUB           ] = std::string("SUB");
        };

    private:
        std::unordered_map<MSG_TYPE, std::string> MSG;
    };
} ;

#endif //DOORI_PROJECT_PROTOCOLMSG_H

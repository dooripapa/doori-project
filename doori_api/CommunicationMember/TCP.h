//
// Created by jaeseong on 23. 1. 31.
//

#ifndef DOORI_PROJECT_TCP_H
#define DOORI_PROJECT_TCP_H


#include <string>
#include "CommunicationMember/IIPC.h"
#include "CommunicationMember/Endpoint.h"
#include "CommunicationMember/IConnection.h"
#include "CommunicationMember/TCPConnection.h"

using namespace std;

namespace doori::CommunicationMember {
    class TCP : public IIPC {
    public:
        TCP();
        virtual ~TCP();

        virtual auto Create(IIPCBuilder *builder) -> IConnection * override;

    };
};


#endif //DOORI_PROJECT_TCP_H

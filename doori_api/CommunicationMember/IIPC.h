//
// Created by jaeseong on 23. 2. 2.
//

#ifndef DOORI_PROJECT_IIPC_H
#define DOORI_PROJECT_IIPC_H

#include "string"
#include "CommunicationMember/IIPC.h"
#include "CommunicationMember/IIPCBuilder.h"
#include "CommunicationMember/IConnection.h"
#include "CommunicationMember/Endpoint.h"
#include "Common/Error.h"

using namespace std;

namespace doori::CommunicationMember {

    class IIPC {
    public:
        virtual auto Create(IIPCBuilder* builder) -> IConnection* = 0;
    };

} // doori

#endif //DOORI_PROJECT_IIPC_H
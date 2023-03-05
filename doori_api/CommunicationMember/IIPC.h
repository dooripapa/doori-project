//
// Created by jaeseong on 23. 2. 2.
//

#ifndef DOORI_PROJECT_IIPC_H
#define DOORI_PROJECT_IIPC_H

#include "string"
#include "CommunicationMember/IIPC.h"
#include "CommunicationMember/IIPCBuilder.h"
#include "CommunicationMember/ITopology.h"
#include "CommunicationMember/Endpoint.h"
#include "Common/Error.h"

using namespace std;

namespace doori::CommunicationMember {

    class IIPC {
    public:
        virtual int Create(IIPCBuilder *) = 0;
        virtual ITopology* GetIPC() noexcept = 0;
    };

} // doori

#endif //DOORI_PROJECT_IIPC_H

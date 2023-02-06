//
// Created by jaeseong on 23. 2. 2.
//

#ifndef DOORI_PROJECT_IIPC_H
#define DOORI_PROJECT_IIPC_H

#include "string"
#include "CommunicationMember/Endpoint.h"
#include "Common/Error.h"
#include "IConnection.h"

using namespace std;

namespace doori::CommunicationMember {

    class IIPC {
    protected:
        virtual auto Create(IIPCBuilder& builder) -> unique_ptr<IConnection> = 0;
    private:
    };

} // doori

#endif //DOORI_PROJECT_IIPC_H

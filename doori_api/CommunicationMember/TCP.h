//
// Created by jaeseong on 23. 1. 31.
//

#ifndef DOORI_PROJECT_TCP_H
#define DOORI_PROJECT_TCP_H


#include <string>
#include "CommunicationMember/IIPC.h"
#include "CommunicationMember/IConnection.h"
#include "CommunicationMember/Endpoint.h"

using namespace std;

namespace doori::CommunicationMember {
    class TCP : IIPC {
    public:
        TCP();
        virtual ~TCP();
        auto GetOS() -> string override;

    private:
        auto Create(IIPCBuilder& builder) -> unique_ptr<IConnection> override;

    private:
        TCP *mTCP = nullptr;
    };
};


#endif //DOORI_PROJECT_TCP_H

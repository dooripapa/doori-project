//
// Created by jaeseong on 23. 1. 31.
//

#ifndef DOORI_PROJECT_TCP_H
#define DOORI_PROJECT_TCP_H


#include <string>
#include "CommunicationMember/IIPC.h"
#include "CommunicationMember/Endpoint.h"

using namespace std;

namespace doori::CommunicationMember {
    class TCP : IIPC {
    public:
        TCP();
        virtual ~TCP();
    private:
        [[nodiscard]] virtual auto Create(IIPCBuilder& builder) -> const IConnection& override;
    };
};


#endif //DOORI_PROJECT_TCP_H

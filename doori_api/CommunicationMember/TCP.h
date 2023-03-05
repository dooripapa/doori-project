//
// Created by jaeseong on 23. 1. 31.
//

#ifndef DOORI_PROJECT_TCP_H
#define DOORI_PROJECT_TCP_H


#include <string>
#include "CommunicationMember/IIPC.h"
#include "CommunicationMember/Endpoint.h"
#include "CommunicationMember/ITopology.h"
#include "CommunicationMember/TCPTopology.h"

using namespace std;

namespace doori::CommunicationMember {
    class TCP : public IIPC {
    public:
        TCP();
        virtual ~TCP();

        int Create(IIPCBuilder *builder) override;

        ITopology *GetIPC() noexcept override;
    private:
        int mConnection;
    };

};


#endif //DOORI_PROJECT_TCP_H

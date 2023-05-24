//
// Created by jaeseong on 23. 1. 31.
//

#ifndef DOORI_PROJECT_TCP_H
#define DOORI_PROJECT_TCP_H


#include <string>
#include <memory>
#include "CommunicationMember/IIPC.h"
#include "CommunicationMember/Endpoint.h"
#include "CommunicationMember/IIPCTopology.h"
#include "CommunicationMember/TCPTopology.h"

using namespace std;

namespace doori::CommunicationMember {
    class TCP : public IIPC {
    public:
        TCP() = default;
        TCP(const TCP&) = default;
        TCP(TCP&&) = default;
        TCP& operator=(const TCP&) = default;
        TCP& operator=(TCP&&) = default;
        virtual ~TCP() = default;

        int Create(IIPCBuilder& builder) override;

        unique_ptr<IIPCTopology> GetIPC() noexcept override;
    };

};


#endif //DOORI_PROJECT_TCP_H

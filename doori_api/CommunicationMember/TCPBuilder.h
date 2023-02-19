//
// Created by jaeseong on 23. 1. 31.
//

#ifndef DOORI_PROJECT_TCPBUILDER_H
#define DOORI_PROJECT_TCPBUILDER_H

#include "CommunicationMember/IIPCBuilder.h"
#include "Endpoint.h"
#include <string>

using namespace std;

namespace doori::CommunicationMember {

    class TCPBuilder : public IIPCBuilder {
    public:

        TCPBuilder() = delete;
        TCPBuilder(Endpoint From, Endpoint To);

    private:
        auto getFrom() -> const Endpoint & override;

        auto getTo() -> const Endpoint & override;

    private:
        Endpoint mFrom;
        Endpoint mTo;
    };

} // doori

#endif //DOORI_PROJECT_TCPBUILDER_H
